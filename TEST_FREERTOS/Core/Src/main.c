/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
//#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "scheduler.h"
#include <stdio.h>
#include "DBG.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
TaskHandle_t tA, tB, tC;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
void periodic_task( void *pvParameters );
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );
void MyTaskSwitchedIn( void );
void MyTaskSwitchedOut( void );
void vApplicationTickHook( void );
void show_specific_task_states(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Start scheduler */

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  /* Create RT periodic tasks.
       * No need to pass parameters to the task function —
       * everything is stored inside the TCB via xRTTaskCreate. */
  xRTTaskCreate( periodic_task, "A", 1024, NULL,
                 pdMS_TO_TICKS( 700 ),   /* period   */
                 pdMS_TO_TICKS( 700 ),   /* deadline */
                 pdMS_TO_TICKS( 300  ),   /* execution time */
                 &tA );

  xRTTaskCreate( periodic_task, "B", 1024, NULL,
                 pdMS_TO_TICKS( 2000 ),
                 pdMS_TO_TICKS( 2000 ),
                 pdMS_TO_TICKS( 800 ),
                 &tB );


/*  xRTTaskCreate( periodic_task, "C", 1024, NULL,
                     pdMS_TO_TICKS( 1000 ),
                     pdMS_TO_TICKS( 1000 ),
                     pdMS_TO_TICKS( 600  ),
                     &tC );
*/

      /* Print registered task info using helper API */
  DBG_PRINT( "Tasks created and registered:\n\r" );
  for( UBaseType_t i = 0; i < uxRTGetTaskCount(); i++ )
  {
	  TaskHandle_t h = xRTGetTaskByIndex( i );
	  DBG_PRINT( "  Task %s: period=%lu  deadline=%lu  execution_time=%lu\n\r",
      pcTaskGetName( h ),
      ( unsigned long ) xRTGetTaskPeriod( h ),
      ( unsigned long ) xRTGetTaskDeadline( h ),
      ( unsigned long ) xRTGetTaskExecutionTime( h ) );
  }

      /* Active policy shown at startup */
	#if   ( configUSE_RM   == 1 )
  DBG_PRINT( "Policy: Rate Monotonic (RM)\n\r" );
	#elif ( configUSE_DM   == 1 )
  DBG_PRINT( "Policy: Deadline Monotonic (DM)\n\r" );
	#elif ( configUSE_FIFO == 1 )
  DBG_PRINT( "Policy: FIFO\n\r" );
	#elif ( configUSE_EDF  == 1 )
  DBG_PRINT( "Policy: Earliest Deadline First (EDF)\n\r" );
  #endif

  //fflush( stdout );
  HAL_SuspendTick();

  DBG_PRINT( "Starting scheduler\n\r" );
  vTaskStartScheduler();
  DBG_PRINT("5 - ERROR: scheduler returned!\r\n");


  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/* Required by FreeRTOS static allocation */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t  **ppxIdleTaskStackBuffer,
                                    uint32_t      *pulIdleTaskStackSize )
{
    static StaticTask_t xIdleTaskTCB;
    static StackType_t  uxIdleTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__((aligned(8)));

    *ppxIdleTaskTCBBuffer   = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
}/* =========================================================
 * PERIODIC TASK
 *
 * No parameters passed — everything is read directly from
 * the TCB via the helper API using the current task handle.
 *
 * Pattern:
 *   1. Do work (busy wait for execution time duration)
 *   2. Print job info
 *   3. Suspend — TickHook will release next job
 * ========================================================= */
void periodic_task( void *pvParameters )
{
    ( void ) pvParameters;

    for( ;; )
    {
        TaskHandle_t self    = xTaskGetCurrentTaskHandle();
        TickType_t   execution_time = xRTGetTaskExecutionTime( self );
        TickType_t   elapsed = 0;
        TickType_t   slice_start;


        /* Exécute par tranches — accumule seulement le temps CPU réel.
         * Si préemptée entre deux tranches, le temps perdu n'est pas compté. */
        while( elapsed < execution_time )
        {
            slice_start = xTaskGetTickCount();

            /* Tranche de travail — 1 tick à la fois */
            while( xTaskGetTickCount() < (slice_start + 1) )
            {
                /* busy work pendant exactement 1 tick */
            }

            elapsed++;   /* 1 tick CPU consommé */
        }


        vTaskSuspend( NULL );
    }
}

/* =========================================================
 * Hook — Stack Overflow
 * ========================================================= */
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    ( void ) xTask;
    //DBG_PRINT( "STACK OVERFLOW: %s\n", pcTaskName );
    taskDISABLE_INTERRUPTS();
    for( ;; );
}

/* =========================================================
 * Hooks — Task Switch Trace
 * Called by FreeRTOS trace macros (traceTASK_SWITCHED_IN /
 * traceTASK_SWITCHED_OUT) — define these macros in
 * FreeRTOSConfig.h to enable tracing:
 *
 *   #define traceTASK_SWITCHED_IN()  MyTaskSwitchedIn()
 *   #define traceTASK_SWITCHED_OUT() MyTaskSwitchedOut()
 * ========================================================= */
void MyTaskSwitchedIn( void )
{
    TaskHandle_t h = xTaskGetCurrentTaskHandle();
    //we want to SET ON the LED here to visualize the scheduling,
    // if task A is running, SET GPIO PB5
    // if task B is running, SET GPIO PB6

        if (pcTaskGetName(h)[0] == 'A') {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
        } else if (pcTaskGetName(h)[0] == 'B') {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
        } else if (pcTaskGetName(h)[0] == 'C') {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
        }

    DBG_PRINT( "[IN ] %s at %lu\n\r",
          pcTaskGetName( h ),
          ( unsigned long ) xTaskGetTickCount() );

}

void MyTaskSwitchedOut( void )
{
    TaskHandle_t h = xTaskGetCurrentTaskHandle();
    //we want to SET OFF the LED here to visualize the scheduling,
    // if task A is switched out, SET OFF GPIO PB5
    // if task B is switched out, SET OFF GPIO PB6
        if (pcTaskGetName(h)[0] == 'A') {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
        } else if (pcTaskGetName(h)[0] == 'B') {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
        } else if (pcTaskGetName(h)[0] == 'C') {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
        }

    DBG_PRINT( "[OUT] %s at %lu\n\r",
           pcTaskGetName( h ),
          ( unsigned long ) xTaskGetTickCount() );
}

/* =========================================================
 * Hook — Tick
 * vApplicationRTTickHook() is called automatically inside
 * xTaskIncrementTick() via the tasks.c modification.
 * This hook is for application-level periodic logging only.
 * ========================================================= */
/*
void vApplicationTickHook( void )
{
    static TickType_t tickCount = 0;
    tickCount++;

    if( ( tickCount % 100 ) == 0 )
    {
        DBG_PRINT( "[TICK] %lu\n", ( unsigned long ) tickCount );
    }
}
*/

void show_specific_task_states(void)
{
    TaskHandle_t tasks[] = {tA, tB, tC};
    const char* names[] = {"A", "B", "C"};

    DBG_PRINT("\n--- Specific Task States ---\n");
    for(int i = 0; i < 3; i++)
    {
        if(tasks[i] != NULL)
        {
            eTaskState state = eTaskGetState(tasks[i]);
            const char *state_str;

            switch(state)
            {
                case eRunning:   state_str = "Running"; break;
                case eReady:     state_str = "Ready"; break;
                case eBlocked:   state_str = "Blocked"; break;
                case eSuspended: state_str = "Suspended"; break;
                case eDeleted:   state_str = "Deleted"; break;
                default:         state_str = "Unknown"; break;
            }

            DBG_PRINT("Task %s: %s\n", names[i], state_str);
        }
    }
    DBG_PRINT("----------------------------\n");
}

// And update the tick hook to use the simpler version:
void vApplicationTickHook(void)
{
    static TickType_t tickCount = 0;
    tickCount++;

    if ((tickCount % 10) == 0)
    {
    	DBG_PRINT("[TICK] %lu ticks elapsed\n", (unsigned long)tickCount);

        if ((tickCount % 50) == 0)
        {
            show_specific_task_states();
        }
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
