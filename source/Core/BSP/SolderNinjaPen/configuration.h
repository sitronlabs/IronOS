#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_
#include <stdint.h>
/**
 * Configuration.h
 * Default pre settings for SolderNinjaPen (RP2040 based USB soldering iron)
 */

//===========================================================================
//============================= Default Settings ============================
//===========================================================================

#define SLEEP_TEMP         150 // Default sleep temperature
#define BOOST_TEMP         400 // Default boost temp
#define BOOST_MODE_ENABLED 1   // 0: Disable 1: Enable

#define COOLING_TEMP_BLINK 0 // 0: Disable 1: Enable

#define SLEEP_TIME    5 // x10 Seconds
#define SHUTDOWN_TIME 0 // Minutes

#define AUTO_START_MODE 0 // Default to none

#define LOCKING_MODE 0 // Default to deactivated for safety

#define ORIENTATION_MODE           2 // 0: Right 1:Left 2:Automatic - Default automatic
#define MAX_ORIENTATION_MODE       2
#define REVERSE_BUTTON_TEMP_CHANGE 0

#define MIN_BRIGHTNESS     0
#define MAX_BRIGHTNESS     100
#define BRIGHTNESS_STEP    25
#define DEFAULT_BRIGHTNESS 50

#define TEMP_CHANGE_SHORT_STEP     5
#define TEMP_CHANGE_LONG_STEP      10
#define TEMP_CHANGE_SHORT_STEP_MAX 50
#define TEMP_CHANGE_LONG_STEP_MAX  90

#define POWER_PULSE_INCREMENT    1
#define POWER_PULSE_MAX          100
#define POWER_PULSE_WAIT_MAX     9
#define POWER_PULSE_DURATION_MAX 9

#define POWER_PULSE_WAIT_DEFAULT     4
#define POWER_PULSE_DURATION_DEFAULT 1
#define POWER_PULSE_DEFAULT          0

#define SENSITIVITY 7

#define DETAILED_SOLDERING 0
#define DETAILED_IDLE      0

#define THERMAL_RUNAWAY_TIME_SEC 20
#define THERMAL_RUNAWAY_TEMP_C   2

#define CUT_OUT_SETTING          0
#define RECOM_VOL_CELL           33
#define TEMPERATURE_INF          0
#define DESCRIPTION_SCROLL_SPEED 0
#define ANIMATION_LOOP           1
#define ANIMATION_SPEED          settingOffSpeed_t::MEDIUM

#ifndef MODEL_SolderNinjaPen
#error "No model defined!"
#endif

#ifdef MODEL_SolderNinjaPen

#define SOLDERING_TEMP    320 // Default soldering temp is 320.0 C
#define VOLTAGE_DIV       360
#define PID_POWER_LIMIT   45  // 45W max
#define CALIBRATION_OFFSET 0
#define MIN_CALIBRATION_OFFSET 0
#define POWER_LIMIT       45
#define MAX_POWER_LIMIT   45
#define POWER_LIMIT_STEPS 1
#define OP_AMP_GAIN_STAGE 1 // No op-amp; MAX31855 gives digital readout
#define USB_PD_VMAX       20
#define MODEL_HAS_DCDC       // Has buck converter
#define PID_TIM_HZ        (8) // 1000ms cycle time -> ~1 Hz effective, but PID task runs at this rate
#define MAX_TEMP_C         400
#define MAX_TEMP_F         750
#define MIN_TEMP_C         10
#define MIN_TEMP_F         50
#define MIN_BOOST_TEMP_C   150
#define MIN_BOOST_TEMP_F   300

#define SLEW_LIMIT 0 // No PWM slew limit; we use time-proportional burst heating

#define ACCEL_LIS
#define HALL_SENSOR // Has hall effect sensor (magnet detect)

#define OLED_96x16 1
#define POW_PD     1
#define POW_PD_EXT 0
#define POW_QC     1 // Has QC/HVDCP support via resistor network
#define USB_PD_EPR_WATTAGE 0
#define USB_PD_TIMEOUT     20 // Default PD negotiation timeout in x100ms
// TODO: tune PD timeout / QC profile once the FUSB302 + PI3USB9281 path is verified

#define TEMP_MAX31855 // Use external MAX31855 thermocouple IC instead of ADC

// Hardware I2C0 via FRToSI2C (see I2C_Wrapper.cpp). Do not define
// I2C_SOFT_BUS_* / OLED_I2CBB* / ACCEL_I2CBB* — those select the bit-bang drivers.

#define BATTFILTERDEPTH 8

#define ACCEL_EXITS_ON_MOVEMENT
#define NEEDS_VBUS_PROBE 0

#define HARDWARE_MAX_WATTAGE_X10 450
#define TIP_THERMAL_MASS         45 // TODO: needs refinement
#define TIP_RESISTANCE           21 // x10 ohms, 2.1 ohm heater element

// ADC is not used for tip temp (MAX31855 digital), but needed for framework
#define ADC_MAX_READING (4096 * 8)
#define ADC_VDD_MV      3300

#endif /* MODEL_SolderNinjaPen */

#ifdef ACCEL_EXITS_ON_MOVEMENT
#define NO_SLEEP_MODE
#endif

// RP2040 has 2MB flash; we use the last sectors for logo and settings
// Flash base is 0x10000000 on RP2040
// Firmware occupies ~256KB; put logo at 1MB mark and settings at end
#define FLASH_LOGOADDR      (0x10000000 + (1024 * 1024))
#define SETTINGS_START_PAGE (0x10000000 + (2 * 1024 * 1024) - (4 * 1024))

#endif /* CONFIGURATION_H_ */
