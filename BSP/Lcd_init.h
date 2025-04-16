#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "main.h"

#define USE_HORIZONTAL 1 //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 280

#else
#define LCD_W 280
#define LCD_H 240
#endif




//-----------------LCD端口定义---------------- 
/*使用硬件SPI，故注释
#define SCLK_PORT			GPIOA
#define SCLK_PIN			GPIO_PIN_5

#define SDA_PORT			GPIOA
#define SDA_PIN				GPIO_PIN_7
*/

//RESET PIN
#define RES_PORT			GPIOE
#define RES_PIN			    GPIO_PIN_2

#define DC_PORT				GPIOE
#define DC_PIN				GPIO_PIN_3

#define CS_PORT				GPIOE
#define CS_PIN				GPIO_PIN_4

/* 使用PWM进行背光调节 故注释
#define BLK_PORT			GPIOB
#define BLK_PIN				GPIO_PIN_0
*/


void MyGPIO_SetPin(GPIO_TypeDef *GPIOx, uint32_t PinMask);
void MyGPIO_ReSetPin(GPIO_TypeDef *GPIOx, uint32_t PinMask);
void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Write_Bus(uint8_t data);//模拟SPI时序
void LCD_WR_DATA8(uint8_t data);//写入一个字节
void LCD_WR_DATA(uint16_t Data);//写入两个字节
void LCD_WR_REG(uint8_t data);//写入一个指令
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
void LCD_Set_Light(uint8_t dc);
void LCD_Close_Light(void);
void LCD_ST7789_SleepIn(void);
void LCD_ST7789_SleepOut(void);
void LCD_Open_Light(void);

#define LCD_RES_Reset()     MyGPIO_ReSetPin(RES_PORT, RES_PIN)//RESET
#define LCD_RES_Set()       MyGPIO_SetPin(RES_PORT, RES_PIN)

#define LCD_DC_Reset()      MyGPIO_ReSetPin(DC_PORT, DC_PIN)//DC
#define LCD_DC_Set()        MyGPIO_SetPin(DC_PORT, DC_PIN)
 		     
#define LCD_CS_Reset()      MyGPIO_ReSetPin(CS_PORT, CS_PIN)//CS
#define LCD_CS_Set()        MyGPIO_SetPin(CS_PORT, CS_PIN)



#endif
