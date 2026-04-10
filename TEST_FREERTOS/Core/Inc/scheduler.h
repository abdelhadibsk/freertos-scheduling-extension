#ifndef SCHEDULER_H
#define SCHEDULER_H

/* =========================================================
 * scheduler.h
 * RT Scheduler Extension — All-in-One Header
 *
 * Rules:
 *   - TCB_t stays private inside tasks.c (never exposed)
 *   - All functions that touch TCB directly live in tasks.c
 *   - rt_scheduler.c and rt_policies.c only call helpers
 *     via extern — they never see TCB_t at all
 * ========================================================= */

#include "FreeRTOS.h"
#include "task.h"

/* =========================================================
 * SECTION 1 — CONFIGURATION
 * ========================================================= */

// #define configUSE_SCHEDULER    1

#define configUSE_RM              0
#define configUSE_DM              0
#define configUSE_FIFO            0
#define configUSE_EDF             1

#define configMAX_RT_TASKS        10

#if ( configUSE_RM + configUSE_DM + configUSE_FIFO + configUSE_EDF ) > 1
    #error "rt_scheduler.h: Only one RT policy can be enabled at a time."
#endif

/* =========================================================
 * SECTION 2 — RT_Params_t
 *
 * Defined here so tasks.c can embed it in TCB_t.
 * rt_scheduler.c and rt_policies.c never access it directly.
 * ========================================================= */
typedef struct
{
    TickType_t  period;
    TickType_t  deadline;
    TickType_t  execution_time;

    TickType_t  next_release;
    TickType_t  absolute_deadline;

    UBaseType_t release_order;

} RT_Params_t;

/* =========================================================
 * SECTION 3 — PUBLIC API
 * (implemented in scheduler.c)
 * ========================================================= */

BaseType_t xRTTaskCreate(
    TaskFunction_t         pxTaskCode,
    const char * const     pcName,
    configSTACK_DEPTH_TYPE uxStackDepth,
    void *                 pvParameters,
    TickType_t             period,
    TickType_t             deadline,
    TickType_t             execution_time,
    TaskHandle_t *         pxCreatedTask
);

void vApplicationRTTaskRegister(
    TaskHandle_t xTask,
    TickType_t   period,
    TickType_t   deadline,
    TickType_t   execution_time
);

void vApplicationSchedulerTickHook( void );
void vApplicationSchedulerUpdatePriorities( void );

/* =========================================================
 * SECTION 4 — HELPER API
 *
 * These functions are IMPLEMENTED IN tasks.c because they
 * need direct access to TCB_t.
 *
 * They are declared extern here so scheduler.c and
 * policies.c can call them without ever seeing TCB_t.
 * ========================================================= */

/* Registry — implemented in tasks.c */
extern UBaseType_t  uxRTGetTaskCount( void );
extern TaskHandle_t xRTGetTaskByIndex( UBaseType_t index );

/* RT parameter getters — implemented in tasks.c */
extern TickType_t   xRTGetTaskPeriod( TaskHandle_t xTask );
extern TickType_t   xRTGetTaskDeadline( TaskHandle_t xTask );
extern TickType_t   xRTGetTaskExecutionTime( TaskHandle_t xTask );
extern TickType_t   xRTGetTaskAbsoluteDeadline( TaskHandle_t xTask );
extern UBaseType_t  uxRTGetTaskReleaseOrder( TaskHandle_t xTask );

/* ISR-safe helpers for TickHook — implemented in tasks.c */
extern TickType_t   xRTGetNextRelease( TaskHandle_t xTask );
extern void         vRTJobRelease( TaskHandle_t xTask, TickType_t xNow );

/* State check — implemented in tasks.c */
extern BaseType_t   xRTIsTaskReady( TaskHandle_t xTask );

/* Safe priority setter — implemented in tasks.c */
extern void         vRTSetTaskPriority( TaskHandle_t xTask, UBaseType_t uxPriority );
extern void 		prvSetTaskPriorityNoYield( TaskHandle_t xTask, UBaseType_t uxNewPriority );

#endif /* SCHEDULER_H */
