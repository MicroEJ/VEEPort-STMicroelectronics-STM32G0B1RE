/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * Copyright 2023 MicroEJ Corp. This file has been modified by MicroEJ Corp.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  *
  * Copyright 2023 MicroEJ Corp. All rights reserved.
  * Use of this source code is governed by a BSD-style license that can be found with this software.
  *
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "main.h"
#include "gpio.h"
#include "microej_main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os2.h"
#ifdef VALIDATION_BUILD
    #include "t_core_main.h"
#else
    #include "sni.h"
#endif
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MICROJVM_STACK_SIZE                (4 * 1024)

#ifdef VALIDATION_BUILD
    /* Validation task constants */
    #define VALIDATION_TASK_PRIORITY           ( 8 ) 
    #define VALIDATION_TASK_STACK_SIZE         (MICROJVM_STACK_SIZE / 4)
    #define VALIDATION_TASK_NAME               "BoardValidation"

#else
    /* MicroJvm task constants */
    #define JAVA_TASK_PRIORITY                 ( 8 ) 
    #define JAVA_TASK_STACK_SIZE               (MICROJVM_STACK_SIZE / 4)
    #define JAVA_TASK_NAME                     "MicroJvm"
#endif
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#ifdef VALIDATION_BUILD
  static const char *tx_validation_task_name = VALIDATION_TASK_NAME; // cppcheck-suppress [misra-c2012-8.9]: declared as global to ease task customization.
  uint8_t tx_stack_pool_validation[ VALIDATION_TASK_STACK_SIZE ];
#else
  static const char *tx_microej_task_name = JAVA_TASK_NAME; // cppcheck-suppress [misra-c2012-8.9]: declared as global to ease task customization.
  uint8_t tx_stack_pool_microej[ JAVA_TASK_STACK_SIZE ];
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#ifdef VALIDATION_BUILD

static void txValidationTaskFunction(void * pvParameters)
{ 
	(void)(pvParameters);

	T_CORE_main();
}

#else
static void txJavaTaskFunction(void * pvParameters)
{
	(void)(pvParameters);

    microej_main(0, NULL);
}

#endif
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
 // __enable_irq();
  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */
  // Init OS 
  osStatus_t status = osKernelInitialize();
  if ( status != osOK ) 
  {
    MICROEJ_MAIN_TRACE(" ThreadX initialization with CMSIS OS failed : 0x%x !\n",status);
  }
  
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  MICROEJ_MAIN_TRACE("Hardware init started...\n");

  /* Display MCU ref */
  MICROEJ_MAIN_TRACE( "MCU ID %x rev %x \r\n", HAL_GetDEVID(), HAL_GetREVID());
    
  /* Display frequency */
  MICROEJ_MAIN_TRACE( "Sys: %i / HCLK: %i / PCLK1: %i \r\n", HAL_RCC_GetSysClockFreq (),HAL_RCC_GetHCLKFreq(), HAL_RCC_GetPCLK1Freq());

  // Create tasks
  #ifdef VALIDATION_BUILD
  const osThreadAttr_t validation_task_attr = {
    .name       = tx_validation_task_name,
    .stack_mem  = tx_stack_pool_validation,
    .stack_size = VALIDATION_TASK_STACK_SIZE,
    .priority   = (osPriority_t) VALIDATION_TASK_PRIORITY
  };

  osThreadId_t tx_validation_task_id = osThreadNew( txValidationTaskFunction, NULL, &validation_task_attr );
  if (NULL == tx_validation_task_id)
  {
    MICROEJ_MAIN_TRACE( "Error in validation task creation!\n" );
  }
#else
  const osThreadAttr_t microej_task_attr = {
    .name       = tx_microej_task_name,
    .stack_mem  = tx_stack_pool_microej,
    .stack_size = JAVA_TASK_STACK_SIZE,
    .priority   = (osPriority_t) JAVA_TASK_PRIORITY
  };

  osThreadId_t tx_microej_task_id = osThreadNew( txJavaTaskFunction, NULL, &microej_task_attr );
  if (NULL == tx_microej_task_id)
  {
    MICROEJ_MAIN_TRACE( "Error in microej task creation!\n" );
  }

#endif
  
  // Start OS
  osKernelStart();
  /* USER CODE END 2 */


  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
	(void)(file);
	(void)(line);
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: MICROEJ_MAIN_TRACE("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
