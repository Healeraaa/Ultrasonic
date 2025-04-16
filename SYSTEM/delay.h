#ifndef __DELAY_H
#define __DELAY_H
// #include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_bus.h"

void delay_init(uint16_t sysclk);           
void delay_ms(uint16_t nms);                
void delay_us(uint32_t nus);               

#endif

