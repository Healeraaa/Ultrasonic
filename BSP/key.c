#include "key.h"
#include "FreeRTOS.h"
#include "task.h"

void Key_Init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* 扩展板上按键，使用核心板时请忽略 */
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_1 | LL_GPIO_PIN_3 | LL_GPIO_PIN_5 | LL_GPIO_PIN_7;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/**
 * 函    数：按键获取键码
 * 参    数：mode:0不支持长按；1支持长按。
 * 返 回 值：按下按键的键码值，范围：0~1，返回0代表没有按键按下
 */

uint8_t Key_GetNum(uint8_t mode)
{
	static uint8_t lastKeyNum = 0; // 记录上一次的键码
	uint8_t KeyNum = 0;

	// 读取按键1
	if (KEY0 == 0)
	{
		vTaskDelay(20); // 消抖延时
		if (KEY0 == 0)
		{ // 再次确认按键状态
			KeyNum = KEY0_PRES;
		}
	}
	else if (KEY1 == 0)
	{
		vTaskDelay(20); // 消抖延时
		if (KEY1 == 0)
		{ // 再次确认按键状态
			KeyNum = KEY1_PRES;
		}
	}
	else if (KEY2 == 0)
	{
		vTaskDelay(20); // 消抖延时
		if (KEY2 == 0)
		{ // 再次确认按键状态
			KeyNum = KEY2_PRES;
		}
	}
	else if (KEY3 == 0)
	{
		vTaskDelay(20); // 消抖延时
		if (KEY3 == 0)
		{ // 再次确认按键状态
			KeyNum = KEY3_PRES;
		}
	}
	else if (KEY4 == 0)
	{
		vTaskDelay(20); // 消抖延时
		if (KEY4 == 0)
		{ // 再次确认按键状态
			KeyNum = KEY4_PRES;
		}
	}
	else
	{
		KeyNum = 0xFF;
	}

	// 根据 mode 参数选择工作模式
	if (mode == 1)
	{
		// 连续按下模式：每次按键按下时直接返回键码
		return KeyNum;
	}
	else
	{
		// 非连续按下模式：只有在按键状态发生变化时返回键码
		if (KeyNum != lastKeyNum)
		{
			lastKeyNum = KeyNum;
			return KeyNum;
		}
	}

	return 0; // 未检测到新按键时返回0
}
