#include "User_UltrasonictxTask.h"
#include "User_TasksInit.h"
#include "OLED.h"
#include "AToD.h"
#include "Ultrasonic.h"

void XPCollection_Task(void *argument)
{
    ADC1_Init();
    OLED_Init();
    while (1)
    {
        // OLED_ShowNum(0,0,ADC1C2_FifterRead(),4,OLED_8X16);
        // OLED_ShowNum(0,16,ADC1C1_FifterRead(),4,OLED_8X16);
        // OLED_Update();
        Ultrasonic_GiveTxNum(0xAA);
        vTaskDelay(150);
    }
}
void UltrasonicSend_Task(void *argument)
{
    UltrasonicTX_Init();
    UltrasonicTX_Set();
    while (1)
    {
        Ultrasonic_Send_Task();
    }
}