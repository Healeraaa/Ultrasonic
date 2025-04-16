#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"

// KEY1
#define KEY_PORT_0 GPIOA
#define KEY0_PIN GPIO_PIN_0
#define KEY0 LL_GPIO_IsInputPinSet(KEY_PORT_0, KEY0_PIN)
#define KEY0_PRES 0

#define KEY_PORT_1 GPIOD

#define KEY1_PIN GPIO_PIN_1
#define KEY1 LL_GPIO_IsInputPinSet(KEY_PORT_1, KEY1_PIN)
#define KEY1_PRES 1

#define KEY2_PIN GPIO_PIN_3
#define KEY2 LL_GPIO_IsInputPinSet(KEY_PORT_1, KEY2_PIN)
#define KEY2_PRES 2

#define KEY3_PIN GPIO_PIN_5
#define KEY3 LL_GPIO_IsInputPinSet(KEY_PORT_1, KEY3_PIN)
#define KEY3_PRES 3

#define KEY4_PIN GPIO_PIN_7
#define KEY4 LL_GPIO_IsInputPinSet(KEY_PORT_1, KEY4_PIN)
#define KEY4_PRES 4

void Key_Init(void);
uint8_t Key_GetNum(uint8_t mode);

#endif
