#include "LED.h"

// void LED_Init(void)
// {
// 	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

// 	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
// 	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
// 	GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
// 	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
// 	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
// 	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
// 	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
// 	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
// }

// void LED_ON(void)
// {
// 	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
// }

// void LED_OFF(void)
// {
// 	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
// }

// void LED_Reveral(void)
// {
// 	LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
// }

void LED1_RGB_Init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
	LL_GPIO_ResetOutputPin(GPIOE, LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void LED1_RGB_ON(uint8_t Color)
{
	uint32_t Port_Read = 0;
	Port_Read = LL_GPIO_ReadOutputPort(GPIOE);
	Port_Read &= ~(0x07 << 13);
	Port_Read |= (Color << 13);
	LL_GPIO_WriteOutputPort(GPIOE, Port_Read);
}
void LED1_RGB_OFF(void)
{
	uint32_t Port_Read = 0;
	Port_Read = LL_GPIO_ReadOutputPort(GPIOE);
	Port_Read &= ~(0x07 << 13);
	Port_Read |= (0x07 << 13);
	LL_GPIO_WriteOutputPort(GPIOE, Port_Read);
}