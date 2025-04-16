#include "Ultrasonic.h"
#include "User_TasksInit.h"
#include "user_gpio.h"
#include "PWM1.h"
#include "delay.h"
uint8_t Ultrasonic_TxFlag = 0;
uint8_t Ultrasonic_RxFlag = 0;
uint8_t Ultrasonic_TxNum = 0;


void UltrasonicTX_Init(void)
{
    User_GPIOC1_Init(); // 超声波使能引脚初始化
    PWM1_Init();
    PWM1_SetDutyCycle(50);
    PWM1_Enable();
}

void UltrasonicTX_Reset(void)
{
    PWM1_Enable();
    // LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
}

void UltrasonicTX_Set(void)
{
    PWM1_Disable();
    // LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
}

uint8_t Ultrasonic_GetTxFlag(void)
{
    if (Ultrasonic_TxFlag == 1) // 如果标志位为1
    {
        Ultrasonic_TxFlag = 0;
        return 1; // 则返回1，并自动清零标志位
    }
    return 0; // 如果标志位为0，则返回0
}

void Ultrasonic_GiveTxNum(uint8_t data)
{
    Ultrasonic_TxNum = data;
    Ultrasonic_TxFlag = 1;
}

uint8_t Ultrasonic_GetTxNum(void)
{
    return Ultrasonic_TxNum;
}

void Ultrasonic_Send_Task(void)
{
    static uint8_t TxState = 0;
    static uint8_t TxData = 0;
    static uint8_t TxIndex = 0;
    if (TxState == 0) // 等待发送数据状态
    {
        if (Ultrasonic_GetTxFlag())
        {
            TxData = Ultrasonic_GetTxNum();
            TxState = 1; // 置下一个状态
        }
    }
    else if (TxState == 1) // 发送空闲位状态
    {
        UltrasonicTX_Reset();
        TxState = 2; // 置下一个状态
        TxIndex = 0; // 数据第0位开始发
    }
    else if (TxState == 2) // 1BIT数据发送状态
    {
        if (TxData & (1 << TxIndex))
        {
            UltrasonicTX_Set();
        }
        else
        {
            UltrasonicTX_Reset();
        }
        TxIndex++;
        if (TxIndex >= 8)
        {
            TxState = 3;
        }
    }
    else if (TxState == 3) // 发送停止位状态
    {
        UltrasonicTX_Set();
        TxState = 0;
    }
    delay_ms(10);
}

/* 接收端代码 */
void UltrasonicRX_Init(void)
{
    User_GPIOA1_Init();
}

uint8_t UltrasonicRX_Get(void)
{
    return (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_10));
}

uint8_t Ultrasonic_GetRxFlag(void)
{
    if (Ultrasonic_RxFlag == 1) // 如果标志位为1
    {
        Ultrasonic_RxFlag = 0;
        return 1; // 则返回1，并自动清零标志位
    }
    return 0; // 如果标志位为0，则返回0
}


// 判断是否为起始位
uint8_t Ultrasonic_Receive_State(void)
{
    uint8_t i = 0;
    uint8_t index = 0;
    for (i = 0; i < 10; i++)
    {
        if (UltrasonicRX_Get() == 0)
        {
            index++;
            delay_ms(1);
        }
    }
    if (index > 5)
        return 1;
    else
        return 0;
}

uint8_t Ultrasonic_Receive_Task(void)
{
    static uint8_t RxState = 0;
    static uint8_t RxData = 0;
    static uint8_t RxIndex = 0;

    if (Ultrasonic_Receive_State())
    {
        for (RxIndex = 0; RxIndex < 8; RxIndex++)
        {
            delay_ms(6);
            if (UltrasonicRX_Get())
            {
                RxData |= (1 << RxIndex);
            }
            else
            {
                RxData &= ~(1 << RxIndex);
            }
            delay_ms(4);
        }
        delay_ms(6);
        if(UltrasonicRX_Get() == 0)
        {
            return RxData;
        }
        else
        {
            return 0;
        }

    }
}

void EXTI15_10_IRQHandler(void)
{
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_10))
    {
        Ultrasonic_RxFlag = 1;
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_10); // 清除中断标志
        // 用户的中断处理代码
    }
}
