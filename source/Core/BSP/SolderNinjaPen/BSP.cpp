// BSP mapping functions for Solder Ninja Pen (RP2040 + MAX31855)

#include "BSP.h"
#include "Setup.h"
#include "Pins.h"
#include "Setup.h"
#include "TipThermoModel.h"
#include "configuration.h"
#include "history.hpp"
#include "main.hpp"
#include <IRQ.h>

#include "hardware/gpio.h"
#include "hardware/watchdog.h"
#include "hardware/spi.h"
#include "hardware/adc.h"
#include "pico/time.h"
#include "pico/unique_id.h"

volatile uint16_t             PWMSafetyTimer         = 0;
volatile uint8_t              pendingPWM             = 0;
uint16_t                      totalPWM               = 255;
const uint16_t                powerPWM               = 255;
static bool                   lastTipDisconnected    = true;
history<uint16_t, PID_TIM_HZ> rawTempFilter          = {{0}, 0, 0};

// MAX31855 cached readings
static volatile uint16_t snpTipTempRaw = 0;
static volatile bool     snpTipOpen    = true;

void resetWatchdog() { watchdog_update(); }

int16_t getHandleTemperature(uint8_t sample) {
  // TODO: return MAX31855 cold-junction temperature (C x10) from the SPI path
  // instead of this fixed ambient stub.
  (void)sample;
  return 250; // 25.0 C x10
}

uint16_t getTipRawTemp(uint8_t refresh) {
  if (refresh) {
    // Trigger a new SPI read of the MAX31855
    gpio_put(SNP_MAX31855_CS_PIN, 0);
    uint8_t buf[4] = {0};
    spi_read_blocking(spi1, 0, buf, 4);
    gpio_put(SNP_MAX31855_CS_PIN, 1);

    // Parse 32-bit result: bits[31:18] = thermocouple temp (signed, 0.25C/LSB)
    // bit[16] = fault, bits[15:4] = internal temp, bits[2:0] = fault flags
    uint32_t raw = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                   ((uint32_t)buf[2] << 8) | buf[3];

    bool fault = (raw >> 16) & 1;
    bool openCircuit = raw & 1;

    if (fault || openCircuit) {
      snpTipOpen = true;
      snpTipTempRaw = 0xFFFF;
    } else {
      snpTipOpen = false;
      int16_t tcRaw = (int16_t)(raw >> 18);
      if (raw & 0x80000000) {
        tcRaw |= 0xC000; // sign extend
      }
      // tcRaw is in 0.25C units; convert to a raw value the ThermoModel can use
      // We scale to match ADC-like range: store as temperature_C * 4 (0.25C resolution)
      snpTipTempRaw = (uint16_t)(tcRaw & 0x3FFF);
    }
    rawTempFilter.update(snpTipTempRaw);
    return snpTipTempRaw;
  } else {
    return rawTempFilter.average();
  }
}

uint16_t getInputVoltageX10(uint16_t divisor, uint8_t sample) {
  // The FUSB302 VBUS measurement is used in the PD stack.
  // For the BSP hook, return a nominal value since we don't have
  // a dedicated voltage divider on an ADC pin for VIN.
  (void)divisor;
  (void)sample;
  return 200; // 20.0V nominal (will be overridden by PD negotiation)
}

void setTipPWM(const uint8_t pulse, const bool shouldUseFastModePWM) {
  // The Solder Ninja Pen uses time-proportional burst heating via the buck
  // enable pin, not a hardware PWM to the element. This function controls
  // the buck converter enable.
  (void)shouldUseFastModePWM;
  if (pulse > 0) {
    gpio_put(SNP_BUCK_ENABLE_PIN, 1);
  } else {
    gpio_put(SNP_BUCK_ENABLE_PIN, 0);
  }
}

void unstick_I2C() {
  // Drop the peripheral so SCL can be toggled. A slave holding SDA low
  // releases it after a few clocks; then hand the pins back to I2C0.
  gpio_set_function(SNP_I2C0_SDA_PIN, GPIO_FUNC_SIO);
  gpio_set_function(SNP_I2C0_SCL_PIN, GPIO_FUNC_SIO);
  gpio_pull_up(SNP_I2C0_SDA_PIN);
  gpio_set_dir(SNP_I2C0_SDA_PIN, GPIO_IN);
  gpio_set_dir(SNP_I2C0_SCL_PIN, GPIO_OUT);

  int timeout = 100;
  int cnt     = 0;
  while (!gpio_get(SNP_I2C0_SDA_PIN)) {
    gpio_put(SNP_I2C0_SCL_PIN, 0);
    busy_wait_us(5);
    gpio_put(SNP_I2C0_SCL_PIN, 1);
    busy_wait_us(5);
    cnt++;
    if (cnt > timeout) {
      break;
    }
  }
  setup_i2c();
}

uint8_t getButtonA() {
  return gpio_get(SNP_BUTTON_LEFT_PIN) == 0 ? 1 : 0;
}

uint8_t getButtonB() {
  return gpio_get(SNP_BUTTON_RIGHT_PIN) == 0 ? 1 : 0;
}

void BSPInit(void) {
  // Nothing extra needed beyond Setup
}

void reboot() { watchdog_reboot(0, 0, 0); }

void delay_ms(uint16_t count) { sleep_ms(count); }

bool getHallSensorFitted() {
  // The SNP has a hall sensor on GPIO 15
  return true;
}

int16_t getRawHallEffect() {
  // Active low when magnet is detected
  return gpio_get(SNP_HALL_PIN) == 0 ? 32768 : 0;
}

void log_system_state(int32_t PWMWattsx10) {
  (void)PWMWattsx10;
  // Could log to UART if debug enabled
}

bool isTipDisconnected() {
  return snpTipOpen;
}

uint64_t getDeviceID() {
  pico_unique_board_id_t id;
  pico_get_unique_board_id(&id);
  uint64_t uid = 0;
  for (int i = 0; i < 8; i++) {
    uid |= ((uint64_t)id.id[i]) << (i * 8);
  }
  return uid;
}

uint32_t getDeviceValidation() { return 0; }
uint8_t  getDeviceValidationStatus() { return 0; }

void setStatusLED(const enum StatusLED state) {
  // The Solder Ninja Pen doesn't have a status LED
  (void)state;
}

void setBuzzer(bool on) {
  // The Solder Ninja Pen doesn't have a buzzer
  (void)on;
}

uint8_t preStartChecks() { return 1; }
uint8_t preStartChecksDone() { return 1; }

void showBootLogo(void) {
  // TODO: implement boot logo from flash
}

void showBootLogoIfavailable() {
  // No-op for now
}
