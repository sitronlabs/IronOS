// Power management BSP hooks for Solder Ninja Pen

#include "BSP.h"
#include "BSP_Power.h"
#include "Pins.h"
#include "configuration.h"

#ifdef POW_PD
#include "USBPD.h"
#endif

void power_check() {
#ifdef POW_PD
  if (USBPowerDelivery::negotiationComplete()) {
    return;
  }
#endif
}

bool getIsPoweredByDCIN() { return false; }

uint8_t getTipResistanceX10() { return TIP_RESISTANCE; }

bool isTipShorted() { return false; }

TemperatureType_t getCustomTipMaxInC() { return MAX_TEMP_C; }

uint16_t getTipThermalMass() { return TIP_THERMAL_MASS; }
uint16_t getTipInertia() { return TIP_THERMAL_MASS; }
