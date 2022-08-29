/*
 * gyro.c
 *
 *  Created on: Oct 10, 2021
 *      Author: Kaitlyn Roberts
 */

#include "stdio.h"
#include "float.h"
#include "display.h"
#include "main.h"
#include "gyro.h"
#include "control.h"
#include "stdbool.h"
#include <stdlib.h>
#include <math.h>

struct Data * fifo_gyro_G;
struct Data * fifo_gyro_A;
static int32_t gyro_bias[3];
static int32_t accel_bias[3];
static int size;
Node* current_gyro_G;
Node* current_gyro_A;

static bool currentFALL;

// data sheet: https://www.invensense.com/download-pdf/icm-20789-datasheet/
// referenced example code: https://github.com/drcpattison/ICM-20948
// FD_I2C1_SDA_GPIO_Port
// I2C address 0x34
void gyro_init(I2C_HandleTypeDef * hi2c1){
	fifo_gyro_G = malloc(sizeof(struct Data));
	fifo_gyro_A = malloc(sizeof(struct Data));

	fifo_gyro_G->max = INT16_MIN;
	fifo_gyro_A->max = INT16_MIN;
	fifo_gyro_G->min = INT16_MAX;
	fifo_gyro_A->min = INT16_MAX;
	fifo_gyro_G->head = (struct Node*)malloc(sizeof(struct Node));
	fifo_gyro_A->head = (struct Node*)malloc(sizeof(struct Node));
	fifo_gyro_G->head->next = NULL;
	fifo_gyro_A->head->next = NULL;
	fifo_gyro_G->head->prev = NULL;
	fifo_gyro_A->head->prev = NULL;
	fifo_gyro_G->tail = fifo_gyro_G->head;
	fifo_gyro_A->tail = fifo_gyro_A->head;
	gyro_bias[0] = 0;
	gyro_bias[1] = 0;
	gyro_bias[2] = 0;
	accel_bias[0] = 0;
	accel_bias[1] = 0;
	accel_bias[2] = 0;
	size = 0;

		uint8_t * pdata = malloc(sizeof(uint8_t) * 2);

		//reset device, disable internal temperature sensor, auto-select internal clock
		pdata[0] = REG_PWR_MGMT1;
		pdata[1] = 0b10001001;
		HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);
		HAL_Delay(500);

		pdata[0] = REG_USER_CTRL;
		pdata[1] = 0b11011101; //reset I2C, DMP, FIFO, data registers
		HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);
		HAL_Delay(500);

		//set interrupt pin as open drain, active low and disable FSYNC interrupt
		pdata[0] = REG_INT_PIN_CFG;
		pdata[1] = 0b11010000;
		HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);

		//set FIFO sampling rate to 1000/(1 + 6 ) = 166 Hz
		//which is slower than Accel and Gyro bandwidths and faster than human movement
		//(see gyro and accel config below)
		pdata[0] = REG_SMPLRT_DIV;
		pdata[1] = 0x06;
		HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);

		// Enable Accelerometer (all axes) and Gyroscope (all axes) by writing zero to PWR_MGMT_2
		pdata[0] = REG_PWR_MGMT2;
		pdata[1] = 0x0;
		HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);

		// configure Accelerometer to read +/- 16 G, and set Accelerometer 3dB bandwidth to 218.1 Hz
		//human movement is below 120 Hz according to https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.217.992&rep=rep1&type=pdf
		//therefore 218.1 Hz is definitely enough for our purposes
		pdata[0] = REG_ACCEL_CONFIG1;
		pdata[1] = 0b00011000;
		HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);

		//set FIFO to delete old data on overflow, configure Gyroscope to read +/- 250 Degrees/sec,
		//and set Gyroscope 3dB bandwidth to 176 Hz
		//human movement is below 120 Hz according to https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.217.992&rep=rep1&type=pdf
		//therefore 176 Hz is definitely enough for our purposes
		pdata[0] = REG_GYRO_CONFIG;
		pdata[1] = 0x00;
		HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);

		pdata[0] = REG_CONFIG_USR;
		pdata[1] = 0x01;
		HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);

		//set FIFO to read accel x,y,z and gyro x,y,z registers (in that order)
		pdata[0] = REG_FIFO_EN;
		pdata[1] = 0b01111000;
		HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);
		HAL_Delay(500);
}

void gyro_read(I2C_HandleTypeDef * hi2c1){
	uint8_t * pdata = malloc(sizeof(uint8_t) * 2);
	uint8_t fifo_gyro_sample[12];
	pdata[0] = REG_FIFO_RW;
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 1, 1000);
	HAL_I2C_Master_Receive((I2C_HandleTypeDef *) hi2c1, GYRO_READ_ADDR, fifo_gyro_sample, 12, 1000);

		fifo_gyro_A->head->dataX = (fifo_gyro_sample[0]<<8) + fifo_gyro_sample[1];
		fifo_gyro_A->head->dataY = (fifo_gyro_sample[2]<<8) + fifo_gyro_sample[3];
		fifo_gyro_A->head->dataZ = (fifo_gyro_sample[4]<<8) + fifo_gyro_sample[5];
		fifo_gyro_G->head->dataX = (fifo_gyro_sample[6]<<8) + fifo_gyro_sample[7];
		fifo_gyro_G->head->dataY = (fifo_gyro_sample[8]<<8) + fifo_gyro_sample[9];
		fifo_gyro_G->head->dataZ = (fifo_gyro_sample[10]<<8) + fifo_gyro_sample[11];

		fifo_gyro_A->head->dataMag = sqrt((fifo_gyro_A->head->dataX * fifo_gyro_A->head->dataX)+(fifo_gyro_A->head->dataY * fifo_gyro_A->head->dataY)+(fifo_gyro_A->head->dataZ * fifo_gyro_A->head->dataZ));
		fifo_gyro_G->head->dataMag = sqrt((fifo_gyro_G->head->dataX * fifo_gyro_G->head->dataX)+(fifo_gyro_G->head->dataY * fifo_gyro_G->head->dataY)+(fifo_gyro_G->head->dataZ * fifo_gyro_G->head->dataZ));
		if (fifo_gyro_A->head->dataMag >= fifo_gyro_A->max) {fifo_gyro_A->max = fifo_gyro_A->head->dataMag;}
		if (fifo_gyro_A->head->dataMag <= fifo_gyro_A->min) {fifo_gyro_A->min = fifo_gyro_A->head->dataMag;}
		if (fifo_gyro_G->head->dataMag >= fifo_gyro_G->max) {fifo_gyro_G->max = fifo_gyro_G->head->dataMag;}
		if (fifo_gyro_G->head->dataMag <= fifo_gyro_G->min) {fifo_gyro_G->min = fifo_gyro_G->head->dataMag;}


		gyro_bias[0] += fifo_gyro_G->head->dataX;
		gyro_bias[1] += fifo_gyro_G->head->dataY;
		gyro_bias[2] += fifo_gyro_G->head->dataZ;
		accel_bias[0] += fifo_gyro_A->head->dataX;
		accel_bias[1] += fifo_gyro_A->head->dataY;
		accel_bias[2] += fifo_gyro_A->head->dataZ;

		Node* temp = fifo_gyro_G->head;
		fifo_gyro_G->head = (struct Node*)malloc(sizeof(struct Node));
		temp->next = fifo_gyro_G->head;
		fifo_gyro_G->head->prev = temp;
		fifo_gyro_G->head->next = NULL;

		temp = fifo_gyro_A->head;
		fifo_gyro_A->head = (struct Node*)malloc(sizeof(struct Node));
		temp->next = fifo_gyro_A->head;
		fifo_gyro_A->head->prev = temp;
		fifo_gyro_A->head->next = NULL;


		size++;
		if(size == MAX_DATA_PT){//reached maximum buffer size for calculating, remove tail datapoint
			gyro_bias[0] -= fifo_gyro_G->tail->dataX;
			gyro_bias[1] -= fifo_gyro_G->tail->dataY;
			gyro_bias[2] -= fifo_gyro_G->tail->dataZ;
			accel_bias[0] -= fifo_gyro_A->tail->dataX;
			accel_bias[1] -= fifo_gyro_A->tail->dataY;
			accel_bias[2] -= fifo_gyro_A->tail->dataZ;

			if(fifo_gyro_G->tail->dataMag == fifo_gyro_G->max){fifo_gyro_G->max = INT16_MIN;}
			if(fifo_gyro_A->tail->dataMag == fifo_gyro_A->max){fifo_gyro_A->max = INT16_MIN;}
			if(fifo_gyro_G->tail->dataMag == fifo_gyro_G->min){fifo_gyro_G->min = INT16_MAX;}
			if(fifo_gyro_A->tail->dataMag == fifo_gyro_A->min){fifo_gyro_A->min = INT16_MAX;}

			Node* temp = fifo_gyro_G->tail->next;
			free(fifo_gyro_G->tail);
			temp->prev = NULL;
			fifo_gyro_G->tail = temp;

			temp = fifo_gyro_A->tail->next;
			free(fifo_gyro_A->tail);
			temp->prev = NULL;
			fifo_gyro_A->tail = temp;
			size--;
		}
}

void getGyroData(I2C_HandleTypeDef * hi2c1){
	if(fifo_gyro_G->head == fifo_gyro_G->tail) //collecting data for the first time
	{
		for(int i = 0; i<MAX_DATA_PT; i++){
			gyro_read((I2C_HandleTypeDef *) hi2c1);
		}

		//need to remove last head node, which is uninitialized
		Node* temp = fifo_gyro_G->head->prev;
		free(fifo_gyro_G->head);
		temp->next = NULL;
		fifo_gyro_G->head = temp;

		temp = fifo_gyro_A->head->prev;
		free(fifo_gyro_A->head);
		temp->next = NULL;
		fifo_gyro_A->head = temp;
	}
	else{gyro_read((I2C_HandleTypeDef *) hi2c1);}

	calibrate((I2C_HandleTypeDef *) hi2c1);

	//if maxs or mins have not been reset, and local max minus min is over a certain threshold, say we have detected a fall
	//reference for thresholds: https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.217.992&rep=rep1&type=pdf
	if((fifo_gyro_G->max != INT16_MIN) & (fifo_gyro_A->max != INT16_MIN) & (fifo_gyro_G->min != INT16_MAX) & (fifo_gyro_A->min != INT16_MAX)){
		if((fifo_gyro_G->max - fifo_gyro_G->min > GTHRESHOLD)
				& (fifo_gyro_A->max - fifo_gyro_A->min > ATHRESHOLD)){
			currentFALL = true;
		}
		else{currentFALL = false;}
	}
}

bool getFALL(void){
	return currentFALL;
}

// Function which accumulates gyro and accelerometer data after device
// initialization. It calculates the average of the at-rest readings and then
// loads the resulting offsets into accelerometer and gyro bias registers.
void calibrate(I2C_HandleTypeDef * hi2c1)
{
	uint8_t * pdata = malloc(sizeof(uint8_t) * 2);
	int32_t gyro_bias_avg[3];
	int32_t accel_bias_avg[3];
  uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
  uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec scale factor
  uint16_t  accelsensitivity = 16384; // = 16384 LSB/g scale factor

  // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
  accel_bias_avg[0] =  accel_bias[0]/(int32_t) size;
  accel_bias_avg[1] = accel_bias[1]/(int32_t) size;
  accel_bias_avg[2] = accel_bias[2]/(int32_t) size;
  gyro_bias_avg[0]  = gyro_bias[0]/(int32_t) size;
  gyro_bias_avg[1]  = gyro_bias[1]/(int32_t) size;
  gyro_bias_avg[2]  = gyro_bias[2]/(int32_t) size;

  // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
  if (accel_bias_avg[2] > 0L) {accel_bias_avg[2] -= (int32_t) accelsensitivity;}
  else {accel_bias_avg[2] += (int32_t) accelsensitivity;}

  // Construct the gyro biases for push to the hardware gyro bias registers,
  // which are reset to zero upon device startup.
  // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input
  // format.
  data[0] = (-gyro_bias_avg[0]/4  >> 8) & 0xFF;
  // Biases are additive, so change sign on calculated average gyro biases
  data[1] = (-gyro_bias_avg[0]/4)       & 0xFF;
  data[2] = (-gyro_bias_avg[1]/4  >> 8) & 0xFF;
  data[3] = (-gyro_bias_avg[1]/4)       & 0xFF;
  data[4] = (-gyro_bias_avg[2]/4  >> 8) & 0xFF;
  data[5] = (-gyro_bias_avg[2]/4)       & 0xFF;

  // Push gyro biases to hardware registers
	pdata[0] = REG_XG_OFFS_H;
	pdata[1] = data[0];
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);
	pdata[0] = REG_XG_OFFS_L;
	pdata[1] = data[1];
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);
	pdata[0] = REG_YG_OFFS_H;
	pdata[1] = data[2];
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);
	pdata[0] = REG_YG_OFFS_L;
	pdata[1] = data[3];
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);
	pdata[0] = REG_ZG_OFFS_H;
	pdata[1] = data[4];
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);
	pdata[0] = REG_ZG_OFFS_L;
	pdata[1] = data[5];
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);


	gyro_bias[0] = (float) gyro_bias[0]/(float) gyrosensitivity;
	gyro_bias[1] = (float) gyro_bias[1]/(float) gyrosensitivity;
	gyro_bias[2] = (float) gyro_bias[2]/(float) gyrosensitivity;

  // Construct the accelerometer biases for push to the hardware accelerometer
  // bias registers. These registers contain factory trim values which must be
  // added to the calculated accelerometer biases; on boot up these registers
  // will hold non-zero values. In addition, bit 0 of the lower byte must be
  // preserved since it is used for temperature compensation calculations.
  // Accelerometer bias registers expect bias input as 2048 LSB per g, so that
  // the accelerometer biases calculated above must be divided by 8.

  // A place to hold the factory accelerometer trim biases
  int32_t accel_bias_reg[3] = {0, 0, 0};
  // Read factory accelerometer trim values
	pdata[0] = REG_XA_OFFS_H;
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 1, 1000);
	HAL_I2C_Master_Receive((I2C_HandleTypeDef *) hi2c1, GYRO_READ_ADDR, &data[0], 2, 1000);
  accel_bias_reg[0] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
	pdata[0] = REG_YA_OFFS_H;
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 1, 1000);
	HAL_I2C_Master_Receive((I2C_HandleTypeDef *) hi2c1, GYRO_READ_ADDR, &data[0], 2, 1000);
  accel_bias_reg[1] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
	pdata[0] = REG_ZA_OFFS_H;
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 1, 1000);
	HAL_I2C_Master_Receive((I2C_HandleTypeDef *) hi2c1, GYRO_READ_ADDR, &data[0], 2, 1000);
  accel_bias_reg[2] = (int32_t) (((int16_t)data[0] << 8) | data[1]);

  // Define mask for temperature compensation bit 0 of lower byte of
  // accelerometer bias registers
  uint32_t mask = 1uL;
  // Define array to hold mask bit for each accelerometer bias axis
  uint8_t mask_bit[3] = {0, 0, 0};

  for (int ii = 0; ii < 3; ii++)
  {
    // If temperature compensation bit is set, record that fact in mask_bit
    if ((accel_bias_reg[ii] & mask)){mask_bit[ii] = 0x01;}
  }

  // Construct total accelerometer bias, including calculated average
  // accelerometer bias from above
  // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g
  // (16 g full scale)
  accel_bias_reg[0] -= (accel_bias_avg[0]/8);
  accel_bias_reg[1] -= (accel_bias_avg[1]/8);
  accel_bias_reg[2] -= (accel_bias_avg[2]/8);

  data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
  data[1] = (accel_bias_reg[0])      & 0xFF;
  // preserve temperature compensation bit when writing back to accelerometer
  // bias registers
  data[1] = data[1] | mask_bit[0];
  data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
  data[3] = (accel_bias_reg[1])      & 0xFF;
  // Preserve temperature compensation bit when writing back to accelerometer
  // bias registers
  data[3] = data[3] | mask_bit[1];
  data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
  data[5] = (accel_bias_reg[2])      & 0xFF;
  // Preserve temperature compensation bit when writing back to accelerometer
  // bias registers
  data[5] = data[5] | mask_bit[2];

  // Push accelerometer biases to hardware registers
	pdata[0] = REG_XA_OFFS_H;
	pdata[1] = data[0];
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);
	pdata[0] = REG_XA_OFFS_L;
	pdata[1] = data[1];
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);
	pdata[0] = REG_YA_OFFS_H;
	pdata[1] = data[2];
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);
	pdata[0] = REG_YA_OFFS_L;
	pdata[1] = data[3];
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);
	pdata[0] = REG_ZA_OFFS_H;
	pdata[1] = data[4];
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);
	pdata[0] = REG_ZA_OFFS_L;
	pdata[1] = data[5];
	HAL_I2C_Master_Transmit((I2C_HandleTypeDef *) hi2c1, GYRO_WRITE_ADDR, pdata, 2, 1000);
}
