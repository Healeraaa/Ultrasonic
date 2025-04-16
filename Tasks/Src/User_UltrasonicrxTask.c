#include "User_UltrasonicrxTask.h"
#include "lvgl.h"
#include "User_TasksInit.h"
#include "Ultrasonic.h"
#include "user_gpio.h"
#include "Lcd_init.h"
#include "LCD.h"
#include "stdio.h"
uint8_t Ultrasonic_value = 0;
// SemaphoreHandle_t semaphore_handle;
// /* 获取当前活动屏幕的宽高 */
// #define scr_act_width() lv_obj_get_width(lv_scr_act())
// #define scr_act_height() lv_obj_get_height(lv_scr_act())

// // 配色方案
// #define PRIMARY_COLOR lv_color_hex(0x2196F3)    // 主蓝色（用于状态栏和部分元素）
// #define SECONDARY_COLOR lv_color_hex(0x4CAF50)  // 健康绿色（不再用于渐变色）
// #define BACKGROUND_COLOR lv_color_hex(0xF5F5F5) // 浅灰背景色
// #define CARD_COLOR lv_color_hex(0xFFFFFF)       // 纯白卡片背景
// #define TEXT_SECONDARY lv_color_hex(0x666666)   // 次级文字颜色（灰色）

// static const lv_font_t *font; /* 定义字体 */

// lv_obj_t *label_ult = NULL;
// lv_obj_t *bar = NULL;
// lv_obj_t *label_load = NULL;

// // SemaphoreHandle_t semaphore_handle;

// void ui_ultrasonic(void)
// {
//     font = &lv_font_montserrat_16;
//     //进度条标签
//     label_ult = lv_label_create(lv_scr_act());
//     lv_label_set_text(label_ult, "%0");
//     lv_obj_set_style_text_font(label_ult, font, LV_STATE_DEFAULT);
//     lv_obj_align(label_ult, LV_ALIGN_CENTER, 0, scr_act_height() / 10);
//     /* 加载标题标签 */
//     label_load = lv_label_create(lv_scr_act());
//     lv_label_set_text(label_load, "LOADING...");
//     lv_obj_set_style_text_font(label_load, font, LV_STATE_DEFAULT);
//     lv_obj_align(label_load, LV_ALIGN_CENTER, 0, -scr_act_height()/10);
//     // 创建进度条
//     bar = lv_bar_create(lv_scr_act());  // 创建进度条，父对象是当前屏幕
//     lv_obj_set_size(bar, scr_act_width(), scr_act_height()/10);
//     lv_obj_align(bar, LV_ALIGN_CENTER, 0, 0);  // 将进度条居中显示
//     lv_bar_set_range(bar, 0, 255);  // 设置进度条的范围为 0 到 100
//     lv_bar_set_value(bar, 0, LV_ANIM_OFF);  // 设置进度为 0，且不使用动画
// }

// void UltrasonicReceive_Task(void *argument)
// {
//     // semaphore_handle = xSemaphoreCreateBinary();//创建二值信号量
//     uint8_t Ultrasonic_value = 0;
//     UltrasonicRX_Init();
//     ui_ultrasonic();
//     while (1)
//     {
//         //读取传感器的值
//         // Ultrasonic_value = 0xAA;
//         // xSemaphoreTake(semaphore_handle,portMAX_DELAY);/* 获取信号量并死等 */
//         if(UltrasonicRX_Get()==0)
//             Ultrasonic_value++;

//         // 使用互斥锁来保护LVGL操作
//         if (pdTRUE == xSemaphoreTake(xMutex, portMAX_DELAY))
//         {
//             //更新GUI
//             // lv_label_set_text(label_ult, buf);
//             lv_bar_set_value(bar, Ultrasonic_value, LV_ANIM_ON);
//             lv_label_set_text_fmt(label_ult, "Ultra:%d", lv_bar_get_value(bar)); /* 获取当前值，更新显示 */

//             xSemaphoreGive(xMutex); // 释放互斥锁
//         }
//         vTaskDelay(100);
//     }
// }

// // 7. 必须实现的中断服务函数
// void EXTI15_10_IRQHandler(void)
// {
//     if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_10))
//     {
//         LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_10); // 清除中断标志
//         xSemaphoreGiveFromISR(semaphore_handle,NULL);
//         // 用户的中断处理代码
//     }
// }

void UltrasonicReceive_Task(void *argument)
{
    uint8_t display_str[32];
    UltrasonicRX_Init();
    while (1)
    {
        // 读取传感器的值
        if (Ultrasonic_GetRxFlag())
        {
            Ultrasonic_value = Ultrasonic_Receive_Task();
        }
        if (Ultrasonic_value != 0)
        {
            sprintf(display_str, "Ultrasonic:%d", Ultrasonic_value);
            LCD_ShowString(72, LCD_H / 2 - 20, display_str, BLUE, RED, 24, 0);
        }

        // 使用互斥锁来保护LVGL操作

        // vTaskDelay(100);
    }
}
