/*
 * IIC.h
 *
 *  Created on: Dec 1, 2021
 *      Author: augus
 */

#ifndef INC_IIC_H_
#define INC_IIC_H_

#define IIC_SCL_PORT              GPIOB
#define IIC_SCL_PIN               GPIO_PIN_10

#define IIC_SDA_PORT              GPIOB
#define IIC_SDA_PIN               GPIO_PIN_3

void IIC_Delay(void);
void IIC_Init(void);
void SDA_IN(void);
void SDA_OUT(void);
void IIC_Start(void);
void IIC_Stop(void);
uint8_t  IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(uint8_t  txd);
uint8_t  IIC_Read_Byte(unsigned char ack);

#endif /* INC_IIC_H_ */
