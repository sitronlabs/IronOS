#ifndef BSP_SNP_PINS_H_
#define BSP_SNP_PINS_H_

/*
 * Solder Ninja Pen R8A - RP2040 Pin Assignments
 *
 * I2C0 Bus (400kHz):
 *   FUSB302 (0x22), PI3USB9281C (0x25), EEPROM (0x50),
 *   SSD1306 OLED (0x3C), LIS2DH12 (0x18)
 *
 * SPI1 Bus:
 *   MAX31855 thermocouple AFE, DAC5311 buck voltage setpoint
 */

// --- I2C0 ---
#define SNP_I2C0_SDA_PIN      4
#define SNP_I2C0_SCL_PIN      5

// --- SPI1 ---
#define SNP_SPI1_SCK_PIN      26
#define SNP_SPI1_MOSI_PIN     27
#define SNP_SPI1_MISO_PIN     24

// --- SPI Chip Selects ---
#define SNP_DAC_CS_PIN        20  // DAC5311 for buck converter setpoint
#define SNP_MAX31855_CS_PIN   25  // MAX31855 thermocouple AFE

// --- OLED ---
#define SNP_OLED_RESET_PIN    6

// --- Buttons (active low, internal pullup) ---
#define SNP_BUTTON_LEFT_PIN   2   // KEY_A
#define SNP_BUTTON_RIGHT_PIN  8   // KEY_B

// --- Buck Converter ---
#define SNP_BUCK_ENABLE_PIN   3   // Enable pin for DC-DC converter

// --- USB Mux/Charger Detection ---
#define SNP_MUX_CONTROL_PIN   7   // FSUSB43 + PI3USB9281C control

// --- Accelerometer LIS2DH12 ---
#define SNP_ACCEL_INT1_PIN    12  // Free-fall interrupt (FALLING)
#define SNP_ACCEL_INT2_PIN    13  // Inactivity/wake (CHANGE)

// --- Hall Effect Sensor (magnet detect) ---
#define SNP_HALL_PIN          15  // Active low when magnet detected

// --- QC/HVDCP Resistor Network ---
#define SNP_QC_DN_H_PIN       16  // D- upper resistor
#define SNP_QC_DN_L_PIN       17  // D- lower resistor
#define SNP_QC_DP_H_PIN       18  // D+ upper resistor
#define SNP_QC_DP_L_PIN       19  // D+ lower resistor
#define SNP_QC_DP_M_PIN       28  // D+ middle resistor
#define SNP_QC_DN_M_PIN       29  // D- middle resistor (also ADC3 for QC voltage sense)

// --- I2C Device Addresses ---
#define SNP_FUSB302_ADDR     (0x22)
#define SNP_PI3USB9281_ADDR  (0x25)
#define SNP_EEPROM_ADDR      (0x50)
#define SNP_OLED_ADDR        (0x3C)
#define SNP_LIS2DH12_ADDR    (0x18)

#endif /* BSP_SNP_PINS_H_ */
