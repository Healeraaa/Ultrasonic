#ifndef __LED_H__
#define __LED_H__

#include "main.h"

// void LED_Init(void);
// void LED_ON(void);
// void LED_OFF(void);
// void LED_Reveral(void);

void LED1_RGB_Init(void);
void LED1_RGB_ON(uint8_t Color);
void LED1_RGB_OFF(void);

#define RGB_RED 0x06
#define RGB_GREE 0x05
#define RGB_BLUE 0x03
#define RGB_CYAN 0x01
#define RGB_PURPLE 0x02
#define RGB_YELLOW 0x04
#define RGB_WHITE 0x00



#endif
