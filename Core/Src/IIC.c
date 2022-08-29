/*
 * IIC.c
 *
 *  Created on: Dec 1, 2021
 *      Author: augus
 */


#include <STDINT.H>
#include "IIC.H"
#include "main.h"  // Device header
//#define IIC_SCL_PORT              GPIOB
//#define IIC_SCL_PIN               GPIO_PIN_6

//#define IIC_SDA_PORT              GPIOB
//#define IIC_SDA_PIN               GPIO_PIN_5

void delay_us(int32_t nus)
{
    uint32_t i;
    for(i=0;i<nus;i++)
    {

    }
}


void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = IIC_SCL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(IIC_SCL_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = IIC_SDA_PIN;
    HAL_GPIO_Init(IIC_SDA_PORT, &GPIO_InitStruct);


   HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_SET);
   HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,GPIO_PIN_SET);

}


void SDA_IN(void)
 {

     GPIO_InitTypeDef GPIO_InitStruct;
     GPIO_InitStruct.Pin = IIC_SDA_PIN;
     GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
     //GPIO_InitStruct.Pull = GPIO_PULLUP;
     GPIO_InitStruct.Pull = GPIO_NOPULL;
     HAL_GPIO_Init(IIC_SCL_PORT, &GPIO_InitStruct);
 }


void SDA_OUT(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
     GPIO_InitStruct.Pin = IIC_SDA_PIN;
    // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD ;
     GPIO_InitStruct.Pull = GPIO_PULLUP;

     HAL_GPIO_Init(IIC_SCL_PORT, &GPIO_InitStruct);
}


void IIC_Start(void)
{
    SDA_OUT();
    HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,GPIO_PIN_SET);
    HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_SET);
    delay_us(200);
    HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,GPIO_PIN_RESET);
    delay_us(200);
    HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_RESET);

}


void IIC_Stop(void)
{
   SDA_OUT();
   HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_RESET);
   HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,GPIO_PIN_RESET);
   delay_us(200);
   HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_SET);
   //IIC_Delay();
   HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,GPIO_PIN_SET);
   delay_us(200);
}


uint8_t  IIC_Wait_Ack(void)
{
  uint32_t i=250;
  SDA_IN();
  HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,GPIO_PIN_SET);
  delay_us(50);
  HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_SET);
  delay_us(50);

  while(i--){
            if(HAL_GPIO_ReadPin(IIC_SDA_PORT, IIC_SDA_PIN)==1)
            {
            	HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_RESET);
            	delay_us(200);
                HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_SET);
                delay_us(200);
             }
            else
                goto loop;

            }
loop:
 // Clr_IIC_SCL;
  HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_RESET);
  //delay_us(200);
  return 0;
}


void IIC_Ack(void)
{
    HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_RESET);
    SDA_OUT();
    HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,GPIO_PIN_RESET);
    delay_us(100);
    HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_SET);
    delay_us(100);
    HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_RESET);

}


void IIC_NAck(void)
{
    HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_RESET);
    SDA_OUT();
    HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,GPIO_PIN_SET);
    delay_us(100);
    HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_SET);
    delay_us(100);
    HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_RESET);
}


void IIC_Send_Byte(uint8_t txd)
{
   uint8_t t;
   SDA_OUT();
   for(t=0;t<8;t++)
    {
        //HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_RESET);
        //delay_us(100);
        if (txd&0x80)
        {
        	HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,GPIO_PIN_SET);
        }
        else
        {
        	HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,GPIO_PIN_RESET);
        }
        delay_us(100);
        HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_SET);
        delay_us(100);
        HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_RESET);
        if(t==7)
        {
        	HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,GPIO_PIN_SET);
        }
        txd<<=1;
        delay_us(100);

     }
   	 //HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_RESET);
}


uint8_t  IIC_Read_Byte(unsigned char ack)
{
    uint8_t i,receive=0;
    SDA_IN();
    for(i=0;i<8;i++ )
    {
        receive<<=1;
    //    Clr_IIC_SCL;
        HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_RESET);
        delay_us(100);
    //    Set_IIC_SCL;
        HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_SET);

        delay_us(100);

        if(HAL_GPIO_ReadPin(IIC_SDA_PORT, IIC_SDA_PIN)==GPIO_PIN_SET)
        {
            receive |= 0x01;
        }
        else
        {
            receive &= 0xfe;
        }
    }

    //HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_SET);
    if (!ack)
    {
    	IIC_NAck();//发送nACK
    }
    else
    {
    	IIC_Ack(); //发送ACK
    }
    return receive;
}



