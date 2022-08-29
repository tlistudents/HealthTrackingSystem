#include "lcd.h"
#include "stdlib.h"
#include "stdio.h"
#include "lcdfont.h"
/*****************************************************************************
The beggning code is provided by the manufacturer
http://www.lcdwiki.com/2.2inch_SPI_Module_ILI9341_SKU:MSP2202
******************************************************************************/

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;
u16 DeviceCode;

extern SPI_HandleTypeDef hspi1;
/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u8 data)
{
   LCD_CS_CLR;
   LCD_RS_CLR;
   SPI_WriteByte(data);
   LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u8 data)
{
   LCD_CS_CLR;
   LCD_RS_SET;
   SPI_WriteByte(data);
   LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/
void Lcd_WriteData_16Bit(u16 Data)
{
   LCD_CS_CLR;
   LCD_RS_SET;
   SPI_WriteByte(Data>>8);
   SPI_WriteByte(Data);
   LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y, u16 color)
{
	LCD_SetCursor(x,y);//设置光标位置
	Lcd_WriteData_16Bit(color);
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/
void LCD_Clear(u16 Color)
{
  unsigned int i,m;
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);
	LCD_CS_CLR;
	LCD_RS_SET;
	for(i=0;i<lcddev.height;i++)
	{
    for(m=0;m<lcddev.width;m++)
    {
			Lcd_WriteData_16Bit(Color);
		}
	}
	 LCD_CS_SET;
}


/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void LCD_RESET(void)
{
	LCD_RST_CLR;
	HAL_Delay(100);
	LCD_RST_SET;
	HAL_Delay(50);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void LCD_Init(void)
{

 	LCD_RESET(); //LCD 复位
//*************2.2inch ILI9341初始化**********//
	LCD_WR_REG(0xB1);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x2D);
	LCD_WR_REG(0xB2);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x2D);
	LCD_WR_REG(0xB3);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x2D);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x2D);
	LCD_WR_REG(0xB4); //Column inversion
	LCD_WR_DATA(0x07);
	//ST7735R Power Sequence
	LCD_WR_REG(0xC0);
	LCD_WR_DATA(0xA2);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x84);
	LCD_WR_REG(0xC1);
	LCD_WR_DATA(0xC5);
	LCD_WR_REG(0xC2);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xC3);
	LCD_WR_DATA(0x8A);
	LCD_WR_DATA(0x2A);
	LCD_WR_REG(0xC4);
	LCD_WR_DATA(0x8A);
	LCD_WR_DATA(0xEE);
	LCD_WR_REG(0xC5); //VCOM
	LCD_WR_DATA(0x0E);
	LCD_WR_REG(0x36); //MX, MY, RGB mode
	LCD_WR_DATA(0xC0);
	//ST7735R Gamma Sequence
	LCD_WR_REG(0xe0);
	LCD_WR_DATA(0x0f);
	LCD_WR_DATA(0x1a);
	LCD_WR_DATA(0x0f);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x2f);
	LCD_WR_DATA(0x28);
	LCD_WR_DATA(0x20);
	LCD_WR_DATA(0x22);
	LCD_WR_DATA(0x1f);
	LCD_WR_DATA(0x1b);
	LCD_WR_DATA(0x23);
	LCD_WR_DATA(0x37);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x10);
	LCD_WR_REG(0xe1);
	LCD_WR_DATA(0x0f);
	LCD_WR_DATA(0x1b);
	LCD_WR_DATA(0x0f);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x33);
	LCD_WR_DATA(0x2c);
	LCD_WR_DATA(0x29);
	LCD_WR_DATA(0x2e);
	LCD_WR_DATA(0x30);
	LCD_WR_DATA(0x30);
	LCD_WR_DATA(0x39);
	LCD_WR_DATA(0x3f);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x10);
	LCD_WR_REG(0x2a);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x7f);
	LCD_WR_REG(0x2b);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x9f);
	LCD_WR_REG(0xF0); //Enable test command
	LCD_WR_DATA(0x01);
	LCD_WR_REG(0xF6); //Disable ram power save mode
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0x3A); //65k mode
	LCD_WR_DATA(0x05);


	LCD_WR_REG(0x11); //Exit Sleep
	HAL_Delay(120);
	LCD_WR_REG(0x29); //display on

    LCD_direction(USE_HORIZONTAL);//set LCD direction
	LCD_LED_SET;//点亮背光
	LCD_Clear(WHITE);//清全屏白色
}

/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{
	LCD_WR_REG(lcddev.setxcmd);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(xStar);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(xEnd);

	LCD_WR_REG(lcddev.setycmd);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(yStar);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(yEnd);

	LCD_WriteRAM_Prepare();	//start to write to GRAM
}

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);
}

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/
void LCD_direction(u8 direction)
{
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;
	lcddev.wramcmd=0x2C;
switch(direction){
case 0:
	lcddev.width=LCD_W;
	lcddev.height=LCD_H;
	LCD_WriteReg(0x36,(0<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
break;
case 1:
	lcddev.width=LCD_H;
	lcddev.height=LCD_W;
	LCD_WriteReg(0x36,(0<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
break;
case 2:
	lcddev.width=LCD_W;
	lcddev.height=LCD_H;
	LCD_WriteReg(0x36,(0<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
break;
case 3:
	lcddev.width=LCD_H;
	lcddev.height=LCD_W;
	LCD_WriteReg(0x36,(0<<3)|(0<<7)|(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
break;
default:break;
}
}


// use to wrtie a byte of data to display
u8 SPI_WriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&hspi1,&TxData,&Rxdata,1, 1000);
 	return Rxdata;
}

/******************************************************************************
      function：draw line
      input：x1,y1   begins
                x2,y2   ends
                color   line color
      return：  n/a
******************************************************************************/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u16 t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1;
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1;
	else if (delta_x==0)incx=0;
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x;
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

/******************************************************************************
      function：draw rectangle
      input：x1,y1   starts
                x2,y2   ends
                color   color
      return：  n/a
******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}

/******************************************************************************
      function：fill in color in some area
      input：xsta,ysta   start
             xend,yend   end
			color       color to fill in
      return：  n/a
******************************************************************************/
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{
	u16 i,j;
	u16 width=ex-sx+1; 		//得到填充的宽度
	u16 height=ey-sy+1;		//高度
	LCD_SetWindows(sx,sy,ex,ey);//设置显示窗口
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		Lcd_WriteData_16Bit(color);	//写入数据
	}
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口设置为全屏
}


/******************************************************************************
      function：draw circle
      input：x0,y0   center
             r       radius
             color   color
      return：  n/a
******************************************************************************/
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	a=0;b=r;
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3
		LCD_DrawPoint(x0+b,y0-a,color);             //0
		LCD_DrawPoint(x0-a,y0+b,color);             //1
		LCD_DrawPoint(x0-a,y0-b,color);             //2
		LCD_DrawPoint(x0+b,y0+a,color);             //4
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//determine if the radius is too large
		{
			b--;
		}
	}
}


/******************************************************************************
      function：display single char
      input：x,y	begins coordinate
                num char to display
                fc font color
                bc background color
                sizey font siez-8 or 16 or 32
                mode:  0-nonoverlay  1-Overlay
      retun：  n/a
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
    u8 temp;
    u8 pos,t;
	u16 colortemp=POINT_COLOR;

	num=num-' ';//得到偏移后的值
	LCD_SetWindows(x,y,x+sizey/2-1,y+sizey-1);//设置单个文字显示窗口
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<sizey;pos++)
		{
			if(sizey==12)temp=asc2_1206[num][pos];//调用1206字体 //asc2_1206
			else temp=asc2_1608[num][pos];		 //调用1608字体 //asc2_1608
			for(t=0;t<sizey/2;t++)
		    {
		        if(temp&0x01)Lcd_WriteData_16Bit(fc);
				else Lcd_WriteData_16Bit(bc);
				temp>>=1;

		    }

		}
	}else//叠加方式
	{
		for(pos=0;pos<sizey;pos++)
		{
			if(sizey==12)temp=asc2_1206[num][pos];//调用1206字体
			else temp=asc2_1608[num][pos];		 //调用1608字体
			for(t=0;t<sizey/2;t++)
		    {
				POINT_COLOR=fc;
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos,fc);//画一个点
		        temp>>=1;
		    }
		}
	}
	POINT_COLOR=colortemp;
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏
}


/******************************************************************************
      function：display string
      input：x,y	begins coordinate
                *p string to display
                fc font color
                bc background color
                sizey font siez-8 or 16 or 32
                mode:  0-nonoverlay  1-Overlay
      retun：  n/a
******************************************************************************/
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	while((*p<='~')&&(*p>=' '))
	{
		if(x>(lcddev.width-1)||y>(lcddev.height-1))
		return;
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}
}



/******************************************************************************
      function：display image
      input：x,y beinges coordinate
                length length of image
                width  width of image
                pic[]  image array
      return：  n/a
******************************************************************************/
void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[])
{
	u16 i,j,k=0;
	LCD_SetWindows(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_WR_DATA(pic[k*2]);
			LCD_WR_DATA(pic[k*2+1]);
			k++;
		}
	}
}

/******************************************************************************
      function：display integer variable(V1)
      input：x,y begins coordinate
               var variable to display
               var1 array to store the string
      return：  n/a
******************************************************************************/
void LCD_ShowIntVar(u16 x,u16 y,int var,u8* var1, u16 fc,u16 bc,u8 sizey)
{
	sprintf(var1, "%d", var);
	LCD_ShowString(x,y,var1,fc,bc,sizey,1);
}

/******************************************************************************
      function：display float variable(V1)
      input：x,y begins coordinate
               var variable to display
               var1 array to store the string
      n/a：  n/a
******************************************************************************/
void LCD_ShowFloatVar(u16 x,u16 y,float var,u8* var1, u16 fc,u16 bc,u8 sizey)
{
	sprintf(var1, "%.2f", var);
	LCD_ShowString(x,y,var1,fc,bc,sizey,1);
}




/******************************************************************************
      function：display chinese string(mainly used for degree unit)
      input：x,y begins coordinate
                *s-chinese to display
                fc-font color
                bc-font background color
                sizey font sieze from 16 24 32
                mode:  0-nonoverlay  1-Overlay
      return：  n/a
******************************************************************************/
void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	while(*s!=0)
	{
		if(sizey==16) LCD_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
		else if(sizey==24) LCD_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
		else if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
		else return;
		s+=2;
		x+=sizey;
	}
}


/******************************************************************************
      function：display single 16x16 chinese character
      input：x,y begins coordinate
                *s-chinese to display
                fc-font color
                bc-font background color
                sizey font sieze from 16 24 32
                mode:  0-nonoverlay  1-Overlay
      return：  n/a
******************************************************************************/
void LCD_ShowChinese16x16(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;//size
	u16 TypefaceNum;
	u16 x0=x;
	TypefaceNum=sizey/8*sizey;

	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);
	for(k=0;k<HZnum;k++)
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{
			LCD_SetWindows(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{
					if(!mode)//nonoverlay
					{
						if(tfont16[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
					}
					else//overaly
					{
						if(tfont16[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}
		continue;  //quit after find the correct charater
	}
}


/******************************************************************************
      function：display single 24x24 chinese character
      input：x,y begins coordinate
                *s-chinese to display
                fc-font color
                bc-font background color
                sizey font sieze from 16 24 32
                mode:  0-nonoverlay  1-Overlay
      return：  n/a
******************************************************************************/
void LCD_ShowChinese24x24(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;
	u16 TypefaceNum;
	u16 x0=x;
	TypefaceNum=sizey/8*sizey;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);
	for(k=0;k<HZnum;k++)
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{
			LCD_SetWindows(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{
					if(!mode)
					{
						if(tfont24[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
					}
					else
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}
		continue;  //quit after find the correct charater
	}
}

/******************************************************************************
      function：display single 32x32 chinese character
      input：x,y begins coordinate
                *s-chinese to display
                fc-font color
                bc-font background color
                sizey font sieze from 16 24 32
                mode:  0-nonoverlay  1-Overlay
      return：  n/a
******************************************************************************/
void LCD_ShowChinese32x32(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;
	u16 TypefaceNum;
	u16 x0=x;
	TypefaceNum=sizey/8*sizey;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);
	for(k=0;k<HZnum;k++)
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{
			LCD_SetWindows(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{
					if(!mode)
					{
						if(tfont32[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
					}
					else
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}
		continue;  //quit after find the correct charater
	}
}


