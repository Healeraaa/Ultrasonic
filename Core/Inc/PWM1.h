#ifndef __PWM1_H__
#define __PWM1_H__

#include "main.h"

void PWM1_Init(void);
void PWM1_SetDutyCycle(uint8_t duty_cycle);
void PWM1_Disable(void);
void PWM1_Enable(void);


#endif 

