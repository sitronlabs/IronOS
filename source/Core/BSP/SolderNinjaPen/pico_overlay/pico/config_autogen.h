// Make-side stand-in for the CMake-generated pico/config_autogen.h.
// Kept outside Vendor/pico-sdk so the vendored tree stays unpatched.
// Assembler-safe: preprocessor only (pulled in via pico/config.h).

#ifndef _PICO_CONFIG_AUTOGEN_H
#define _PICO_CONFIG_AUTOGEN_H

#ifndef PICO_RP2040
#define PICO_RP2040 1
#endif
#ifndef PICO_RP2350
#define PICO_RP2350 0
#endif
#ifndef PICO_ON_DEVICE
#define PICO_ON_DEVICE 1
#endif

#include "board_solder_ninja_pen.h"

#define LIB_PICO_PLATFORM_PANIC       1
#define LIB_CMSIS_CORE                1
#define LIB_PICO_SYNC                 1
#define LIB_PICO_SYNC_CRITICAL_SECTION 1
#define LIB_PICO_SYNC_MUTEX           1
#define LIB_PICO_SYNC_SEM             1
#define LIB_PICO_TIME                 1
#define LIB_PICO_UTIL                 1
#define LIB_PICO_UNIQUE_ID            1
#define LIB_PICO_BOOTROM              1
#define LIB_PICO_FLASH                1
#define LIB_PICO_ATOMIC               1
#define LIB_PICO_BINARY_INFO          1
#define LIB_PICO_RUNTIME              1
#define LIB_PICO_RUNTIME_INIT         1

#define LIB_HARDWARE_GPIO     1
#define LIB_HARDWARE_SPI      1
#define LIB_HARDWARE_I2C      1
#define LIB_HARDWARE_ADC      1
#define LIB_HARDWARE_FLASH    1
#define LIB_HARDWARE_WATCHDOG 1
#define LIB_HARDWARE_CLOCKS   1
#define LIB_HARDWARE_SYNC     1
#define LIB_HARDWARE_TIMER    1
#define LIB_HARDWARE_IRQ      1
#define LIB_HARDWARE_PLL      1
#define LIB_HARDWARE_VREG     1
#define LIB_HARDWARE_XOSC     1
#define LIB_HARDWARE_RESETS   1
#define LIB_HARDWARE_TICKS    1
#define LIB_HARDWARE_EXCEPTION 1
#define LIB_HARDWARE_UART     1
#define LIB_HARDWARE_CLAIM    1

#include "cmsis/rename_exceptions.h"

#endif
