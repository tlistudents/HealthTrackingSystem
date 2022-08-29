/*
 * lcd.h
 *
 *  Created on: Sep 17, 2021
 *      Author: Tingzhang Li
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_
#include "main.h"

#define u32 uint32_t
#define u16 uint16_t
#define u8 uint8_t

//LCD parameter
typedef struct
{
	u16 width;			//LCD width
	u16 height;			//LCD height
	u16 id;				//LCD ID
	u8  dir;			//0，vertical；1 horzational
	u16	 wramcmd;		//start to write gram order
	u16  setxcmd;		//set x coordinate order
	u16  setycmd;		//set y coordinate order
}_lcd_dev;

//LCD parameter
extern _lcd_dev lcddev;	//mangage LCD important parameter
/////////////////////////////////////用户配置区///////////////////////////////////
#define USE_HORIZONTAL  	 0//定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转

//////////////////////////////////////////////////////////////////////////////////
//define LCD size (need to change for new display)
#define LCD_W 128
#define LCD_H 160

//TFTLCD variabele
extern u16  POINT_COLOR;//pen color
extern u16  BACK_COLOR; //background color

////////////////////////////////////////////////////////////////////

// if useing IO directily register may have faster fps?
//set pin
#define	LCD_LED_SET HAL_GPIO_WritePin(TFT_LED_TIM2_GPIO_Port, TFT_LED_TIM2_Pin, GPIO_PIN_SET) //LCD LED backlight(change TIM4 t0 TIM2 for final)
#define	LCD_CS_SET  HAL_GPIO_WritePin(TFT_GPIO__CS_GPIO_Port, TFT_GPIO__CS_Pin, GPIO_PIN_SET)//GPIO_TYPE->BSRR=1<<LCD_CS    CS Pin
#define	LCD_RS_SET	HAL_GPIO_WritePin(TFT_GPIO_DC_GPIO_Port, TFT_GPIO_DC_Pin, GPIO_PIN_SET)//GPIO_TYPE->BSRR=1<<LCD_RS    DC pin
#define	LCD_RST_SET	HAL_GPIO_WritePin(TFT_GPIO_RESET_GPIO_Port, TFT_GPIO_RESET_Pin, GPIO_PIN_SET)//GPIO_TYPE->BSRR=1<<LCD_RST   RESET Pin
//clear pin
#define	LCD_LED_CLR HAL_GPIO_WritePin(TFT_LED_TIM4_GPIO_Port, TFT_LED_TIM2_Pin, GPIO_PIN_RESET) //LCD背光
#define	LCD_CS_CLR  HAL_GPIO_WritePin(TFT_GPIO__CS_GPIO_Port, TFT_GPIO__CS_Pin, GPIO_PIN_RESET)//GPIO_TYPE->BRR=1<<LCD_CS     //片选端口  	PB11
#define	LCD_RS_CLR	HAL_GPIO_WritePin(TFT_GPIO_DC_GPIO_Port, TFT_GPIO_DC_Pin, GPIO_PIN_RESET)//GPIO_TYPE->BRR=1<<LCD_RS     //数据/命令  PB10
#define	LCD_RST_CLR	HAL_GPIO_WritePin(TFT_GPIO_RESET_GPIO_Port, TFT_GPIO_RESET_Pin, GPIO_PIN_RESET)//GPIO_TYPE->BRR=1<<LCD_RST    //复位			  PB12




//commom pen color
#define WHITE       0xFFFF
#define BLACK      	0x0000
#define BLUE       	0x001F
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40
#define BRRED 			0XFC07
#define GRAY  			0X8430
//GUI color

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN     	0X841F //浅绿色
#define LIGHTGRAY     0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 		0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE      	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE          0X2B12 //浅棕蓝色(选择条目的反色)

void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(u16 Color);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
u16  LCD_ReadPoint(u16 x,u16 y); //read point
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);
//Function to draw stuff
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);//draw line
void LCD_DrawPoint(u16 x,u16 y, u16 color); //draw ponit
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);//fill in color with specific area
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);//draw rectangle
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);//draw circle
void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode); // display single char
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode); //display string
void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[]); // show pictures
void LCD_ShowIntVar(u16 x,u16 y,int var,u8* var1, u16 fc,u16 bc,u8 sizey); //show integer variable v1
void LCD_ShowFloatVar(u16 x,u16 y,float var,u8* var1, u16 fc,u16 bc,u8 sizey); //show float variable v1
void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);
void LCD_ShowChinese16x16(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);
void LCD_ShowChinese24x24(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);
void LCD_ShowChinese32x32(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);


u16 LCD_RD_DATA(void);//read LCD data
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
void LCD_WR_DATA(u8 data);
u16 LCD_ReadReg(u8 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
u16 LCD_ReadRAM(void);
u16 LCD_BGR2RGB(u16 c);
void LCD_SetParam(void);
void Lcd_WriteData_16Bit(u16 Data);
void LCD_direction(u8 direction );

u8 SPI_WriteByte(u8 TxData);

//如果仍然觉得速度不够快，可以使用下面的宏定义,提高速度.
//注意要去掉lcd.c中void LCD_WR_DATA(u16 data)函数定义哦
/*
#if LCD_USE8BIT_MODEL==1//使用8位并行数据总线模式
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	DATAOUT(data<<8);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	}
	#else//使用16位并行数据总线模式
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	}
#endif
*/


#endif /* INC_LCD_H_ */
