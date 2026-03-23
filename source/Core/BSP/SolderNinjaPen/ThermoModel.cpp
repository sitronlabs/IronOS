// Thermo model for Solder Ninja Pen using MAX31855 thermocouple IC
//
// Data flow in IronOS:
//   getTipRawTemp() -> convertTipRawADCTouV() -> convertuVToDegC()
//
// The shared convertTipRawADCTouV() does:
//   rawInputmVX10 = (rawADC * 33000) / ADC_MAX_READING
//   valueuV = rawInputmVX10 * 100 / OP_AMP_GAIN_STAGE
//
// With OP_AMP_GAIN_STAGE=1 and ADC_MAX_READING=32768:
//   valueuV ≈ rawADC * 100.708...
//
// We encode the MAX31855 temperature (in 0.25°C units) into the raw ADC value,
// then decode it back in convertuVToDegC().
//
// The original Solder Ninja Pen firmware applies:
//   T_corrected = 2.3482 * T_max31855 - 47.426

#include "TipThermoModel.h"
#include "Utils.hpp"
#include "configuration.h"

TemperatureType_t TipThermoModel::convertuVToDegC(uint32_t tipuVDelta) {
  // tipuVDelta arrives as rawADC * (ADC_VDD_MV * 1000 / ADC_MAX_READING)
  // With our settings: rawADC * (3300000 / 32768) ≈ rawADC * 100.708
  // Our rawADC was the MAX31855 14-bit value in 0.25°C units
  // So: tipuVDelta ≈ tempQuarterDegC * 100.708
  // Recover quarter-degree value:
  float tempQuarterC = (float)tipuVDelta / 100.708f;

  // Convert from 0.25°C units to °C
  float tempC = tempQuarterC * 0.25f;

  // Apply the calibration correction from the original Solder Ninja Pen firmware
  tempC = 2.3482f * tempC - 47.426f;

  if (tempC < -50.0f || tempC > 500.0f) {
    return 0;
  }

  return (TemperatureType_t)(tempC);
}
