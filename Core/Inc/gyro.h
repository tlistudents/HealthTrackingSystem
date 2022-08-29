/*
 * gyro.h
 *
 *  Created on: Oct 10, 2021
 *      Author: Kaitlyn Roberts
 */

#ifndef INC_GYRO_H_
#define INC_GYRO_H_

#include "stdbool.h"

#define GYRO_WRITE_ADDR 0x68 //0x34 << 1
#define GYRO_READ_ADDR 0x69
//register addresses
#define REG_XG_OFFS_H 0x13
#define REG_XG_OFFS_L 0x14
#define REG_YG_OFFS_H 0x15
#define REG_YG_OFFS_L 0x16
#define REG_ZG_OFFS_H 0x17
#define REG_ZG_OFFS_L 0x18
#define REG_SMPLRT_DIV 0x19
#define REG_CONFIG_USR 0x1A
#define REG_GYRO_CONFIG 0x1B
#define REG_ACCEL_CONFIG1 0x1C
#define REG_ACCEL_CONFIG2 0x1D
#define REG_FIFO_EN 0x23
#define REG_INT_PIN_CFG 0x37
#define REG_INT_STATUS 0x3A
#define REG_USER_CTRL 0x6A
#define REG_PWR_MGMT1 0x6B
#define REG_PWR_MGMT2 0x6C
#define REG_FIFO_RW 0x74
#define REG_XA_OFFS_H 0x77
#define REG_XA_OFFS_L 0x78
#define REG_YA_OFFS_H 0x7A
#define REG_YA_OFFS_L 0x7B
#define REG_ZA_OFFS_H 0x7D
#define REG_ZA_OFFS_L 0x7E

#define MAX_DATA_PT 50
#define GTHRESHOLD (int16_t) 3 // G
#define ATHRESHOLD (int16_t) 340 // degrees/sec

void gyro_init(I2C_HandleTypeDef *hi2c1);
void gyro_read(I2C_HandleTypeDef *hi2c1);
void getGyroData(I2C_HandleTypeDef *hi2c1);
void calibrate(I2C_HandleTypeDef *hi2c1);
bool getFALL(void);

struct Data * fifo_gyro_G;
struct Data * fifo_gyro_A;

typedef struct Node {
	int16_t dataX;
	int16_t dataY;
	int16_t dataZ;
	int16_t dataMag;
	struct Node* next;
	struct Node* prev;
} Node;

typedef struct Data {
	Node* head;
	Node* tail;
	int16_t max;
	int16_t min;
} Data;

#endif /* INC_GYRO_H_ */
