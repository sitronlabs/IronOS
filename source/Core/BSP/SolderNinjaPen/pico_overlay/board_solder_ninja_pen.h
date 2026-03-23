// Solder Ninja Pen board profile for the Pico SDK.
// Assembler-safe: this header is included from pico/config.h.
// Pin assignments used by IronOS live in Pins.h; these defaults only stop
// the stock "pico" board from claiming GP25 (our MAX31855 chip-select) as an LED.

#ifndef _BOARDS_SOLDER_NINJA_PEN_H
#define _BOARDS_SOLDER_NINJA_PEN_H

pico_board_cmake_set(PICO_PLATFORM, rp2040)

#define SOLDER_NINJA_PEN

// No default UART or LED. Pins 0/1/25 are not a Pico header on this product.

#ifndef PICO_DEFAULT_I2C
#define PICO_DEFAULT_I2C 0
#endif
#ifndef PICO_DEFAULT_I2C_SDA_PIN
#define PICO_DEFAULT_I2C_SDA_PIN 4
#endif
#ifndef PICO_DEFAULT_I2C_SCL_PIN
#define PICO_DEFAULT_I2C_SCL_PIN 5
#endif

#ifndef PICO_DEFAULT_SPI
#define PICO_DEFAULT_SPI 1
#endif
#ifndef PICO_DEFAULT_SPI_SCK_PIN
#define PICO_DEFAULT_SPI_SCK_PIN 26
#endif
#ifndef PICO_DEFAULT_SPI_TX_PIN
#define PICO_DEFAULT_SPI_TX_PIN 27
#endif
#ifndef PICO_DEFAULT_SPI_RX_PIN
#define PICO_DEFAULT_SPI_RX_PIN 24
#endif

#define PICO_BOOT_STAGE2_CHOOSE_W25Q080 1

#ifndef PICO_FLASH_SPI_CLKDIV
#define PICO_FLASH_SPI_CLKDIV 2
#endif

pico_board_cmake_set_default(PICO_FLASH_SIZE_BYTES, (2 * 1024 * 1024))
#ifndef PICO_FLASH_SIZE_BYTES
#define PICO_FLASH_SIZE_BYTES (2 * 1024 * 1024)
#endif

#endif
