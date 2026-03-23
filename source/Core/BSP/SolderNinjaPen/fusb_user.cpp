// FUSB302 I2C transport for Solder Ninja Pen.
// The shared PD stack passes 8-bit addresses (FUSB302B_ADDR is 0x22 << 1).

#include "BSP.h"
#include "BSP_PD.h"
#include "I2C_Wrapper.hpp"
#include "Pins.h"
#include "configuration.h"

#include "hardware/gpio.h"

#ifdef POW_PD

bool fusb_write_buf(const uint8_t deviceAddr, const uint8_t registerAdd,
                    const uint8_t size, uint8_t *buf) {
  return FRToSI2C::Mem_Write(deviceAddr, registerAdd, buf, size);
}

bool fusb_read_buf(const uint8_t deviceAddr, const uint8_t registerAdd,
                   const uint8_t size, uint8_t *buf) {
  return FRToSI2C::Mem_Read(deviceAddr, registerAdd, buf, size);
}

void setupFUSBIRQ() {
  // The Solder Ninja Pen doesn't use a dedicated FUSB302 IRQ line.
  // The PD stack polls instead. If an IRQ pin were wired, we'd
  // configure it as a GPIO falling-edge interrupt here.
}

bool getFUS302IRQLow() {
  // No dedicated IRQ pin; return true to indicate the PD stack
  // should always check for messages (polled mode).
  return true;
}

#endif /* POW_PD */
