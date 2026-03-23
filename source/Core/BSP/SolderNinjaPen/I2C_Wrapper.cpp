/*
 * Hardware I2C0 (GP4/GP5, 400 kHz) behind the shared FRToSI2C API.
 * IronOS passes 8-bit addresses (7-bit << 1); the Pico peripheral wants 7-bit.
 * Transfers are blocking. A stuck bus is recovered by unstick_I2C().
 */

#include "BSP.h"
#include "I2C_Wrapper.hpp"
#include "Pins.h"
#include "Setup.h"

#include "hardware/i2c.h"

SemaphoreHandle_t FRToSI2C::I2CSemaphore = nullptr;
StaticSemaphore_t FRToSI2C::xSemaphoreBuffer;

static uint8_t addr7(uint16_t addr8) { return (uint8_t)(addr8 >> 1); }

static bool write_raw(uint8_t address, const uint8_t *data, uint16_t len, bool noStop) {
  if (len == 0) {
    return true;
  }
  int wrote = i2c_write_blocking(i2c0, address, data, len, noStop);
  return wrote == (int)len;
}

static bool read_raw(uint8_t address, uint8_t *data, uint16_t len, bool noStop) {
  if (len == 0) {
    return true;
  }
  int got = i2c_read_blocking(i2c0, address, data, len, noStop);
  return got == (int)len;
}

void FRToSI2C::CpltCallback() {
  // Blocking transfers; nothing to do from an IRQ.
}

bool FRToSI2C::I2C_RegisterWrite(uint8_t address, uint8_t reg, uint8_t data) { return Mem_Write(address, reg, &data, 1); }

uint8_t FRToSI2C::I2C_RegisterRead(uint8_t address, uint8_t reg) {
  uint8_t value = 0;
  Mem_Read(address, reg, &value, 1);
  return value;
}

bool FRToSI2C::Mem_Read(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size) {
  if (!lock()) {
    return false;
  }
  uint8_t reg    = (uint8_t)MemAddress;
  uint8_t target = addr7(DevAddress);
  // Repeated start: register write, then read, one transaction.
  bool ok = write_raw(target, &reg, 1, true) && read_raw(target, pData, Size, false);
  if (!ok) {
    I2C_Unstick();
  }
  unlock();
  return ok;
}

bool FRToSI2C::Mem_Write(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size) {
  if (!lock()) {
    return false;
  }
  uint8_t reg    = (uint8_t)MemAddress;
  uint8_t target = addr7(DevAddress);
  // Keep one transaction: register byte, then payload, no second address cycle.
  // nostop leaves restart_on_next set; clear it so the payload is not a Restart.
  bool ok = write_raw(target, &reg, 1, Size > 0);
  if (ok && Size > 0) {
    i2c0->restart_on_next = false;
    ok                    = write_raw(target, pData, Size, false);
  }
  if (!ok) {
    I2C_Unstick();
  }
  unlock();
  return ok;
}

bool FRToSI2C::Transmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size) {
  if (Size == 0) {
    return false;
  }
  return Mem_Write(DevAddress, pData[0], pData + 1, Size - 1);
}

void FRToSI2C::Receive(uint16_t DevAddress, uint8_t *pData, uint16_t Size) {
  if (!lock()) {
    return;
  }
  if (!read_raw(addr7(DevAddress), pData, Size, false)) {
    I2C_Unstick();
  }
  unlock();
}

void FRToSI2C::TransmitReceive(uint16_t DevAddress, uint8_t *pData_tx, uint16_t Size_tx, uint8_t *pData_rx, uint16_t Size_rx) {
  if (!lock()) {
    return;
  }
  uint8_t target = addr7(DevAddress);
  bool    ok     = write_raw(target, pData_tx, Size_tx, true) && read_raw(target, pData_rx, Size_rx, false);
  if (!ok) {
    I2C_Unstick();
  }
  unlock();
}

bool FRToSI2C::probe(uint16_t DevAddress) {
  uint8_t temp = 0;
  return Mem_Read(DevAddress, 0x00, &temp, 1);
}

void FRToSI2C::I2C_Unstick() { unstick_I2C(); }

bool FRToSI2C::lock() {
  if (I2CSemaphore == nullptr) {
    return false;
  }
  return xSemaphoreTake(I2CSemaphore, TICKS_SECOND) == pdTRUE;
}

void FRToSI2C::unlock() { xSemaphoreGive(I2CSemaphore); }

bool FRToSI2C::writeRegistersBulk(const uint8_t address, const I2C_REG *registers, const uint8_t registersLength) {
  for (uint8_t index = 0; index < registersLength; index++) {
    if (!I2C_RegisterWrite(address, registers[index].reg, registers[index].val)) {
      return false;
    }
    if (registers[index].pause_ms) {
      delay_ms(registers[index].pause_ms);
    }
  }
  return true;
}

bool FRToSI2C::wakePart(uint16_t DevAddress) {
  // Address the device. The Pico helper will not emit a byte-less address cycle.
  if (!lock()) {
    return false;
  }
  uint8_t dummy = 0;
  bool    ok    = write_raw(addr7(DevAddress), &dummy, 1, false);
  if (!ok) {
    I2C_Unstick();
  }
  unlock();
  return ok;
}
