// Interrupt handling for Solder Ninja Pen (RP2040)
//
// The RP2040 uses a simpler interrupt model than STM32.
// FUSB302 is polled (no dedicated IRQ pin on the SNP).
// Accelerometer interrupts can be added here if needed.

#include "IRQ.h"
#include "Pins.h"
#include "configuration.h"

// POWTaskHandle is owned by main.cpp (osThreadId); IRQ handlers may notify it.
// TODO: wire FUSB302 / accel GPIO IRQs to xTaskNotifyFromISR(POWTaskHandle, ...)
