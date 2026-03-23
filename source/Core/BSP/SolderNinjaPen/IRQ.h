#ifndef BSP_SNP_IRQ_H_
#define BSP_SNP_IRQ_H_

#include "configuration.h"

#ifdef __cplusplus
extern "C" {
#endif

// FreeRTOS task handle for the power task (used by IRQ callbacks)
#include "FreeRTOS.h"
#include "task.h"

// TODO: declare IRQ helpers here once FUSB302 / LIS2DH12 interrupts are wired

#ifdef __cplusplus
}
#endif

#endif /* BSP_SNP_IRQ_H_ */
