/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define MAX_BRIGHTNESS 255

uint32_t aun_ir_buffer[100]; //IR LED sensor data

int32_t n_ir_buffer_length;    //data length

uint32_t aun_red_buffer[100];    //Red LED sensor data

int32_t n_sp02; //SPO2 value

int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid 指示SP02计算是否有效的指示器

int32_t n_heart_rate;   //heart rate value 心率�?
int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid显示心率计算是否有效的指示器

uint8_t uch_dummy;
/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_GPIO_Pin GPIO_PIN_13
#define LED_GPIO_GPIO_Port GPIOC
#define TFT_LED_TIM2_Pin GPIO_PIN_1
#define TFT_LED_TIM2_GPIO_Port GPIOA
#define BLU_USART2_RX_Pin GPIO_PIN_2
#define BLU_USART2_RX_GPIO_Port GPIOA
#define BLU_USART2_TX_Pin GPIO_PIN_3
#define BLU_USART2_TX_GPIO_Port GPIOA
#define BLU_GPIO_SW_Pin GPIO_PIN_4
#define BLU_GPIO_SW_GPIO_Port GPIOA
#define TFT_SPI1_SCK_Pin GPIO_PIN_5
#define TFT_SPI1_SCK_GPIO_Port GPIOA
#define TFT_GPIO_RESET_Pin GPIO_PIN_6
#define TFT_GPIO_RESET_GPIO_Port GPIOA
#define TFT_SPI1_MOSI_SDA_Pin GPIO_PIN_7
#define TFT_SPI1_MOSI_SDA_GPIO_Port GPIOA
#define TFT_GPIO_DC_Pin GPIO_PIN_0
#define TFT_GPIO_DC_GPIO_Port GPIOB
#define TFT_GPIO__CS_Pin GPIO_PIN_1
#define TFT_GPIO__CS_GPIO_Port GPIOB
#define HR_I2C2_SCL_Pin GPIO_PIN_10
#define HR_I2C2_SCL_GPIO_Port GPIOB
#define key2_GPIO_Pin GPIO_PIN_12
#define key2_GPIO_GPIO_Port GPIOB
#define key1_GPIO_Pin GPIO_PIN_13
#define key1_GPIO_GPIO_Port GPIOB
#define key3_GPIO_Pin GPIO_PIN_15
#define key3_GPIO_GPIO_Port GPIOB
#define BLU_GPIO_HW_Pin GPIO_PIN_8
#define BLU_GPIO_HW_GPIO_Port GPIOA
#define GPS_USART1_RX_Pin GPIO_PIN_9
#define GPS_USART1_RX_GPIO_Port GPIOA
#define GPS_USART1_TX_Pin GPIO_PIN_10
#define GPS_USART1_TX_GPIO_Port GPIOA
#define BLU_GPIO_CMD_Pin GPIO_PIN_15
#define BLU_GPIO_CMD_GPIO_Port GPIOA
#define HR_I2C2_SDA_Pin GPIO_PIN_3
#define HR_I2C2_SDA_GPIO_Port GPIOB
#define FD_GPIO_INT_Pin GPIO_PIN_5
#define FD_GPIO_INT_GPIO_Port GPIOB
#define FD_I2C1_SCL_Pin GPIO_PIN_6
#define FD_I2C1_SCL_GPIO_Port GPIOB
#define HR_GPIO_INT_Pin GPIO_PIN_8
#define HR_GPIO_INT_GPIO_Port GPIOB
#define FD_I2C1_SDA_Pin GPIO_PIN_9
#define FD_I2C1_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
