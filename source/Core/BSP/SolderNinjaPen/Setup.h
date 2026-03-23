#ifndef BSP_SNP_SETUP_H_
#define BSP_SNP_SETUP_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initialize all RP2040 hardware peripherals
void Setup_HAL(void);

// I2C0 at 400 kHz on SNP_I2C0_SDA/SCL. Safe to call again after a bus unstick.
void setup_i2c(void);

// SystemCoreClock for FreeRTOS — defined by Pico SDK CMSIS (system_RP2040.c)
extern uint32_t SystemCoreClock;

#ifdef __cplusplus
}
#endif

#endif /* BSP_SNP_SETUP_H_ */
