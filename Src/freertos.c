/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "gpio.h"
#include "tim.h"
#include "usart.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId WheelDriverTaskHandle;
osThreadId LED_Blink_TaskHandle;
osThreadId Commuicate_TaskHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void StartWheelDriverTask(void const * argument);
void StartLED_Blink_Task(void const * argument);
void StartCommuicate_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
/*
Upload:

Download:
0xAA 0x55 1 2 3 4 0xFE
1:Left 	Direction 0x00-> +,0x01->-
2:Left 	Speed %
3:Right Direction	0x00-> +,0x01->-
4:Right Speed %
*/
/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of WheelDriverTask */
  osThreadDef(WheelDriverTask, StartWheelDriverTask, osPriorityNormal, 0, 128);
  WheelDriverTaskHandle = osThreadCreate(osThread(WheelDriverTask), NULL);

  /* definition and creation of LED_Blink_Task */
  osThreadDef(LED_Blink_Task, StartLED_Blink_Task, osPriorityIdle, 0, 128);
  LED_Blink_TaskHandle = osThreadCreate(osThread(LED_Blink_Task), NULL);

  /* definition and creation of Commuicate_Task */
  osThreadDef(Commuicate_Task, StartCommuicate_Task, osPriorityNormal, 0, 128);
  Commuicate_TaskHandle = osThreadCreate(osThread(Commuicate_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
		RUN_LED_Blink();
    osDelay(500);
  }
  /* USER CODE END StartDefaultTask */
}

/* StartWheelDriverTask function */
void StartWheelDriverTask(void const * argument)
{
  /* USER CODE BEGIN StartWheelDriverTask */
  /* Infinite loop */
  for(;;)
  {
		if(RxFinishFlag)
		{
			if(UART3RxBuff[2])
			{
				HAL_GPIO_WritePin(GPIOE, LeftDirection_Pin, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOE, LeftDirection_Pin, GPIO_PIN_RESET);
			}
			if(UART3RxBuff[4])
			{
				HAL_GPIO_WritePin(GPIOE, RightDirection_Pin, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOE, RightDirection_Pin, GPIO_PIN_RESET);
			}
			
			SetLeftWheelSpeed(UART3RxBuff[3]);
	
			SetRightWheelSpeed(UART3RxBuff[5]);
		}
		else
		{
			SetLeftWheelSpeed(0);
			SetRightWheelSpeed(0);
		}
    osDelay(10);
  }
  /* USER CODE END StartWheelDriverTask */
}

/* StartLED_Blink_Task function */
void StartLED_Blink_Task(void const * argument)
{
  /* USER CODE BEGIN StartLED_Blink_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(10);
  }
  /* USER CODE END StartLED_Blink_Task */
}

/* StartCommuicate_Task function */
void StartCommuicate_Task(void const * argument)
{
  /* USER CODE BEGIN StartCommuicate_Task */
  /* Infinite loop */
  for(;;)
  {
		//printf("usart is ok\r\n");
    osDelay(1);
  }
  /* USER CODE END StartCommuicate_Task */
}

/* USER CODE BEGIN Application */
   

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
