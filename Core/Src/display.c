#include "stdio.h"
#include "display.h"
#include "main.h"
#include "lcd.h"
#include "pic.h"
#include "control.h"


/*******************************************
* @function: 	display startup menu
* @input:		n/a
* @return:		n/a
*******************************************/

float temperature = 37;
u8 temperature1[5] ; //temperature string

float temperatureUS = 98.6;
u8 temperatureUS1[6] ; //temperature string

unsigned char heart_rate = 100;
u8 heart_rate1[5] ; //heart rate string

unsigned char blood_oxygen = 95;
u8 blood_oxygen1[3] ; //blood oxygen string

float latitude = 10;
u8 latitude1[6] ; //latitude string

float longitude = 10;
u8 longitude1[6] ; //longitude string

unsigned char fall = 0;

unsigned char blueOn = 0;
//unsigned char GPSon = 0;
unsigned char unit = 1;

extern unsigned char selection = 10;
extern unsigned char selectedFlag = 100;

void MENU_init_(void)
{

	//LCD_DrawRectangle(0,0,128, 160,WHITE);
	//LCD_Fill(LCD_W/16*10, 0,LCD_W/16*13,LCD_H/16*2,LIGHTBLUE); //bluetooth background
	LCD_Fill(LCD_W/16*13, 0,LCD_W,LCD_H/16*2,MAGENTA); //gps background
	//LCD_ShowChar(LCD_W/16*11,0,'B',BLUE,LIGHTBLUE,16,1);//bluetooth word
	LCD_ShowChar(LCD_W/16*14,0,'P',CYAN,MAGENTA,16,1); //Power word

	LCD_ShowString (LCD_W/16*1,LCD_H/16*3,"TEMP", WHITE,	BLACK, 12,	1); //Tempature
	LCD_ShowString (LCD_W/16*1,LCD_H/16*5,"SpO2", WHITE,	BLACK, 12,	1); //Blood Oxygen
	LCD_ShowString (LCD_W/16*1,LCD_H/16*7,"Heart", WHITE,	BLACK, 12,	1);
	LCD_ShowString (LCD_W/16*1,LCD_H/16*9,"Latitude", WHITE,	BLACK, 12,	1);
	LCD_ShowString (LCD_W/16*1,LCD_H/16*11,"Longitude", WHITE,	BLACK, 12,	1);
	LCD_ShowString (LCD_W/16*1,LCD_H/16*13,"Fall?", WHITE,	BLACK, 12,	1);

	LCD_ShowString (LCD_W/16*6,LCD_H/16*14+8,"SETTING", WHITE,	BLACK, 12,	1);
	selection = SETTING;

	if(unit == 1)
	{
		LCD_ShowString(LCD_W/16*14, LCD_H/16*3,	"F",	WHITE,	BLACK, 12,	1);
	}
	else
	{
		LCD_ShowChinese(LCD_W/16*13, LCD_H/16*3,	"℃",	WHITE,	BLACK, 16,	1);
	}

	LCD_ShowString (LCD_W/16*14, LCD_H/16*5,    "%", 	WHITE,	BLACK, 12,	1);
	LCD_ShowString (LCD_W/16*13, LCD_H/16*7,    "BPM", 	WHITE,	BLACK, 12,	1);

	// showing the values in case they equal to history
	// begin ------------------
	LCD_Fill(LCD_W/16*10, 0,LCD_W/16*13,LCD_H/16*2,LIGHTBLUE); //bluetooth background
	//LCD_DrawLine(LCD_W/16*10, 0, LCD_W/16*13, LCD_H/16*2, LIGHTBLUE);
	//LCD_DrawLine(LCD_W/16*13, 0, LCD_W/16*10, LCD_H/16*2, LIGHTBLUE);
	if(blueOn == 1)
	{
		LCD_ShowChar(LCD_W/16*11,0,'B',BLUE,LIGHTBLUE,16,1);//bluetooth word
	}
	else
	{
		LCD_ShowChar(LCD_W/16*11,0,'B',BLUE,LIGHTBLUE,16,1);//bluetooth word
		LCD_DrawLine(LCD_W/16*10, 0, LCD_W/16*13, LCD_H/16*2, WHITE);
		LCD_DrawLine(LCD_W/16*13, 0, LCD_W/16*10, LCD_H/16*2, WHITE);
	}

	LCD_Fill(LCD_W/16*7,LCD_H/16*3,LCD_W/16*12,LCD_H/16*4,BLACK);
	if(unit == 1) //US unite
	{
		if((temperature < 28) || (temperature>38))
		{
			LCD_ShowFloatVar(LCD_W/16*7,LCD_H/16*3,temperatureUS,temperatureUS1, RED,BLACK,12); //show tempeature
		}
		else
		{
			LCD_ShowFloatVar(LCD_W/16*7,LCD_H/16*3,temperatureUS,temperatureUS1, WHITE,BLACK,12); //show tempeature
		}
	}
	else
	{
		if((temperature < 28) || (temperature>38))
		{
			LCD_ShowFloatVar(LCD_W/16*7,LCD_H/16*3,temperature,temperature1, RED,BLACK,12); //show tempeature
		}
		else
		{
			LCD_ShowFloatVar(LCD_W/16*7,LCD_H/16*3,temperature,temperature1, WHITE,BLACK,12); //show tempeature
		}
	}

	//LCD_Fill(LCD_W/16*7,LCD_H/16*5,LCD_W/16*12,LCD_H/16*6,BLACK);
	//LCD_ShowString (LCD_W/16*9,LCD_H/16*5,"YES", RED,	BLACK, 16,	1);
	if(blood_oxygen < 95)
	{
		LCD_ShowIntVar(LCD_W/16*7,LCD_H/16*5,blood_oxygen,blood_oxygen1, RED,BLACK,12); //show blood oxygen
	}
	else
	{
		LCD_ShowIntVar(LCD_W/16*7,LCD_H/16*5,blood_oxygen,blood_oxygen1, WHITE,BLACK,12); //show blood oxygen
	}

	//LCD_Fill(LCD_W/16*7,LCD_H/16*7,LCD_W/16*12,LCD_H/16*8,BLACK);
	if((heart_rate < 60) || (heart_rate > 100))
	{
		LCD_ShowIntVar(LCD_W/16*7,LCD_H/16*7,heart_rate,heart_rate1, RED,BLACK,12); //show heart rate
	}
	else
	{
		LCD_ShowIntVar(LCD_W/16*7,LCD_H/16*7,heart_rate,heart_rate1, WHITE,BLACK,12); //show heart rate
	}

	//LCD_Fill(LCD_W/16*9,LCD_H/16*9,LCD_W/16*12,LCD_H/16*10,BLACK);
	LCD_Fill(LCD_W/16*9,LCD_H/16*9,LCD_W/16*13,LCD_H/16*10,BLACK);
	if(latitude == 0)
	{
		LCD_ShowString (LCD_W/16*9,LCD_H/16*9,"No Conn.", RED,	BLACK, 12,	1);
		//LCD_ShowFloatVar(LCD_W/16*9,LCD_H/16*9,latitude,latitude1, WHITE,BLACK,12);
	}
	else
	{
		LCD_ShowFloatVar(LCD_W/16*9,LCD_H/16*9,latitude,latitude1, WHITE,BLACK,12);
	}
	LCD_Fill(LCD_W/16*9,LCD_H/16*11,LCD_W/16*13,LCD_H/16*12,BLACK);
	if(longitude == 0)
	{
		LCD_ShowString (LCD_W/16*9,LCD_H/16*11,"No Conn.", RED,	BLACK, 12,	1);
	}
	else
	{
		LCD_ShowFloatVar(LCD_W/16*9,LCD_H/16*11,longitude,longitude1, WHITE,BLACK,12);
	}
	//LCD_ShowFloatVar(LCD_W/16*9,LCD_H/16*9,latitude,latitude1, WHITE,BLACK,12);
	//LCD_Fill(LCD_W/16*9,LCD_H/16*11,LCD_W/16*12,LCD_H/16*12,BLACK);
	//LCD_ShowFloatVar(LCD_W/16*9,LCD_H/16*11,longitude,longitude1, WHITE,BLACK,12);
	//LCD_Fill(LCD_W/16*8,LCD_H/16*13,LCD_W/16*13,LCD_H/16*14,BLACK);
	if(fall == 1)
	{
		LCD_ShowString (LCD_W/16*8,LCD_H/16*13,"YES", RED,	BLACK, 12,	1);
	}
	else
	{
		LCD_ShowString (LCD_W/16*8,LCD_H/16*13,"NO", WHITE,BLACK, 12,	1);
	}
	//end --------------------------

	LCD_DrawRectangle(1, LCD_H/16*14+8, LCD_W-1, LCD_H-1,WHITE); // Setting selection

}




/*******************************************
* @function: 	Display startup
* @input:		n/a
* @return:		n/a
*******************************************/
void MENU_init(void)
{
	LCD_Init();//LCD initialize

	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);

	//LCD_ShowString (LCD_W/16*1,LCD_H/16*3,"Temperature", WHITE,	BLACK, 12,	0);
	//LCD_ShowString (LCD_W/16*1,LCD_H/16*6,"Temperature", WHITE,	BLACK, 12,	1);
	//LCD_ShowString (LCD_W/16*1,LCD_H/16*9,"Temperature", WHITE,	BLACK, 32,	0);
	//LCD_ShowString (LCD_W/16*1,LCD_H/16*12,"Temperature", WHITE,BLACK, 32,	1);
	MENU_init_();
}


/*******************************************
* @function: 	main menu(things to display to the user)
* @input:		n/a
* @return:		n/a
*******************************************/
// Self-defined variables
//static unsigned char history_time[6] = {100,100,100,100,100,100};

static float history_temperature = 32;

static unsigned char history_heart_rate = 96;

static unsigned char history_blood_oxygen = 95;

static float history_latitude = 10.0;

static float history_longitude = 10.0;

static unsigned char history_fall = 1;

static unsigned char history_blueOn = 1;

//static unsigned char history_GPSon = 1;


void MENU_main(void)
{
	selectedFlag = 100;

	if(history_blueOn != blueOn)
	{
		LCD_Fill(LCD_W/16*10, 0,LCD_W/16*13,LCD_H/16*2,LIGHTBLUE); //bluetooth background
		//LCD_DrawLine(LCD_W/16*10, 0, LCD_W/16*13, LCD_H/16*2, LIGHTBLUE);
		//LCD_DrawLine(LCD_W/16*13, 0, LCD_W/16*10, LCD_H/16*2, LIGHTBLUE);
		if(blueOn == 1)
		{
			LCD_ShowChar(LCD_W/16*11,0,'B',BLUE,LIGHTBLUE,16,1);//bluetooth word
		}
		else
		{
			LCD_ShowChar(LCD_W/16*11,0,'B',BLUE,LIGHTBLUE,16,1);//bluetooth word
			LCD_DrawLine(LCD_W/16*10, 0, LCD_W/16*13, LCD_H/16*2, WHITE);
			LCD_DrawLine(LCD_W/16*13, 0, LCD_W/16*10, LCD_H/16*2, WHITE);
		}
	}


	if(	history_temperature != temperature)
	{
		LCD_Fill(LCD_W/16*7,LCD_H/16*3,LCD_W/16*12,LCD_H/16*4,BLACK);
		if(unit == 1) //US unite
		{
			if((temperature < 28) || (temperature>38))
			{
				LCD_ShowFloatVar(LCD_W/16*7,LCD_H/16*3,temperatureUS,temperatureUS1, RED,BLACK,12); //show tempeature
			}
			else
			{
				LCD_ShowFloatVar(LCD_W/16*7,LCD_H/16*3,temperatureUS,temperatureUS1, WHITE,BLACK,12); //show tempeature
			}
		}
		else
		{
			if((temperature < 28) || (temperature>38))
			{
				LCD_ShowFloatVar(LCD_W/16*7,LCD_H/16*3,temperature,temperature1, RED,BLACK,12); //show tempeature
			}
			else
			{
				LCD_ShowFloatVar(LCD_W/16*7,LCD_H/16*3,temperature,temperature1, WHITE,BLACK,12); //show tempeature
			}
		}


	}
	if(	history_blood_oxygen != blood_oxygen)
	{
		LCD_Fill(LCD_W/16*7,LCD_H/16*5,LCD_W/16*12,LCD_H/16*6,BLACK);
		//LCD_ShowString (LCD_W/16*9,LCD_H/16*5,"YES", RED,	BLACK, 16,	1);
		if(blood_oxygen < 95)
		{
			LCD_ShowIntVar(LCD_W/16*7,LCD_H/16*5,blood_oxygen,blood_oxygen1, RED,BLACK,12); //show blood oxygen
		}
		else
		{
			LCD_ShowIntVar(LCD_W/16*7,LCD_H/16*5,blood_oxygen,blood_oxygen1, WHITE,BLACK,12); //show blood oxygen
		}
	}

	if(	history_heart_rate != heart_rate)
	{
		LCD_Fill(LCD_W/16*7,LCD_H/16*7,LCD_W/16*12,LCD_H/16*8,BLACK);
		if((heart_rate < 60) || (heart_rate > 100))
		{
			LCD_ShowIntVar(LCD_W/16*7,LCD_H/16*7,heart_rate,heart_rate1, RED,BLACK,12); //show heart rate
		}
		else
		{
			LCD_ShowIntVar(LCD_W/16*7,LCD_H/16*7,heart_rate,heart_rate1, WHITE,BLACK,12); //show heart rate
		}

	}

	if(	history_latitude != latitude)
	{
		LCD_Fill(LCD_W/16*9,LCD_H/16*9,LCD_W/16*12,LCD_H/16*10,BLACK);
		if(latitude == 0)
		{
			LCD_ShowString (LCD_W/16*9,LCD_H/16*9,"No Conn.", RED,	BLACK, 12,	1);
			//LCD_ShowFloatVar(LCD_W/16*9,LCD_H/16*9,latitude,latitude1, WHITE,BLACK,12);
		}
		else
		{
			LCD_ShowFloatVar(LCD_W/16*9,LCD_H/16*9,latitude,latitude1, WHITE,BLACK,12);
		}
		//LCD_ShowIntVar (LCD_W/16*9,LCD_H/16*9,heart_rate,heart_rate1, BLACK,WHITE,16); //show heart rate
	}

	if(	history_longitude != longitude)
	{
		LCD_Fill(LCD_W/16*9,LCD_H/16*11,LCD_W/16*12,LCD_H/16*12,BLACK);
		if(longitude == 0)
		{
			LCD_ShowString (LCD_W/16*9,LCD_H/16*11,"No Conn.", RED,	BLACK, 12,	1);
		}
		else
		{
			LCD_ShowFloatVar(LCD_W/16*9,LCD_H/16*11,longitude,longitude1, WHITE,BLACK,12);
		}

		//LCD_ShowIntVar(LCD_W/16*9,LCD_H/16*11,heart_rate,heart_rate1, BLACK,WHITE,16); //show heart rate
	}

	if(	history_fall != fall)
	{
		LCD_Fill(LCD_W/16*8,LCD_H/16*13,LCD_W/16*13,LCD_H/16*14,BLACK);
		if(fall == 1)
		{
			LCD_ShowString (LCD_W/16*8,LCD_H/16*13,"YES", RED,	BLACK, 12,	1);
		}
		else
		{
			LCD_ShowString (LCD_W/16*8,LCD_H/16*13,"NO", WHITE,BLACK, 12,	1);
		}
	}

	//HAL_Delay(300);

	history_temperature = temperature;
	history_blood_oxygen = blood_oxygen;
	history_heart_rate = heart_rate;

	history_latitude = latitude;
	history_longitude = longitude;

	history_fall = fall;
	history_blueOn = blueOn;
	//history_GPSon = GPSon;

}

/*******************************************
* @function: 	setting page
* @input:		n/a
* @return:		n/a
*******************************************/
void MENU_SETTING(void)
{
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	LCD_ShowString (LCD_W/16*6,0,"SETTING", WHITE,	BLACK, 12,	1);
	LCD_ShowString (1,LCD_H/16*2,"Unit", WHITE,	BLACK, 12,	1);
	LCD_ShowString (LCD_W/16*9,LCD_H/16*2,"US", WHITE,	BLACK, 12,	1);



	LCD_ShowString (LCD_W/16*13,LCD_H/16*2,"SI", WHITE,	BLACK, 12,	1);



	LCD_ShowString (1,LCD_H/16*4,"Bluetooth", WHITE,	BLACK, 12,	1);
	LCD_ShowString (LCD_W/16*9,LCD_H/16*4,"ON", WHITE,	BLACK, 12,	1);



	LCD_ShowString (LCD_W/16*13,LCD_H/16*4,"OFF", WHITE,	BLACK, 12,	1);



	//LCD_ShowString (LCD_W/16*1,LCD_H/16*6,"GPS", WHITE,	BLACK, 16,	1);
	//LCD_ShowString (LCD_W/16*9,LCD_H/16*6,"ON", WHITE,	BLACK, 16,	1);



	//LCD_ShowString (LCD_W/16*14,LCD_H/16*6,"OFF", WHITE,	BLACK, 16,	1);


}


void MENU_TEMP(void)
{
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	LCD_ShowString (LCD_W/16*4,0,"Temperature", WHITE,	BLACK, 12,	1);
	LCD_ShowChinese(LCD_W/16*13, LCD_H/16*3,	"℃",	WHITE,	BLACK, 16,	1);
	LCD_ShowString(LCD_W/16*13, LCD_H/16*5,	"F",	WHITE,	BLACK, 16,	1);
	//LCD_ShowString (LCD_W/2,LCD_H/2,"TEMP", WHITE,	BLACK, 16,	1);
}

void MENU_BLOOD(void)
{
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	LCD_ShowString (LCD_W/16*4,0,"Blood Oxygen", WHITE,	BLACK, 12,	1);
	LCD_ShowString (LCD_W/16*14, LCD_H/16*3,    "%", 	WHITE,	BLACK, 16,	1);

	//LCD_ShowString (LCD_W/2,LCD_H/2,"BLOOD", WHITE,	BLACK, 16,	1);

}

void MENU_HEART(void)
{
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	LCD_ShowString (LCD_W/16*4,0,"Heart Rate", WHITE,	BLACK, 12,	1);
	LCD_ShowString (LCD_W/16*10, LCD_H/16*3,    "BPM", 	WHITE,	BLACK, 16,	1);
	//LCD_ShowString (LCD_W/2,LCD_H/2,"HEART", WHITE,	BLACK, 16,	1);

}


void MENU_LAT(void)
{
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	LCD_ShowString (LCD_W/16*4,0,"Location", WHITE,	BLACK, 12,	1);
	LCD_ShowString (LCD_W/16*1,LCD_H/16*3,"Latitude", WHITE,	BLACK, 12,	1);
	LCD_ShowString (LCD_W/16*1,LCD_H/16*5,"Longitude", WHITE,	BLACK, 12,	1);
	//LCD_ShowString (LCD_W/16*6,0,"Location", WHITE,	BLACK, 16,	1);
	//LCD_ShowString (LCD_W/2,LCD_H/2,"LAT", WHITE,	BLACK, 16,	1);

}

void MENU_LON(void)
{
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	LCD_ShowString (LCD_W/16*4,0,"Location", WHITE,	BLACK, 12,	1);
	LCD_ShowString (LCD_W/16*1,LCD_H/16*3,"Latitude", WHITE,	BLACK, 12,	1);
	LCD_ShowString (LCD_W/16*1,LCD_H/16*5,"Longitude", WHITE,	BLACK, 12,	1);
	//LCD_ShowString (LCD_W/2,LCD_H/2,"LON", WHITE,	BLACK, 16,	1);

}

void MENU_FALL(void)
{
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	LCD_ShowString (LCD_W/16*4,0,"Fall Status", WHITE,	BLACK, 12,	1);
	//LCD_ShowString (LCD_W/2,LCD_H/2,"FALL", WHITE,	BLACK, 16,	1);

}

void MENU_BLUET(void)
{
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	LCD_ShowString (LCD_W/16*2,0,"Bluetooth Status", WHITE,	BLACK, 12,	1);
	if(blueOn == 1)
	{
		LCD_ShowString (LCD_W/16*7,LCD_H/16*3,"ON", WHITE, BLACK, 16,	1);
	}
	else
	{
		LCD_ShowString (LCD_W/16*7,LCD_H/16*3,"OFF", WHITE,BLACK, 16,	1);
	}
	//LCD_ShowString (LCD_W/2,LCD_H/2,"BLUET", WHITE,	BLACK, 16,	1);

}

void MENU_GPS(void)
{
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	LCD_ShowString (LCD_W/16*4,0,"Location", WHITE,	BLACK, 12,	1);
	LCD_ShowString (LCD_W/16*1,LCD_H/16*3,"Latitude", WHITE,	BLACK, 12,	1);
	LCD_ShowString (LCD_W/16*1,LCD_H/16*5,"Longitude", WHITE,	BLACK, 12,	1);
}


void Refresh(void)
{
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	MENU_init_();
	MENU_main();
}


/*******************************************
* @function: 	setting page
* @input:		n/a
* @return:		n/a
*******************************************/
unsigned char MENU_SELECTION(unsigned char switch_value)
{
	if((switch_value == 1) || (switch_value == 3))
	{
		switch(selection)
		{
			case SETTING :
				LCD_DrawRectangle(1, LCD_H/16*14+8, LCD_W-1, LCD_H-1,BLACK);
		        break;
		    case TEMP :
				LCD_DrawRectangle(1, LCD_H/16*3, LCD_W/16*5, LCD_H/16*4+3,BLACK);
		        break;
		    case BLOOD :
				LCD_DrawRectangle(1, LCD_H/16*5, LCD_W/16*5, LCD_H/16*6+3,BLACK);
		        break;
		    case HEART :
				LCD_DrawRectangle(0, LCD_H/16*7, LCD_W/16*5, LCD_H/16*8+3,BLACK);
		        break;
		    case LAT :
				LCD_DrawRectangle(0, LCD_H/16*9, LCD_W/16*8, LCD_H/16*10+3,BLACK);
		        break;
		    case LON :
				LCD_DrawRectangle(0, LCD_H/16*11, LCD_W/16*8, LCD_H/16*12+3,BLACK);
		        break;
		    case FALLD :
				LCD_DrawRectangle(0, LCD_H/16*13, LCD_W/16*5, LCD_H/16*14+3,BLACK);
		        break;
		    case BLUEM :
				//LCD_DrawRectangle(LCD_W/16*10, 0,LCD_W/16*13,LCD_H/16*2,BLACK);
				LCD_DrawRectangle(LCD_W/16*10, 0,LCD_W/16*13,LCD_H/16*2,LIGHTBLUE);
				LCD_DrawRectangle(LCD_W/16*10+1, 1,LCD_W/16*13-1,LCD_H/16*2-1,LIGHTBLUE);
				LCD_DrawRectangle(LCD_W/16*10+2, 2,LCD_W/16*13-2,LCD_H/16*2-2,LIGHTBLUE);
		        break;
		    case POWER :
				LCD_DrawRectangle(LCD_W/16*13, 0,LCD_W,LCD_H/16*2,MAGENTA);
				LCD_DrawRectangle(LCD_W/16*13+1, 1,LCD_W-1,LCD_H/16*2-1,MAGENTA);
				LCD_DrawRectangle(LCD_W/16*13+2, 2,LCD_W-2,LCD_H/16*2-2,MAGENTA);
		        break;
		    default :
		    	LCD_DrawRectangle(1, LCD_H/16*14+8, LCD_W-1, LCD_H-1,BLACK);
		    	break;
		}

		if(switch_value == 1)
		{
			selection++;
		}
		else if(switch_value == 3)
		{
			selection--;
		}

		if(selection > 9)
		{
			selection = 1;
		}
		else if(selection < 1)
		{
			selection = 9;
		}

		switch(selection)
		{
		case SETTING :
			LCD_DrawRectangle(1, LCD_H/16*14+8, LCD_W-1, LCD_H-1,WHITE);
	        break;
	    case TEMP :
			LCD_DrawRectangle(1, LCD_H/16*3, LCD_W/16*5, LCD_H/16*4+3,WHITE);
	        break;
	    case BLOOD :
			LCD_DrawRectangle(1, LCD_H/16*5, LCD_W/16*5, LCD_H/16*6+3,WHITE);
	        break;
	    case HEART :
			LCD_DrawRectangle(0, LCD_H/16*7, LCD_W/16*5, LCD_H/16*8+3,WHITE);
	        break;
	    case LAT :
			LCD_DrawRectangle(0, LCD_H/16*9, LCD_W/16*8, LCD_H/16*10+3,WHITE);
	        break;
	    case LON :
			LCD_DrawRectangle(0, LCD_H/16*11, LCD_W/16*8, LCD_H/16*12+3,WHITE);
	        break;
	    case FALLD :
			LCD_DrawRectangle(0, LCD_H/16*13, LCD_W/16*5, LCD_H/16*14+3,WHITE);
	        break;
	    case BLUEM :
			//LCD_DrawRectangle(LCD_W/16*10, 0,LCD_W/16*13,LCD_H/16*2,RED);
			LCD_DrawRectangle(LCD_W/16*10, 0,LCD_W/16*13,LCD_H/16*2,RED);
			LCD_DrawRectangle(LCD_W/16*10+1, 1,LCD_W/16*13-1,LCD_H/16*2-1,RED);
			LCD_DrawRectangle(LCD_W/16*10+2, 2,LCD_W/16*13-2,LCD_H/16*2-2,RED);
	        break;
	    case POWER :
			LCD_DrawRectangle(LCD_W/16*13, 0,LCD_W,LCD_H/16*2,RED);
			LCD_DrawRectangle(LCD_W/16*13+1, 1,LCD_W-1,LCD_H/16*2-1,RED);
			LCD_DrawRectangle(LCD_W/16*13+2, 2,LCD_W-2,LCD_H/16*2-2,RED);

	        break;
	    default :
	    	LCD_DrawRectangle(1, LCD_H/16*14+8, LCD_W-1, LCD_H-1,WHITE);
	    	break;
		}
		return 100;
	}
	else if (switch_value == 2)
	{
		return selection;
	}
	else
	{
		return 100;
	}
}




unsigned char SETTING_SELECTION(unsigned char select1, unsigned char select2, unsigned char switch_value)
{

	if((select1 == 1) && (select2 == 1))
	{
		LCD_DrawRectangle(LCD_W/16*12,LCD_H/16*2, LCD_W/16*15+5, LCD_H/16*3+3, BLACK); //SI
		LCD_DrawRectangle(LCD_W/16*8,LCD_H/16*4, LCD_W/16*11, LCD_H/16*5+3, BLACK); //BLUEON
		LCD_DrawRectangle(LCD_W/16*12,LCD_H/16*4, LCD_W/16*15+5, LCD_H/16*5, BLACK); //BLUEOFF
		LCD_DrawRectangle(LCD_W/16*8,LCD_H/16*2, LCD_W/16*11, LCD_H/16*3+3, WHITE); //US
	}
	else if((select1 == 1) && (select2 == 2))
	{
		LCD_DrawRectangle(LCD_W/16*8,LCD_H/16*2, LCD_W/16*11, LCD_H/16*3+3, BLACK); //US
		LCD_DrawRectangle(LCD_W/16*12,LCD_H/16*2, LCD_W/16*15+5, LCD_H/16*3+3, WHITE); //SI
		LCD_DrawRectangle(LCD_W/16*8,LCD_H/16*4, LCD_W/16*11, LCD_H/16*5+3, BLACK); //BLUEON
		LCD_DrawRectangle(LCD_W/16*12,LCD_H/16*4, LCD_W/16*15+5, LCD_H/16*5+3, BLACK); //BLUEOFF
	}
	else if((select1 == 2) && (select2 == 1))
	{
		LCD_DrawRectangle(LCD_W/16*8,LCD_H/16*2, LCD_W/16*11, LCD_H/16*3+3, BLACK); //US
		LCD_DrawRectangle(LCD_W/16*12,LCD_H/16*2, LCD_W/16*15+5, LCD_H/16*3+3, BLACK); //SI
		LCD_DrawRectangle(LCD_W/16*8,LCD_H/16*4, LCD_W/16*11, LCD_H/16*5+3, WHITE); //BLUEON
		LCD_DrawRectangle(LCD_W/16*12,LCD_H/16*4, LCD_W/16*15+5, LCD_H/16*5+3, BLACK); //BLUEOFF
	}
	else if((select1 == 2) && (select2 == 2))
	{
		LCD_DrawRectangle(LCD_W/16*8,LCD_H/16*2, LCD_W/16*11, LCD_H/16*3+3, BLACK); //US
		LCD_DrawRectangle(LCD_W/16*12,LCD_H/16*2, LCD_W/16*15+5, LCD_H/16*3+3, BLACK); //SI
		LCD_DrawRectangle(LCD_W/16*8,LCD_H/16*4, LCD_W/16*11, LCD_H/16*5+3, BLACK); //BLUEON
		LCD_DrawRectangle(LCD_W/16*12,LCD_H/16*4, LCD_W/16*15+5, LCD_H/16*5+3, WHITE); //BLUEOFF
	}

	if(switch_value == 2)
	{
		if((select1 == 1) && (select2 == 1))
		{
			unit = 1;//US unit
		}
		else if((select1 == 1) && (select2 == 2))
		{
			unit = 0;//SI unit
		}
		else if((select1 == 2) && (select2 == 1))
		{
			//blueOn = 1; //BLUEON
			//HAL_GPIO_WritePin(BLU_GPIO_SW_GPIO_Port, BLU_GPIO_SW_Pin, GPIO_PIN_SET);
		}
		else if((select1 == 2) && (select2 == 2))
		{
			//blueOn = 0; //BLUEOFF
			//HAL_GPIO_WritePin(BLU_GPIO_SW_GPIO_Port, BLU_GPIO_SW_Pin, GPIO_PIN_RESET);
		}

		Refresh();
		return 100;
	}
	else
	{
		return SETTING;
	}


}







