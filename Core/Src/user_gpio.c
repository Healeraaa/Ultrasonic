#include "user_gpio.h"

void User_GPIOC1_Init(void) // PC13
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
void User_GPIOA1_Init(void) // PA10
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 1. 使能SYSCFG时钟（关键！EXTI配置依赖此时钟）
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);

    // 2. 使能GPIOA时钟
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

    // 3. 配置PA10为输入模式
    GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;      // 普通输入模式
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;         // 无上拉/下拉
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH; // 可选，根据需求设置
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 4. 配置EXTI中断源映射（将PA10映射到EXTI10）
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE10);

    // 5. 配置EXTI10为下降沿触发
    LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
    EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_10;     // 选择EXTI线10
    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;          // 启用中断模式（非事件模式）
    EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING; // 下降沿触发
    EXTI_InitStruct.LineCommand = ENABLE;             // 使能EXTI线
    LL_EXTI_Init(&EXTI_InitStruct);

    // 6. 配置NVIC中断优先级和使能
    NVIC_SetPriority(EXTI15_10_IRQn, 0);    // 设置优先级
    NVIC_EnableIRQ(EXTI15_10_IRQn);         // 使能中断
}

