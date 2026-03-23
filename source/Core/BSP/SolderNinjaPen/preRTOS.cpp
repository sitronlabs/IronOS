// Pre-RTOS initialization for Solder Ninja Pen (RP2040)

#include "BSP.h"
#include "I2C_Wrapper.hpp"
#include "Pins.h"
#include "Setup.h"

void preRToSInit() {
  Setup_HAL();
  BSPInit();
  FRToSI2C::FRToSInit();
}
