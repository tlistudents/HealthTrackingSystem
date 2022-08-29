/*
 * display.h
 *
 *  Created on: Sep 16, 2021
 *      Author: Tingzhang Li
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_
#include "lcd.h"

void MENU_init_(void);

void MENU_init(void);

void MENU_main(void);

void MENU_SETTING(void);

void MENU_TEMP(void);

void MENU_BLOOD(void);

void MENU_HEART(void);

void MENU_LAT(void);

void MENU_LON(void);

void MENU_FALL(void);

void MENU_BLUET(void);

void MENU_GPS(void);

void Refresh(void);

unsigned char MENU_SELECTION(unsigned char switch_value);

unsigned char SETTING_SELECTION(unsigned char select1, unsigned char select2, unsigned char switch_value);


//void MENU_clock(void );

extern char Total_cycle; //Global variables added to the loop every time the main loop is used to create a delay
extern unsigned char selection;
extern unsigned char selectedFlag;




#endif /* INC_DISPLAY_H_ */
