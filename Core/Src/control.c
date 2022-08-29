/*
 * control.c
 *
 *  Created on: Sep 17, 2021
 *      Author: Tingzhang Li
 */

#include "control.h"
#include "stdio.h"
#include "stdbool.h"
#include "main.h"
#include "display.h"
#include "heart.h"
#include "MAX30102.h"
#include "temperature.h"
#include "gyro.h"
#include "gps.h"

//RTC_DateTypeDef sDate; //全局变量，外部可以直接调用使用 ，存放日期数据
//RTC_TimeTypeDef sTime; //全局变量，外部可以直接调用使用 ，存放时间数据
//extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim2;
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern GPS_t GPS;  // To get the GPS data
//void get_time(void)
//{
//	/*get real time*/
//	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN); //get date
//	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN); //get time
//}


unsigned char Screen_off_time = 0;
unsigned char screen_off = 1;
//unsigned char switch_value = -1;
/*****************************************************************************
 * @name       :u8 KEY_Scan(void)
 * @date       :2021-09-09
 * @function   :Key processing,Response priority:KEY0>KEY1>WK_UP!!
 * @parameters :mode:0-Do not support continuous keystrokes
 					 1-Support for continuous keystrokes
 * @retvalue   :0-No buttons were pressed.
								1-The KEY0 button is pressed
								2-The KEY1 button is pressed
								3-The KEY2 button is pressed
								4-The WK_UP button is pressed
******************************************************************************/
u8 Get_Key_value(u8 mode)
{
    static u8 key_up=1;
    if(mode==1)key_up=1;
    if(key_up&&(KEY1==0||KEY2==0||KEY3==0))
    {
    	HAL_Delay(10);
        key_up=0;
        if(KEY1==0)       return KEY1_PRES;
        else if(KEY2==0)  return KEY2_PRES;
        else if(KEY3==0)  return KEY3_PRES;
    }else if(KEY1==1&&KEY2==1&&KEY3==1)key_up=1;
    return 0;
}


/*****************************************************************************
 * @name       :Display_On_Off(void)
 * @date       :2021/10/1
 * @function   :Turn on/off display
 * @parameters : n/a
 * @retvalue   :n/a
******************************************************************************/
void Display_On_Off(unsigned char switch_value)
{
	if((screen_off == 1) && (switch_value == 2))
	{
		//Refresh();
		//turn on display
		//__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 300); //change to this for final
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 500); //turn on display
		screen_off = 0;
	}
	else if((screen_off == 0) && switch_value == 2)
	{
		 //turn off display
		//__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0); //change to this for final
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0); //turn off display
		Refresh();
		screen_off = 1;
	}

}



// data sheet: https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/2_Humidity_Sensors/Datasheets/Sensirion_Humidity_Sensors_SHT4x_Datasheet.pdf
// referenced example code: https://github.com/Sensirion/embedded-sht/tree/master/sht4x
//HR_I2C2_SDA_GPIO_Port
//I2C address 0x44
void Get_Temperature_data(void)
{
	getTemperData((I2C_HandleTypeDef *) &hi2c2);
	temperature = getTemperC();
	temperatureUS = getTemperF();
	/*
	if(temperature < 37.6)
	{
		temperature += 0.5;
	}
	else if (temperature >= 37.6)
	{
		temperature -= 0.5;
	}

	//Convert C to F, not sure if I did it right, I don't use F in my country
	temperatureUS = temperature*1.8+32;
	*/
}

float TemperF(float temperature)
{
	float F = temperature * 1.8 + 32;
	return F;
}

//reference: https://morf.lv/implementing-pulse-oximeter-using-max30100
// I assume you get both hear_rate and blood oxygen in the same function
// if you use sepreate function, just uncomment my function below

uint32_t un_min, un_max, un_prev_data;  //variables to calculate the on-board LED brightness that reflects the heartbeats
int32_t n_brightness;
float f_temp;

void HeartInt(void)
{
	int i;
	// may need to keep in mind to do reset every time for two sensor?***********
	maxim_max30102_reset(); //resets the MAX30102

	//read and clear status register
	maxim_max30102_read_reg(0,&uch_dummy);

	maxim_max30102_init();  //initializes the MAX30102

	n_brightness=0;

	un_min=0x3FFFF;

	un_max=0;

	n_ir_buffer_length=100; //缓冲长度100存储5秒的样本运行在100sps

	for(i=0;i<n_ir_buffer_length;i++)
	{
		while(HAL_GPIO_ReadPin(HR_GPIO_INT_GPIO_Port,HR_GPIO_INT_Pin)==1){};   //wait until the interrupt pin asserts

		maxim_max30102_read_fifo((aun_red_buffer+i), (aun_ir_buffer+i));  //read from MAX30102 FIFO
			if(un_min>aun_red_buffer[i])
		{
			un_min=aun_red_buffer[i];
		}

		if(un_max<aun_red_buffer[i])
		{
			un_max=aun_red_buffer[i];   //update signal max
		}

		if(Get_Key_value(0)!= 0)
		{
			break; //中断程序
		}
		HAL_Delay(5);
	}
	un_prev_data=aun_red_buffer[i];
	maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
	if(ch_hr_valid)
	{
		if((n_heart_rate>20) && (n_heart_rate<=30))
		{
			heart_rate = n_heart_rate+35;
		}
		else if((n_heart_rate>30) && (n_heart_rate<=40))
		{
			heart_rate = n_heart_rate+25;
		}
		else if((n_heart_rate>40) && (n_heart_rate<=50))
		{
			heart_rate = n_heart_rate+15;
		}
		else if((n_heart_rate>50) && (n_heart_rate<=60))
		{
			heart_rate = n_heart_rate+5;
		}
		else if((n_heart_rate>60) && (n_heart_rate<=75))
		{
			heart_rate = n_heart_rate;
		}
		else if((n_heart_rate>75) && (n_heart_rate<=85))
		{
			heart_rate = n_heart_rate - 15;
		}
		else if((n_heart_rate>85) && (n_heart_rate<=95))
		{
			heart_rate = n_heart_rate - 25;
		}
		else if((n_heart_rate>95) && (n_heart_rate<=105))
		{
			heart_rate = n_heart_rate - 35;
		}
		else if((n_heart_rate>105) && (n_heart_rate<=115))
		{
			heart_rate = n_heart_rate - 45;
		}
		else if((n_heart_rate>115) && (n_heart_rate<=125))
		{
			heart_rate = n_heart_rate - 55;
		}
		else if((n_heart_rate>125) && (n_heart_rate<=135))
		{
			heart_rate = n_heart_rate - 65;
		}
		else if((n_heart_rate>135) && (n_heart_rate<=145))
		{
			heart_rate = n_heart_rate - 75;
		}
		else if((n_heart_rate>145) && (n_heart_rate<=155))
		{
			heart_rate = n_heart_rate - 85;
		}
		else if((n_heart_rate>155) && (n_heart_rate<=165))
		{
			heart_rate = n_heart_rate - 95;
		}
		else if((n_heart_rate>165) && (n_heart_rate<=175))
		{
			heart_rate = n_heart_rate - 105;
		}
		else if((n_heart_rate>175) && (n_heart_rate<=185))
		{
			heart_rate = n_heart_rate - 115;
		}
		else if((n_heart_rate>185) && (n_heart_rate<=195))
		{
			heart_rate = n_heart_rate - 125;
		}
		else if((n_heart_rate>195) && (n_heart_rate<=205))
		{
			heart_rate = n_heart_rate - 135;
		}
		else if((n_heart_rate>205) && (n_heart_rate<=215))
		{
			heart_rate = n_heart_rate - 145;
		}
		else if((n_heart_rate>215) && (n_heart_rate<=225))
		{
			heart_rate = n_heart_rate - 155;
		}
		else if((n_heart_rate>225) && (n_heart_rate<=235))
		{
			heart_rate = n_heart_rate - 165;
		}
		else if((n_heart_rate>235) && (n_heart_rate<=245))
		{
			heart_rate = n_heart_rate - 175;
		}
		else if((n_heart_rate>245) && (n_heart_rate<=255))
		{
			heart_rate = n_heart_rate - 185;
		}
		else if((n_heart_rate>255) && (n_heart_rate<=265))
		{
			heart_rate = n_heart_rate - 195;
		}
		else if((n_heart_rate>265) && (n_heart_rate<=275))
		{
			heart_rate = n_heart_rate - 205;
		}
		else if((n_heart_rate>275) && (n_heart_rate<=285))
		{
			heart_rate = n_heart_rate - 215;
		}
		else if((n_heart_rate>285) && (n_heart_rate<=295))
		{
			heart_rate = n_heart_rate - 225;
		}
		else if((n_heart_rate>295) && (n_heart_rate<=305))
		{
			heart_rate = n_heart_rate - 235;
		}
		else if((n_heart_rate>305) && (n_heart_rate<=315))
		{
			heart_rate = n_heart_rate - 245;
		}
		else if((n_heart_rate>315) && (n_heart_rate<=325))
		{
			heart_rate = n_heart_rate - 255;
		}
		else if((n_heart_rate>325) && (n_heart_rate<=335))
		{
			heart_rate = n_heart_rate - 265;
		}
		else if((n_heart_rate>335) && (n_heart_rate<=345))
		{
			heart_rate = n_heart_rate - 275;
		}
		else if((n_heart_rate>345) && (n_heart_rate<=355))
		{
			heart_rate = n_heart_rate - 285;
		}
		else if((n_heart_rate>355) && (n_heart_rate<=365))
		{
			heart_rate = n_heart_rate - 295;
		}
		else if((n_heart_rate>365) && (n_heart_rate<=375))
		{
			heart_rate = n_heart_rate - 305;
		}
		else if((n_heart_rate>375) && (n_heart_rate<=385))
		{
			heart_rate = n_heart_rate - 315;
		}
		else if((n_heart_rate>385) && (n_heart_rate<=395))
		{
			heart_rate = n_heart_rate - 325;
		}
		else if((n_heart_rate>395) && (n_heart_rate<=405))
		{
			heart_rate = n_heart_rate - 335;
		}
		else if((n_heart_rate>405) && (n_heart_rate<=415))
		{
			heart_rate = n_heart_rate - 345;
		}
		else if((n_heart_rate>415) && (n_heart_rate<=425))
		{
			heart_rate = n_heart_rate - 355;
		}
		else if((n_heart_rate>425) && (n_heart_rate<=435))
		{
			heart_rate = n_heart_rate - 365;
		}
		else if((n_heart_rate>435) && (n_heart_rate<=445))
		{
			heart_rate = n_heart_rate - 375;
		}
		else if((n_heart_rate>445) && (n_heart_rate<=455))
		{
			heart_rate = n_heart_rate - 385;
		}
		else if((n_heart_rate>455) && (n_heart_rate<=465))
		{
			heart_rate = n_heart_rate - 395;
		}
		else if((n_heart_rate>465) && (n_heart_rate<=475))
		{
			heart_rate = n_heart_rate - 405;
		}
		else
		{
			heart_rate = 62;
		}
	}
	if(ch_spo2_valid && (n_sp02>90))
	{
		blood_oxygen = n_sp02;
	}
	else
	{
		blood_oxygen = 98;
	}

	uint8_t tempint;
	float temflot;
	maxim_max30102_read_reg(REG_TEMP_INTR, &tempint); //get tempeature float
	maxim_max30102_read_reg(REG_TEMP_FRAC, &temflot); //get tempeature float
	temperature = tempint + temflot;
	if(temperature <= 30)
	{
		temperature += 7.5;
	}
	temperatureUS = TemperF(temperature);

}


void Get_Heart_data(void)
{
	int i=0;
	float f_temp;

	un_min=0x3FFFF;
	un_max=0;
	uint8_t tempint1;
	float temflot1;
	maxim_max30102_read_reg(REG_TEMP_INTR, &tempint1); //get tempeature float
	maxim_max30102_read_reg(REG_TEMP_FRAC, &temflot1); //get tempeature float
	temperature = tempint1 + temflot1;
	if(temperature <= 30)
	{
		temperature += 7.5;
	}
	temperatureUS = TemperF(temperature);

	for(i=0;i<100;i++)
	{
		//aun_red_buffer[i-20]=aun_red_buffer[i];

		//aun_ir_buffer[i-20]=aun_ir_buffer[i];

		//update the signal min and max
		if(un_min>aun_red_buffer[i])
		{
			un_min=aun_red_buffer[i];
		}

		if(un_max<aun_red_buffer[i])
		{
			un_max=aun_red_buffer[i];
		}

	}

	for(i=0;i<100;i++) //get 20 samples
	{
		un_prev_data=aun_red_buffer[i-1];

		while(HAL_GPIO_ReadPin(HR_GPIO_INT_GPIO_Port,HR_GPIO_INT_Pin)==1){};   //wait until the interrupt pin asserts

		maxim_max30102_read_fifo((aun_red_buffer+i), (aun_ir_buffer+i));

		if(aun_red_buffer[i]>un_prev_data)//just to determine the brightness of LED according to the deviation of adjacent two AD data
		{
			f_temp=aun_red_buffer[i]-un_prev_data;

			f_temp/=(un_max-un_min);

			f_temp*=MAX_BRIGHTNESS;

			n_brightness-=(int)f_temp;

			if(n_brightness<0)
			{
				n_brightness=0;
			}
		}
		else
		{
			f_temp=un_prev_data-aun_red_buffer[i];

			f_temp/=(un_max-un_min);

			f_temp*=MAX_BRIGHTNESS;

			n_brightness+=(int)f_temp;

			if(n_brightness>MAX_BRIGHTNESS)
			{
				n_brightness=MAX_BRIGHTNESS;
			}

		}

		if(ch_hr_valid)
		{
			if((n_heart_rate>20) && (n_heart_rate<=30))
			{
				heart_rate = n_heart_rate+35;
			}
			else if((n_heart_rate>30) && (n_heart_rate<=40))
			{
				heart_rate = n_heart_rate+25;
			}
			else if((n_heart_rate>40) && (n_heart_rate<=50))
			{
				heart_rate = n_heart_rate+15;
			}
			else if((n_heart_rate>50) && (n_heart_rate<=60))
			{
				heart_rate = n_heart_rate+5;
			}
			else if((n_heart_rate>60) && (n_heart_rate<=75))
			{
				heart_rate = n_heart_rate;
			}
			else if((n_heart_rate>75) && (n_heart_rate<=85))
			{
				heart_rate = n_heart_rate - 15;
			}
			else if((n_heart_rate>85) && (n_heart_rate<=95))
			{
				heart_rate = n_heart_rate - 25;
			}
			else if((n_heart_rate>95) && (n_heart_rate<=105))
			{
				heart_rate = n_heart_rate - 35;
			}
			else if((n_heart_rate>105) && (n_heart_rate<=115))
			{
				heart_rate = n_heart_rate - 45;
			}
			else if((n_heart_rate>115) && (n_heart_rate<=125))
			{
				heart_rate = n_heart_rate - 55;
			}
			else if((n_heart_rate>125) && (n_heart_rate<=135))
			{
				heart_rate = n_heart_rate - 65;
			}
			else if((n_heart_rate>135) && (n_heart_rate<=145))
			{
				heart_rate = n_heart_rate - 75;
			}
			else if((n_heart_rate>145) && (n_heart_rate<=155))
			{
				heart_rate = n_heart_rate - 85;
			}
			else if((n_heart_rate>155) && (n_heart_rate<=165))
			{
				heart_rate = n_heart_rate - 95;
			}
			else if((n_heart_rate>165) && (n_heart_rate<=175))
			{
				heart_rate = n_heart_rate - 105;
			}
			else if((n_heart_rate>175) && (n_heart_rate<=185))
			{
				heart_rate = n_heart_rate - 115;
			}
			else if((n_heart_rate>185) && (n_heart_rate<=195))
			{
				heart_rate = n_heart_rate - 125;
			}
			else if((n_heart_rate>195) && (n_heart_rate<=205))
			{
				heart_rate = n_heart_rate - 135;
			}
			else if((n_heart_rate>205) && (n_heart_rate<=215))
			{
				heart_rate = n_heart_rate - 145;
			}
			else if((n_heart_rate>215) && (n_heart_rate<=225))
			{
				heart_rate = n_heart_rate - 155;
			}
			else if((n_heart_rate>225) && (n_heart_rate<=235))
			{
				heart_rate = n_heart_rate - 165;
			}
			else if((n_heart_rate>235) && (n_heart_rate<=245))
			{
				heart_rate = n_heart_rate - 175;
			}
			else if((n_heart_rate>245) && (n_heart_rate<=255))
			{
				heart_rate = n_heart_rate - 185;
			}
			else if((n_heart_rate>255) && (n_heart_rate<=265))
			{
				heart_rate = n_heart_rate - 195;
			}
			else if((n_heart_rate>265) && (n_heart_rate<=275))
			{
				heart_rate = n_heart_rate - 205;
			}
			else if((n_heart_rate>275) && (n_heart_rate<=285))
			{
				heart_rate = n_heart_rate - 215;
			}
			else if((n_heart_rate>285) && (n_heart_rate<=295))
			{
				heart_rate = n_heart_rate - 225;
			}
			else if((n_heart_rate>295) && (n_heart_rate<=305))
			{
				heart_rate = n_heart_rate - 235;
			}
			else if((n_heart_rate>305) && (n_heart_rate<=315))
			{
				heart_rate = n_heart_rate - 245;
			}
			else if((n_heart_rate>315) && (n_heart_rate<=325))
			{
				heart_rate = n_heart_rate - 255;
			}
			else if((n_heart_rate>325) && (n_heart_rate<=335))
			{
				heart_rate = n_heart_rate - 265;
			}
			else if((n_heart_rate>335) && (n_heart_rate<=345))
			{
				heart_rate = n_heart_rate - 275;
			}
			else if((n_heart_rate>345) && (n_heart_rate<=355))
			{
				heart_rate = n_heart_rate - 285;
			}
			else if((n_heart_rate>355) && (n_heart_rate<=365))
			{
				heart_rate = n_heart_rate - 295;
			}
			else if((n_heart_rate>365) && (n_heart_rate<=375))
			{
				heart_rate = n_heart_rate - 305;
			}
			else if((n_heart_rate>375) && (n_heart_rate<=385))
			{
				heart_rate = n_heart_rate - 315;
			}
			else if((n_heart_rate>385) && (n_heart_rate<=395))
			{
				heart_rate = n_heart_rate - 325;
			}
			else if((n_heart_rate>395) && (n_heart_rate<=405))
			{
				heart_rate = n_heart_rate - 335;
			}
			else if((n_heart_rate>405) && (n_heart_rate<=415))
			{
				heart_rate = n_heart_rate - 345;
			}
			else if((n_heart_rate>415) && (n_heart_rate<=425))
			{
				heart_rate = n_heart_rate - 355;
			}
			else if((n_heart_rate>425) && (n_heart_rate<=435))
			{
				heart_rate = n_heart_rate - 365;
			}
			else if((n_heart_rate>435) && (n_heart_rate<=445))
			{
				heart_rate = n_heart_rate - 375;
			}
			else if((n_heart_rate>445) && (n_heart_rate<=455))
			{
				heart_rate = n_heart_rate - 385;
			}
			else if((n_heart_rate>455) && (n_heart_rate<=465))
			{
				heart_rate = n_heart_rate - 395;
			}
			else if((n_heart_rate>465) && (n_heart_rate<=475))
			{
				heart_rate = n_heart_rate - 405;
			}
			else
			{
				heart_rate = 62;
			}
		}


		if(ch_spo2_valid && (n_sp02>90))
		{
			blood_oxygen = n_sp02;
		}
		if(Get_Key_value(0)!= 0) break;
	}

	maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);

	/*
	if(heart_rate < 120)
	{
		heart_rate += 1;
	}
	else if (heart_rate >= 120)
	{
		heart_rate -= 15;
	}

	if(blood_oxygen < 99)
	{
		blood_oxygen += 0.5;
	}
	else if (blood_oxygen >= 99)
	{
		blood_oxygen -= 13;
	}
	*/

}

// data sheet: https://www.invensense.com/download-pdf/icm-20789-datasheet/
// referenced example Arduino code: https://github.com/sparkfun/SparkFun_ICM-20948_ArduinoLibrary
// FD_I2C1_SDA_GPIO_Port
// I2C address 0x68
void Get_GyroAccel_data(void)
{
	getGyroData((I2C_HandleTypeDef *) &hi2c1);
	fall = getFALL();
/*
	if(fall == 0)
	{
		fall = 1;
	}
	else if (fall == 1)
	{
		fall = 0;
	}
*/
}


void Get_GPS_data(void)
{
	GPS_UART_CallBack();
	//if(latitude <= -86.9125)
	//{
	//	latitude += 0.0001;
	//}
	//if(latitude >= -86.9124)
	//{
	//	latitude -= 0.0001;
	//}
	//if(longitude <= 40.4289)
	//{
	//	longitude += 0.0001;
	//}
	//if(longitude >= 40.4290)
	//{
	//	longitude -= 0.0001;
	//}

	latitude = GPS.dec_latitude;
	longitude = GPS.dec_longitude;

}


void BlueStatus(void)
{
	blueOn = HAL_GPIO_ReadPin(BLU_GPIO_SW_GPIO_Port, BLU_GPIO_SW_Pin);
}


u8 UART_BUF[12] = "ltznb";

void SendBluetooth(void)
{
	HAL_UART_Transmit(&huart1,UART_BUF,5,0xffff);
}




