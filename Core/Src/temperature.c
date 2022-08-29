/*
 * temperature.c
 *
 *  Created on: Oct 10, 2021
 *      Author: Kaitlyn Roberts
 */

#include "stdio.h"
#include "float.h"
#include "display.h"
#include "main.h"
#include "temperature.h"
#include "control.h"

static float currentTemperC;
static float currentTemperF;
static float currentHumid;

// data sheet: http://www.farnell.com/datasheets/1780639.pdf?CMP=AFC-CJ-DE-9026295&source=CJ&cjevent=ebebbd2b2c3a11ec81f803550a82b832&cjdata=MXxOfDB8WXww
// referenced example code: https://github.com/Sensirion/embedded-sht/tree/master/sht4x
//HR_I2C2_SDA_GPIO_Port
//I2C address 0x44
void temper_init(I2C_HandleTypeDef * hi2c2){
	uint8_t pdata[2];
	pdata[0] = SHT4X_SOFT_RESET;
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c2, TEMPER_WRITE_ADDR, &pdata[0], 1, 1000);
	HAL_Delay(100);
}

void temper_read(I2C_HandleTypeDef * hi2c2){
	uint8_t fifo_temper_sample[2];
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c2, TEMPER_WRITE_ADDR, (uint8_t *) SHT4X_CMD_MEASURE_T, 1, 1000);
	HAL_Delay(100);
	HAL_I2C_Master_Receive((I2C_HandleTypeDef *) hi2c2, TEMPER_READ_ADDR, fifo_temper_sample, 2, 1000);
	uint16_t temperData = fifo_temper_sample[0] * 256 + fifo_temper_sample[1];

	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c2, TEMPER_WRITE_ADDR, (uint8_t *) SHT4X_CMD_MEASURE_RH, 1, 1000);
	HAL_Delay(50);
	HAL_I2C_Master_Receive((I2C_HandleTypeDef *) hi2c2, TEMPER_READ_ADDR, fifo_temper_sample, 2, 1000);
	uint16_t relativeHumidData = fifo_temper_sample[0] * 256 + fifo_temper_sample[1];

	currentTemperC = -46.85 + 175.72 * temperData/65536;
	currentTemperF = -52.33 + 1581.48 * temperData/327680;
	currentHumid = -6 + 125 * relativeHumidData/65536;
	if (currentHumid > 100) {currentHumid = 100;} else if (currentHumid < 0) {currentHumid = 0;}
}

void getTemperData(I2C_HandleTypeDef * hi2c2){
	temper_read((I2C_HandleTypeDef *) hi2c2);
}

float getTemperC(void){
	return currentTemperC;
}
float getTemperF(void){
	return currentTemperF;
}
