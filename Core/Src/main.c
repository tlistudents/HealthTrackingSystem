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
#include "control.h"
#include "display.h"
#include "lcd.h"
#include "stdio.h"
#include "heart.h"
#include "temperature.h"
#include "gyro.h"
#include "gps.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */

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
  unsigned char switch_value; //pushbottom value
  unsigned char selectedFlag = 100;
  unsigned char selection = 1;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  HAL_I2C_DeInit(I2C1); // de-initialize I2C1 to remove busy flag
  HAL_I2C_DeInit(I2C2);	// de-initialize I2C1 to remove busy flag
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */


  //LED light on to tell MCU start up
  HAL_GPIO_WritePin(LED_GPIO_GPIO_Port, LED_GPIO_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(HR_GPIO_INT_GPIO_Port, HR_GPIO_INT_Pin, GPIO_PIN_RESET); //connect HR_INT to active low
  //HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); //change to this for final
  //__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 300);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 500);
  //heart Sensor initialize registers
  //heart_init((I2C_HandleTypeDef *) &hi2c2);
  //temperature Sensor initialize registers
  //temper_init((I2C_HandleTypeDef *) &hi2c2);
  //gyro Sensor initialize registers
  //gyro_init((I2C_HandleTypeDef *) &hi2c1);
  //GPS init
  GPS_Init((UART_HandleTypeDef *) &huart1);

  screen_off = 0;
  unsigned char i = 0;
  unsigned char j = 0;
  unsigned char z = 0;
  unsigned char	i1=0;
  unsigned char	j1=0;
  unsigned char select1 = 1;
  unsigned char select2 = 1;
  unsigned char unit = 1;
  //char bluetoothTxBuffer[5] = "pur";
  //char bluetoothTxBuffernew[5];
  //uint8_t bluetoothRxBuffer[5] = {0};

  HeartInt();
  //latitude = -86.91;
  //longitude = 40.42;
  MENU_init(); //show start up menu
  //while(1)
  //{
  //	  HAL_UART_Transmit(&huart1, (uint8_t *) bluetoothTxBuffer, 3, 10);
  //	  HAL_UART_Receive(&huart1, (uint8_t *) bluetoothTxBuffernew, 3, 10);
  //}

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  	// Turn on display if off
		switch_value =  Get_Key_value(0);
		if(screen_off == 1)
		{
			Display_On_Off(switch_value);
			continue;
		}



		//Counter to aviod update too fast
		if((i == 0) && (j == 0) && (z==0))
		{
			Get_Heart_data();
			//Get_Temperature_data();
			//Get_GyroAccel_data();
			Get_GPS_data();
			BlueStatus();
			MENU_main();
		}
		i++;
		if(i>=255)
		{
			i=0;
			j++;
		}
		if(j>=255)
		{
			z++;
			j = 0;
		}
		if(z>=5)
		{
			i=0;
			j=0;
			z=0;
		}

		//Selecte page to display
		selectedFlag = MENU_SELECTION(switch_value);
		switch(selectedFlag)
		{
			case SETTING :
				MENU_SETTING();
			    break;
			case TEMP :
			    MENU_TEMP();
			    break;
			case BLOOD :
			    MENU_BLOOD();
			    break;
			case HEART :
			    MENU_HEART();
			    break;
			case LAT :
			    MENU_LAT();
			    break;
			case LON :
			    MENU_LON();
			    break;
			case FALLD :
			    MENU_FALL();
			    break;
			case BLUEM :

				MENU_BLUET();
			    break;
			case POWER : //poweroff
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0); //turn off display
				Refresh();
				screen_off = 1;
				selectedFlag = 100;
			    break;
			default:
				selectedFlag = 100;
				break;
		}

		// In non-main page, key 1 or key 3 return to main page, key 2 turn off display
		// Implement update variable here(you can write funtion like me to implement this or directly write here)
		if(selectedFlag == 1)
		{
			select1 = 1;
			select2 = 1;
			while (selectedFlag == 1)
			{
				switch_value =  Get_Key_value(0);
				if(switch_value == 1)
				{
					select1 ++;
				}
				else if (switch_value == 3)
				{
					select2 ++;
				}

				if(select1 >=3)
				{
					select1 = 1;
				}
				if(select2 >=3)
				{
					select2 = 1;
				}

				selectedFlag = SETTING_SELECTION(select1, select2, switch_value);

			}
		}

		i1=0;
		j1=0;
		while(selectedFlag != 100)
		{
			switch_value =  Get_Key_value(0);
			if((switch_value == 1) || (switch_value==3))
			{
				selectedFlag = 100;
				Refresh();
				break;
			}
			else if(switch_value == 2)
			{
				selectedFlag = 100;
				Display_On_Off(switch_value);
			}



			if((i1==0) && (j1==0))
			{
				// keep getting data from sensor even if not in the main page
				switch_value =  Get_Key_value(0);
				//Get_Temperature_data();
				//Get_GyroAccel_data();

				switch(selectedFlag)
				{
					//case SETTING :
						//LCD_ShowString (LCD_W/2,LCD_H/2,"SETTING", WHITE,	BLACK, 16,	1);
					//  break;
					case TEMP :
						//Get_Temperature_data();
						LCD_Fill(1,LCD_H/16*3,LCD_W/16*7,LCD_H/16*5,BLACK);
						LCD_Fill(1,LCD_H/16*5,LCD_W/16*7,LCD_H/16*7,BLACK);
						if((temperature < 28) || (temperature>38))
						{
							LCD_ShowFloatVar(LCD_W/16*1,LCD_H/16*3,temperature,temperature1, RED,BLACK,16); //show tempeature
							LCD_ShowFloatVar(LCD_W/16*1,LCD_H/16*5,temperatureUS,temperatureUS1, RED,BLACK,16); //show tempeature
						}
						else
						{
							LCD_ShowFloatVar(LCD_W/16*1,LCD_H/16*3,temperature,temperature1, WHITE,BLACK,16); //show tempeature
							LCD_ShowFloatVar(LCD_W/16*1,LCD_H/16*5,temperatureUS,temperatureUS1, WHITE,BLACK,16); //show tempeature
						}
					    break;
					case BLOOD :
						//Get_Heart_data();
						LCD_Fill(LCD_W/16*1,LCD_H/16*3,LCD_W/16*6,LCD_H/16*5,BLACK);
						if(blood_oxygen < 95)
						{
							LCD_ShowIntVar(LCD_W/16*1,LCD_H/16*3,blood_oxygen,blood_oxygen1, RED,BLACK,16); //show blood oxygen
						}
						else
						{
							LCD_ShowIntVar(LCD_W/16*1,LCD_H/16*3,blood_oxygen,blood_oxygen1, WHITE,BLACK,16); //show blood oxygen
						}
					    break;
					case HEART :
						//Get_Heart_data();
						LCD_Fill(LCD_W/16*1,LCD_H/16*3,LCD_W/16*6,LCD_H/16*5,BLACK);
						if((heart_rate < 60) || (heart_rate > 100))
						{
							LCD_ShowIntVar(LCD_W/16*1,LCD_H/16*3,heart_rate,heart_rate1, RED,BLACK,16); //show heart rate
						}
						else
						{
							LCD_ShowIntVar(LCD_W/16*1,LCD_H/16*3,heart_rate,heart_rate1, WHITE,BLACK,16); //show heart rate
						}
					    break;
					case LAT :
						if(latitude == 0 && longitude == 0)
						{
							LCD_ShowString (LCD_W/16*9,LCD_H/16*3,"No Conn.", RED,	BLACK, 12,	1);
							LCD_ShowString (LCD_W/16*9,LCD_H/16*5,"No Conn.", RED,	BLACK, 12,	1);
						}
						else
						{
							LCD_ShowFloatVar(LCD_W/16*9,LCD_H/16*3,latitude,latitude1, WHITE,BLACK,16);
							LCD_ShowFloatVar(LCD_W/16*9,LCD_H/16*5,longitude,longitude1, WHITE,BLACK,16);
						}
						//LCD_ShowString (LCD_W/2,LCD_H/2,"LAT", WHITE,	BLACK, 16,	1);
					    break;
					case LON :
						if(latitude == 0 && longitude == 0)
						{
							LCD_ShowString (LCD_W/16*9,LCD_H/16*3,"No Conn.", RED,	BLACK, 12,	1);
							LCD_ShowString (LCD_W/16*9,LCD_H/16*5,"No Conn.", RED,	BLACK, 12,	1);
						}
						else
						{
							LCD_ShowFloatVar(LCD_W/16*9,LCD_H/16*3,latitude,latitude1, WHITE,BLACK,16);
							LCD_ShowFloatVar(LCD_W/16*9,LCD_H/16*5,longitude,longitude1, WHITE,BLACK,16);
						}
						//LCD_ShowString (LCD_W/2,LCD_H/2,"LON", WHITE,	BLACK, 16,	1);
					    break;
					case FALLD :
						LCD_Fill(LCD_W/16*7,LCD_H/16*3,LCD_W/16*11,LCD_H/16*5,BLACK);
						//Get_GyroAccel_data();
						if(fall == 1)
						{
							LCD_ShowString (LCD_W/16*7,LCD_H/16*3,"YES", RED, BLACK, 16,	1);
						}
						else
						{
							LCD_ShowString (LCD_W/16*7,LCD_H/16*3,"NO", WHITE,BLACK, 16,	1);
						}
					    break;
					case BLUEM :
						LCD_Fill(LCD_W/16*7,LCD_H/16*3,LCD_W/16*11,LCD_H/16*5,BLACK);
						//BlueStatus();
						if(blueOn == 1)
						{
							LCD_ShowString (LCD_W/16*7,LCD_H/16*3,"ON", WHITE, BLACK, 16,	1);
						}
						else
						{
							LCD_ShowString (LCD_W/16*7,LCD_H/16*3,"OFF", WHITE,BLACK, 16,	1);
						}
					    break;
					default:
						selectedFlag = 100;
						break;
				}
				Get_GPS_data();
				BlueStatus();
				Get_Heart_data();
			}

			i1++;
			if(i1>=255)
			{
				i1=0;
				j1++;
			}
			if(j1>=255)
			{
				i1=0;
				j1=0;
			}
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */
  //__HAL_SPI_ENABLE(&hspi1);
  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 100-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 500-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  huart2.Init.BaudRate = 115200;
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_GPIO_Port, LED_GPIO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, BLU_GPIO_SW_Pin|TFT_GPIO_RESET_Pin|BLU_GPIO_HW_Pin|BLU_GPIO_CMD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, TFT_GPIO_DC_Pin|TFT_GPIO__CS_Pin|FD_GPIO_INT_Pin|HR_GPIO_INT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_GPIO_Pin */
  GPIO_InitStruct.Pin = LED_GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BLU_GPIO_SW_Pin TFT_GPIO_RESET_Pin BLU_GPIO_HW_Pin BLU_GPIO_CMD_Pin */
  GPIO_InitStruct.Pin = BLU_GPIO_SW_Pin|TFT_GPIO_RESET_Pin|BLU_GPIO_HW_Pin|BLU_GPIO_CMD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : TFT_GPIO_DC_Pin TFT_GPIO__CS_Pin FD_GPIO_INT_Pin HR_GPIO_INT_Pin */
  GPIO_InitStruct.Pin = TFT_GPIO_DC_Pin|TFT_GPIO__CS_Pin|FD_GPIO_INT_Pin|HR_GPIO_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : key2_GPIO_Pin key1_GPIO_Pin key3_GPIO_Pin */
  GPIO_InitStruct.Pin = key2_GPIO_Pin|key1_GPIO_Pin|key3_GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
