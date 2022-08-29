/*
 * control.h
 *
 *  Created on: Sep 17, 2021
 *      Author: Tingzhang Li
 */


#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#include "lcd.h"

#define KEY1        HAL_GPIO_ReadPin(key1_GPIO_GPIO_Port,key1_GPIO_Pin)  //pb13 key1
#define KEY2        HAL_GPIO_ReadPin(key2_GPIO_GPIO_Port,key2_GPIO_Pin)  //pb12 key2-change to this one for final
#define KEY3        HAL_GPIO_ReadPin(key3_GPIO_GPIO_Port,key3_GPIO_Pin) //pb15 key3

#define KEY1_PRES 	1
#define KEY2_PRES	2
#define KEY3_PRES	3

#define SETTING	    1
#define TEMP	    2
#define BLOOD	    3
#define HEART	    4
#define LAT	    	5
#define LON	    	6
#define FALLD	    7
#define BLUEM	    8
#define POWER	    9

extern float temperature ; //temperature
extern u8 temperature1[5] ; //temperature string

extern float temperatureUS ; //temperature in
extern u8 temperatureUS1[6] ; //temperature in F string

extern unsigned char heart_rate ; //heart rate
extern u8 heart_rate1[5] ; //heart rate string

extern unsigned char blood_oxygen ; //blood oxygen
extern u8 blood_oxygen1[3] ; //blood oxygen string

extern float latitude ; //latitude
extern u8 latitude1[6] ; //latitude string

extern float longitude ; //longitude
extern u8 longitude1[6] ; //longitude string

extern unsigned char fall ; //fall

extern unsigned char blueOn; //if bluetooth on 1-on 0-off
extern unsigned char unit; //1 for US unit 0 for international unite
//extern unsigned char GPSon; //if gps on 1-on 0-off

extern	unsigned char screen_off ; //屏幕是不是已经熄灭了
extern	unsigned char Screen_off_time;
extern	unsigned char switch_value;
extern  unsigned char selection; //use to determine which item is selected

//void get_time(void);
u8 Get_Key_value(u8 mode);
void Display_On_Off(unsigned char switch_value);

//Only for my test
void Get_Temperature_data(void);
void HeartInt(void);
void Get_Heart_data(void);
float TemperF(float temperature);
//void Get_Blood_data(void);
void Get_GyroAccel_data(void);

void Get_GPS_data(void);
void BlueStatus(void);
void SendBluetooth(void);

#endif /* INC_CONTROL_H_ */
