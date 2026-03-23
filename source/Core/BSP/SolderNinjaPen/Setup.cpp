// Hardware setup for Solder Ninja Pen (RP2040)
//
// Initializes clocks, GPIO, I2C, SPI, and watchdog.

#include "Setup.h"
#include "Pins.h"
#include "configuration.h"

#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "hardware/watchdog.h"
#include "pico/time.h"

// SystemCoreClock is provided by Pico SDK CMSIS system_RP2040.c
extern "C" uint32_t SystemCoreClock;

void setup_i2c(void) {
  // 400 kHz matches the OLED / FUSB302 / LIS2DH12 bus on I2C0.
  i2c_init(i2c0, 400 * 1000);
  gpio_set_function(SNP_I2C0_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SNP_I2C0_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(SNP_I2C0_SDA_PIN);
  gpio_pull_up(SNP_I2C0_SCL_PIN);
}

void Setup_HAL(void) {

  // --- GPIO: Buttons (active low, pull-up) ---
  gpio_init(SNP_BUTTON_LEFT_PIN);
  gpio_set_dir(SNP_BUTTON_LEFT_PIN, GPIO_IN);
  gpio_pull_up(SNP_BUTTON_LEFT_PIN);

  gpio_init(SNP_BUTTON_RIGHT_PIN);
  gpio_set_dir(SNP_BUTTON_RIGHT_PIN, GPIO_IN);
  gpio_pull_up(SNP_BUTTON_RIGHT_PIN);

  // --- GPIO: Buck converter enable ---
  gpio_init(SNP_BUCK_ENABLE_PIN);
  gpio_set_dir(SNP_BUCK_ENABLE_PIN, GPIO_OUT);
  gpio_put(SNP_BUCK_ENABLE_PIN, 0); // Start with buck disabled

  // --- GPIO: OLED reset ---
  gpio_init(SNP_OLED_RESET_PIN);
  gpio_set_dir(SNP_OLED_RESET_PIN, GPIO_OUT);
  gpio_put(SNP_OLED_RESET_PIN, 1);

  // --- GPIO: Hall sensor (active low, pull-up) ---
  gpio_init(SNP_HALL_PIN);
  gpio_set_dir(SNP_HALL_PIN, GPIO_IN);
  gpio_pull_up(SNP_HALL_PIN);

  // --- GPIO: USB mux control ---
  gpio_init(SNP_MUX_CONTROL_PIN);
  gpio_set_dir(SNP_MUX_CONTROL_PIN, GPIO_OUT);
  gpio_put(SNP_MUX_CONTROL_PIN, 0);

  // --- GPIO: Accelerometer interrupts ---
  gpio_init(SNP_ACCEL_INT1_PIN);
  gpio_set_dir(SNP_ACCEL_INT1_PIN, GPIO_IN);
  gpio_pull_up(SNP_ACCEL_INT1_PIN);

  gpio_init(SNP_ACCEL_INT2_PIN);
  gpio_set_dir(SNP_ACCEL_INT2_PIN, GPIO_IN);
  gpio_pull_up(SNP_ACCEL_INT2_PIN);

  // Hardware I2C0. FRToSI2C takes the mutex later in preRToSInit().
  setup_i2c();

  // --- SPI1: MAX31855 + DAC5311 ---
  spi_init(spi1, 5000000); // 5 MHz for MAX31855 compatibility
  gpio_set_function(SNP_SPI1_SCK_PIN, GPIO_FUNC_SPI);
  gpio_set_function(SNP_SPI1_MOSI_PIN, GPIO_FUNC_SPI);
  gpio_set_function(SNP_SPI1_MISO_PIN, GPIO_FUNC_SPI);

  // SPI chip selects (active low, manual control)
  gpio_init(SNP_DAC_CS_PIN);
  gpio_set_dir(SNP_DAC_CS_PIN, GPIO_OUT);
  gpio_put(SNP_DAC_CS_PIN, 1);

  gpio_init(SNP_MAX31855_CS_PIN);
  gpio_set_dir(SNP_MAX31855_CS_PIN, GPIO_OUT);
  gpio_put(SNP_MAX31855_CS_PIN, 1);

  // --- QC/HVDCP resistor network pins (start as inputs) ---
  gpio_init(SNP_QC_DN_H_PIN);
  gpio_set_dir(SNP_QC_DN_H_PIN, GPIO_IN);
  gpio_init(SNP_QC_DN_L_PIN);
  gpio_set_dir(SNP_QC_DN_L_PIN, GPIO_IN);
  gpio_init(SNP_QC_DP_H_PIN);
  gpio_set_dir(SNP_QC_DP_H_PIN, GPIO_IN);
  gpio_init(SNP_QC_DP_L_PIN);
  gpio_set_dir(SNP_QC_DP_L_PIN, GPIO_IN);
  gpio_init(SNP_QC_DP_M_PIN);
  gpio_set_dir(SNP_QC_DP_M_PIN, GPIO_IN);
  gpio_init(SNP_QC_DN_M_PIN);
  gpio_set_dir(SNP_QC_DN_M_PIN, GPIO_IN);

  // --- Watchdog: 8 second timeout ---
  watchdog_enable(8000, true);

  // Update SystemCoreClock
  SystemCoreClock = clock_get_hz(clk_sys);
}
