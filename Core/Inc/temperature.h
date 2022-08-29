/*
 * temperature.h
 *
 *  Created on: Oct 10, 2021
 *      Author: Kaitlyn Roberts
 */

#ifndef INC_TEMPERATURE_H_
#define INC_TEMPERATURE_H_

#define TEMPER_I2C_ADDR 0x40
#define TEMPER_WRITE_ADDR 0x80 //0x40 << 1
#define TEMPER_READ_ADDR 0x81

//commands
#define SHT4X_CMD_WRITE_USERCONFIG 0xE6
#define SHT4X_CMD_READ_USERCONFIG 0xE7
#define SHT4X_CMD_MEASURE_T 0xF3
#define SHT4X_CMD_MEASURE_RH 0xF5
#define SHT4X_SOFT_RESET 0xFE

void temper_init(I2C_HandleTypeDef * hi2c2);
void temper_read(I2C_HandleTypeDef * hi2c2);
void getTemperData(I2C_HandleTypeDef * hi2c2);
float getTemperC(void);
float getTemperF(void);

#endif /* INC_TEMPERATURE_H_ */
