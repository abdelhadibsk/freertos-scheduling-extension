#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
  #include <stdint.h>
  #include <string.h>
  #include <stdio.h>
#include "usart.h"
  extern uint32_t SystemCoreClock;
  void xPortSysTickHandler(void);
#endif

#ifndef CMSIS_device_header
#define CMSIS_device_header "stm32l4xx.h"
#endif

/* FPU — mandatory on Cortex-M4F */
#define configENABLE_FPU                         1
#define configENABLE_MPU                         0

/* Core */
#define configUSE_PREEMPTION                     1
#define configSUPPORT_STATIC_ALLOCATION          1
#define configSUPPORT_DYNAMIC_ALLOCATION         1
#define configCPU_CLOCK_HZ                       ( SystemCoreClock )
#define configTICK_RATE_HZ                       ( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES                     ( 8 )
#define configMINIMAL_STACK_SIZE                 ( ( uint16_t ) 2*256 )
#define configTOTAL_HEAP_SIZE                    ( 20 * 1024 )
#define configMAX_TASK_NAME_LEN                  ( 16 )
#define configUSE_16_BIT_TICKS                   0
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  0

/* Your RT scheduler */
#define configUSE_SCHEDULER                      1
#define configUSE_PERIODIC_TASKS                 1
//#define configMAX_RT_TASKS                       8

/* Hooks — all off while debugging */
#define configUSE_IDLE_HOOK                      0
#define configUSE_TICK_HOOK                      0

/* Trace */
#define configUSE_TRACE_FACILITY                 1
#define configCHECK_FOR_STACK_OVERFLOW           2
#define traceTASK_SWITCHED_IN()                  MyTaskSwitchedIn()
#define traceTASK_SWITCHED_OUT()                 MyTaskSwitchedOut()

/* Timers — OFF */
#define configUSE_TIMERS                         0

/* Mutexes / semaphores — OFF */
#define configUSE_MUTEXES                        0
#define configUSE_RECURSIVE_MUTEXES              0
#define configUSE_COUNTING_SEMAPHORES            0
#define configQUEUE_REGISTRY_SIZE                8
#define configUSE_CO_ROUTINES                    0
#define configMAX_CO_ROUTINE_PRIORITIES          ( 2 )

/* Newlib — OFF */
#define configUSE_NEWLIB_REENTRANT               0

/* NVIC priority — Cortex-M4 */
#ifdef __NVIC_PRIO_BITS
  #define configPRIO_BITS                        __NVIC_PRIO_BITS
#else
  #define configPRIO_BITS                        4
#endif

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY         15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    5
#define configKERNEL_INTERRUPT_PRIORITY    \
        ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << ( 8 - configPRIO_BITS ) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
        ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << ( 8 - configPRIO_BITS ) )

/* Assert */
#define configASSERT( x ) \
    if( ( x ) == 0 ) { \
        extern UART_HandleTypeDef huart2; \
        char _a[128]; \
        snprintf(_a, sizeof(_a), "ASSERT %d\r\n", __LINE__); \
        HAL_UART_Transmit(&huart2, (uint8_t*)_a, strlen(_a), 1000); \
        taskDISABLE_INTERRUPTS(); \
        for( ;; ); \
    }

/* Port handler mappings */
#define vPortSVCHandler                          SVC_Handler
#define xPortPendSVHandler                       PendSV_Handler
#define USE_CUSTOM_SYSTICK_HANDLER_IMPLEMENTATION 1

/* INCLUDE flags */
#define INCLUDE_vTaskPrioritySet                 1
#define INCLUDE_uxTaskPriorityGet                1
#define INCLUDE_vTaskDelete                      1
#define INCLUDE_vTaskSuspend                     1
#define INCLUDE_vTaskDelayUntil                  1
#define INCLUDE_vTaskDelay                       1
#define INCLUDE_xTaskGetSchedulerState           1
#define INCLUDE_xTimerPendFunctionCall           0
#define INCLUDE_pcTaskGetTaskName                1
#define INCLUDE_uxTaskGetStackHighWaterMark      1
#define INCLUDE_xTaskGetCurrentTaskHandle        1
#define INCLUDE_eTaskGetState                    1
#define INCLUDE_xTaskGetHandle                   1
#define INCLUDE_xTaskResumeFromISR               1
#define INCLUDE_xSemaphoreGetMutexHolder         0

#define configMESSAGE_BUFFER_LENGTH_TYPE         size_t

#endif /* FREERTOS_CONFIG_H */
