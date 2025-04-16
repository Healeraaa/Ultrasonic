#include "delay.h"

static uint32_t g_fac_us = 0; // 微秒延时因子

void delay_init(uint16_t sysclk)
{
    sysclk /=2;//HCLK二分频到APB1 Timer clocks
    TIM_HandleTypeDef htim4;
    
    __HAL_RCC_TIM4_CLK_ENABLE(); // 启用TIM4时钟

    htim4.Instance = TIM4;
    htim4.Init.Prescaler = sysclk - 1; // 定时器频率为 1 MHz（1 us 分辨率）
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.Period = 0xFFFF; // 最大计数值
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&htim4);

    HAL_TIM_Base_Start(&htim4); // 启动TIM4
    g_fac_us = sysclk;          // 保存系统时钟，用于延时计算
}


void delay_us(uint32_t nus)
{
    uint16_t start = TIM4->CNT; // 读取当前计数值
    uint32_t ticks = nus;       // 微秒对应的计数值

    while ((TIM4->CNT - start) < ticks)
    {
        if ((TIM4->CNT < start) && ((0xFFFF - start + TIM4->CNT) >= ticks))
        {
            break; // 考虑计数器溢出的情况
        }
    }
}



void delay_ms(uint16_t nms)
{
    uint32_t i;
    for (i = 0; i < nms; i++)
    {
        delay_us(1000); // 调用微秒延时函数，延时1 ms
    }
}

