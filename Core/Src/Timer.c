#include "Timer.h"
#include "Ultrasonic.h"




/*
 * @brief  初始化基础定时器 TIM6
 */
void TIM6_Init(void)
{
    LL_TIM_InitTypeDef TIM_InitStruct = {0};
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6); // 使能 TIM6 时钟
    // 配置 TIM6 中断优先级为 5，子优先级为 0
    NVIC_SetPriority(TIM6_DAC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 6, 0));
    // 使能 TIM6 中断
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    // 配置定时器
    TIM_InitStruct.Prescaler = 8400-1;                        // 设置预分频器为 （84 MHz / 8400 = 10 KHz）
    TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;     // 设置计数模式为向上计数
    TIM_InitStruct.Autoreload = 50-1;                      // 设置自动重装载值为 (104-1)  104 时触发中断（每秒中断一次）
    LL_TIM_Init(TIM6, &TIM_InitStruct);                     // 应用配置

    // 禁用自动重装载预加载功能（ARR 修改立即生效）
    LL_TIM_DisableARRPreload(TIM6);   
    // 设置触发输出为复位（不产生触发信号）
    LL_TIM_SetTriggerOutput(TIM6, LL_TIM_TRGO_RESET);
    // 禁用主从模式（独立运行）
    LL_TIM_DisableMasterSlaveMode(TIM6);
    // 使能 TIM6 更新中断
    LL_TIM_EnableIT_UPDATE(TIM6); // 开启更新中断
    // 启动定时器
    LL_TIM_EnableCounter(TIM6); // 使能定时器计数器
}

/*
 * @brief  初始化基础定时器 TIM7
 */
void TIM7_Init(void)
{
    LL_TIM_InitTypeDef TIM_InitStruct = {0};
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM7);
    // 配置 TIM7 中断优先级为 5，子优先级为 0
    NVIC_SetPriority(TIM7_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 4, 0));
    // 使能 TIM7 中断
    NVIC_EnableIRQ(TIM7_IRQn);

    TIM_InitStruct.Prescaler = 8400 - 1;                // 设置预分频器为 8399（84 MHz / 8400 = 10 kHz）
    TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP; // 设置计数模式为向上计数
    TIM_InitStruct.Autoreload = 10000 - 1;               // 设置自动重装载值为 9999，计数到 10000 时触发中断（每秒中断一次）
    LL_TIM_Init(TIM7, &TIM_InitStruct);                 // 应用配置

    // 禁用自动重装载预加载功能（ARR 修改立即生效）
    LL_TIM_DisableARRPreload(TIM7);
    // 设置触发输出为复位（不产生触发信号）
    LL_TIM_SetTriggerOutput(TIM7, LL_TIM_TRGO_RESET);
    // 禁用主从模式（独立运行）
    LL_TIM_DisableMasterSlaveMode(TIM7);
    // 使能 TIM6 更新中断
    LL_TIM_EnableIT_UPDATE(TIM7); // 开启更新中断
                                  // 启动定时器
    LL_TIM_EnableCounter(TIM7); // 使能定时器计数器
}

void TIM6_DAC_IRQHandler(void)
{
    if (LL_TIM_IsActiveFlag_UPDATE(TIM6)) // 检查更新中断标志
    {
        Ultrasonic_Send_Task();
        LL_TIM_ClearFlag_UPDATE(TIM6); // 清除更新中断标志
    }
}
void TIM7_IRQHandler(void)
{
    if (LL_TIM_IsActiveFlag_UPDATE(TIM7)) // 检查更新中断标志
    {
        printf("TIM7正在运行,优先级为:4\r\n");
        LL_TIM_ClearFlag_UPDATE(TIM7); // 清除更新中断标志
    }
}

