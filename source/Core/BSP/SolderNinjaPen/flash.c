// Flash storage for Solder Ninja Pen settings
//
// The RP2040 uses XIP (execute-in-place) flash starting at 0x10000000.
// We use the last 4KB sector for settings storage. The Pico SDK provides
// flash_range_erase() and flash_range_program() which operate on offsets
// from flash base, and require interrupts to be disabled during operation.

#include "BSP.h"
#include "BSP_Flash.h"
#include "configuration.h"

#include "hardware/flash.h"
#include "hardware/sync.h"

// Settings are at the last 4KB of 2MB flash
#define SETTINGS_FLASH_OFFSET (2 * 1024 * 1024 - FLASH_SECTOR_SIZE)

void flash_save_buffer(const uint8_t *buffer, const uint16_t length) {
  // Must disable interrupts for flash operations on RP2040
  uint32_t ints = save_and_disable_interrupts();

  // Erase the sector (4KB)
  flash_range_erase(SETTINGS_FLASH_OFFSET, FLASH_SECTOR_SIZE);

  // Program the data (must be aligned to FLASH_PAGE_SIZE = 256 bytes)
  // Round up length to page boundary
  uint16_t writeLen = (length + FLASH_PAGE_SIZE - 1) & ~(FLASH_PAGE_SIZE - 1);
  flash_range_program(SETTINGS_FLASH_OFFSET, buffer, writeLen);

  restore_interrupts(ints);
}

void flash_read_buffer(uint8_t *buffer, const uint16_t length) {
  const uint8_t *flashAddr = (const uint8_t *)(XIP_BASE + SETTINGS_FLASH_OFFSET);
  for (uint16_t i = 0; i < length; i++) {
    buffer[i] = flashAddr[i];
  }
}
