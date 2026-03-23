#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
#include <stdint.h>
extern uint32_t SystemCoreClock;
#endif

/* V11 tick-width enum. FreeRTOS.h defines the same values, but IronOS can
 * include portmacro.h before that header. Defining them here keeps the
 * portmacro #if well-defined; a later identical #define is not a warning.
 */
#ifndef TICK_TYPE_WIDTH_16_BITS
#define TICK_TYPE_WIDTH_16_BITS 0
#endif
#ifndef TICK_TYPE_WIDTH_32_BITS
#define TICK_TYPE_WIDTH_32_BITS 1
#endif
#ifndef TICK_TYPE_WIDTH_64_BITS
#define TICK_TYPE_WIDTH_64_BITS 2
#endif

/* Numeric 1 == TICK_TYPE_WIDTH_32_BITS. Do not use the token alone: if this
 * header is included before FreeRTOS.h, an undefined token becomes 0 in #if.
 */
#define configTICK_TYPE_WIDTH_IN_BITS 1

/* RP2040 MPU is not used for task isolation. */
#define configENABLE_MPU 0
#ifndef configS_C_B_FLASH
#define configS_C_B_FLASH 0
#endif
#ifndef configS_C_B_SRAM
#define configS_C_B_SRAM 0
#endif

// RP2040 is Cortex-M0+ with 264KB SRAM and 2MB flash.
// Tasks and queues are static (osThreadStaticDef). There is no heap_4.c,
// so configTOTAL_HEAP_SIZE is intentionally absent.
#define configUSE_PREEMPTION                    1
#define configSUPPORT_STATIC_ALLOCATION         1
#define configSUPPORT_DYNAMIC_ALLOCATION        0
#define configUSE_IDLE_HOOK                     1
#define configUSE_TICK_HOOK                     0
#define configCPU_CLOCK_HZ                      (SystemCoreClock)
#define configTICK_RATE_HZ                      ((TickType_t)1000)
#define configMAX_PRIORITIES                    (7)
#define configMINIMAL_STACK_SIZE                ((uint16_t)256)
#define configMAX_TASK_NAME_LEN                 (32)
#define configUSE_MUTEXES                       1
#define configQUEUE_REGISTRY_SIZE               0
#define configUSE_TIMERS                        0
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0 // Not available on CM0
#define configCHECK_FOR_STACK_OVERFLOW          2

#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           0
#define INCLUDE_vTaskDelete                 0
#define INCLUDE_vTaskCleanUpResources       0
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_xTaskGetSchedulerState      1
#define INCLUDE_uxTaskGetStackHighWaterMark 1

// Cortex-M0+ has 2 priority bits
#ifdef __NVIC_PRIO_BITS
#define configPRIO_BITS __NVIC_PRIO_BITS
#else
#define configPRIO_BITS 2
#endif

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY      3
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 1

#define configKERNEL_INTERRUPT_PRIORITY      (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))
#define configMAX_SYSCALL_INTERRUPT_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#define configASSERT(x)       \
  if ((x) == 0) {             \
    taskDISABLE_INTERRUPTS(); \
    for (;;)                  \
      ;                       \
  }

// Map FreeRTOS port handlers to CMSIS standard names
#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#if configUSE_TIMERS
#define configTIMER_TASK_PRIORITY    2
#define configTIMER_QUEUE_LENGTH     8
#define configTIMER_TASK_STACK_DEPTH (512 / 4)
#endif

#ifdef __cplusplus
}
#endif

#endif /* FREERTOS_CONFIG_H */
