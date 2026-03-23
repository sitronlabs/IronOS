// QC/HVDCP GPIO control for Solder Ninja Pen (RP2040)
//
// The SNP uses a resistor network on GPIO 16-19, 28, 29 to generate
// specific voltage levels on USB D+/D- for HVDCP negotiation.
// GPIO 29 (ADC3) is also used to sense D- voltage during QC handshake.

#include "BSP.h"
#include "BSP_QC.h"
#include "Pins.h"
#include "configuration.h"

#include "hardware/gpio.h"
#include "hardware/adc.h"

#ifdef POW_QC

void QC_Init_GPIO() {
  // All QC pins start as inputs (high-impedance)
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
}

void QC_DPlusZero_Six() {
  // Set D+ to ~0.6V using resistor divider (HIGH on upper, LOW on lower)
  gpio_set_dir(SNP_QC_DP_H_PIN, GPIO_OUT);
  gpio_put(SNP_QC_DP_H_PIN, 1);
  gpio_set_dir(SNP_QC_DP_L_PIN, GPIO_OUT);
  gpio_put(SNP_QC_DP_L_PIN, 0);
}

void QC_DNegZero_Six() {
  // Set D- to ~0.6V using resistor divider
  gpio_set_dir(SNP_QC_DN_H_PIN, GPIO_OUT);
  gpio_put(SNP_QC_DN_H_PIN, 1);
  gpio_set_dir(SNP_QC_DN_L_PIN, GPIO_OUT);
  gpio_put(SNP_QC_DN_L_PIN, 0);
}

void QC_DPlusThree_Three() {
  // Set D+ to ~3.3V (both upper pins HIGH)
  gpio_set_dir(SNP_QC_DP_H_PIN, GPIO_OUT);
  gpio_put(SNP_QC_DP_H_PIN, 1);
  gpio_set_dir(SNP_QC_DP_L_PIN, GPIO_OUT);
  gpio_put(SNP_QC_DP_L_PIN, 1);
}

void QC_DNegThree_Three() {
  // Set D- to ~3.3V
  gpio_set_dir(SNP_QC_DN_H_PIN, GPIO_OUT);
  gpio_put(SNP_QC_DN_H_PIN, 1);
  gpio_set_dir(SNP_QC_DN_L_PIN, GPIO_OUT);
  gpio_put(SNP_QC_DN_L_PIN, 1);
}

void QC_DM_PullDown() {
  // Enable weak pull-down on D- middle pin
  gpio_set_dir(SNP_QC_DN_M_PIN, GPIO_IN);
  gpio_pull_down(SNP_QC_DN_M_PIN);
}

void QC_DM_No_PullDown() {
  gpio_disable_pulls(SNP_QC_DN_M_PIN);
}

void QC_Post_Probe_En() {
  // Enable output drivers after initial probe
  // Already handled by the individual voltage setting functions
}

uint8_t QC_DM_PulledDown() {
  // Read D- voltage using ADC on GPIO 29 (ADC3)
  adc_init();
  adc_gpio_init(SNP_QC_DN_M_PIN); // GPIO 29 = ADC3
  adc_select_input(3);
  uint16_t raw = adc_read(); // 12-bit, 0-4095
  float voltage = raw * 3.3f / 4095.0f;
  return (voltage < 0.5f) ? 1 : 0;
}

void QC_resync() {
  // Re-sync QC state if needed
  // Reset all QC pins to inputs
  QC_Init_GPIO();
}

#endif /* POW_QC */
