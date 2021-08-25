/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CIFTEWAIT 75
#define BUZWAIT 200
#define TRIPLEWAIT 100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */
uint8_t veri;
int mod;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void funcCifte(void);
void funcBuz(void);
void funcTriple(void);
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
	MX_DMA_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */
	HAL_UART_Receive_DMA(&huart2, &veri, 1);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		switch (mod) {
		case 1:
			HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, 0);
			funcCifte();
			break;
		case 2:
			//buzzer on
			HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, 1);
			funcBuz();
			break;
		case 3:
			HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, 0);
			funcTriple();
			break;
		default:
			HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, 0);
			break;
		}
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

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
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
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel6_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
	/* DMA1_Channel7_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, buzzer_Pin|D0_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, greenLed_Pin|yellowLed_Pin|blueLed_Pin|internalLed_Pin
			|RS_Pin|RW_Pin|D2_Pin|D1_Pin
			|D7_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, redLed_Pin|D3_Pin|D6_Pin|D4_Pin
			|D5_Pin|E_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : internalButton_Pin */
	GPIO_InitStruct.Pin = internalButton_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(internalButton_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : buzzer_Pin */
	GPIO_InitStruct.Pin = buzzer_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(buzzer_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : greenLed_Pin yellowLed_Pin blueLed_Pin internalLed_Pin */
	GPIO_InitStruct.Pin = greenLed_Pin|yellowLed_Pin|blueLed_Pin|internalLed_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : RS_Pin RW_Pin D2_Pin D1_Pin
                           D7_Pin */
	GPIO_InitStruct.Pin = RS_Pin|RW_Pin|D2_Pin|D1_Pin
			|D7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : redLed_Pin */
	GPIO_InitStruct.Pin = redLed_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(redLed_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : D3_Pin D6_Pin D4_Pin D5_Pin
                           E_Pin */
	GPIO_InitStruct.Pin = D3_Pin|D6_Pin|D4_Pin|D5_Pin
			|E_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : D0_Pin */
	GPIO_InitStruct.Pin = D0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(D0_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void funcCifte(void)
{
	//blue led
	HAL_GPIO_WritePin(blueLed_GPIO_Port, blueLed_Pin,1);
	HAL_Delay(CIFTEWAIT);
	HAL_GPIO_WritePin(blueLed_GPIO_Port, blueLed_Pin,0);
	HAL_Delay(CIFTEWAIT);
	HAL_GPIO_WritePin(blueLed_GPIO_Port, blueLed_Pin,1);
	HAL_Delay(CIFTEWAIT);
	HAL_GPIO_WritePin(blueLed_GPIO_Port, blueLed_Pin,0);
	HAL_Delay(CIFTEWAIT);
	//red led
	HAL_GPIO_WritePin(redLed_GPIO_Port, redLed_Pin,1);
	HAL_Delay(CIFTEWAIT);
	HAL_GPIO_WritePin(redLed_GPIO_Port, redLed_Pin,0);
	HAL_Delay(CIFTEWAIT);
	HAL_GPIO_WritePin(redLed_GPIO_Port, redLed_Pin,1);
	HAL_Delay(CIFTEWAIT);
	HAL_GPIO_WritePin(redLed_GPIO_Port, redLed_Pin,0);
	HAL_Delay(CIFTEWAIT);
}
void funcBuz(void)
{
	//blue led
	HAL_GPIO_WritePin(blueLed_GPIO_Port, blueLed_Pin,1);
	HAL_GPIO_WritePin(redLed_GPIO_Port, redLed_Pin,0);
	HAL_Delay(BUZWAIT);
	//red led
	HAL_GPIO_WritePin(blueLed_GPIO_Port, blueLed_Pin,0);
	HAL_GPIO_WritePin(redLed_GPIO_Port, redLed_Pin,1);
	HAL_Delay(BUZWAIT);
	//clear
	HAL_GPIO_WritePin(blueLed_GPIO_Port, blueLed_Pin,0);
	HAL_GPIO_WritePin(redLed_GPIO_Port, redLed_Pin,0);
}
void funcTriple(void)
{
	HAL_GPIO_WritePin(yellowLed_GPIO_Port, yellowLed_Pin, 1);
	if (HAL_GPIO_ReadPin(internalButton_GPIO_Port, internalButton_Pin)==GPIO_PIN_SET)
	{
		HAL_GPIO_WritePin(greenLed_GPIO_Port, greenLed_Pin, 1);
		HAL_Delay(TRIPLEWAIT);
		HAL_GPIO_WritePin(greenLed_GPIO_Port, greenLed_Pin, 0);
		HAL_Delay(TRIPLEWAIT);
	}
	HAL_GPIO_WritePin(yellowLed_GPIO_Port, yellowLed_Pin, 0);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
