#include "ui_mainstart.h"
#include "lvgl.h"
#include <stdio.h>
#include "User_TasksInit.h"
#include "Ultrasonic.h"

/* 获取当前活动屏幕的宽高 */
#define scr_act_width() lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())

static const lv_font_t *font; /* 定义字体 */

static lv_obj_t *switch_ult; /* 干燥模式开关 */

void ui_mainstart(void)
{
    font = &lv_font_montserrat_16;
    // lv_example_label();         /* 功能标题 */
    // lv_example_switch1();       /* 制冷模式开关 */
    // lv_example_switch2();       /* 制暖模式开关 */
    // lv_example_switch3();       /* 干燥模式开关 */

    /* 超声波接收基础对象（矩形背景） */
    lv_obj_t *obj_dry = lv_obj_create(lv_scr_act());
    lv_obj_set_size(obj_dry, scr_act_height() / 2, scr_act_height() / 2);
    lv_obj_align(obj_dry, LV_ALIGN_CENTER, 0, 0);

    /* 超声波接收 开关标签 */
    lv_obj_t *label_ult = lv_label_create(obj_dry);
    lv_label_set_text(label_ult, "Ultrasonic");
    lv_obj_set_style_text_font(label_ult, font, LV_STATE_DEFAULT);
    lv_obj_align(label_ult, LV_ALIGN_CENTER, 0, -scr_act_height() / 16);

    /* 超声波接收 开关 */
    switch_ult = lv_switch_create(obj_dry);
    lv_obj_set_size(switch_ult, scr_act_height() / 6, scr_act_height() / 12);
    lv_obj_align(switch_ult, LV_ALIGN_CENTER, 0, scr_act_height() / 16);
    // lv_obj_add_state(switch_ult, LV_STATE_CHECKED | LV_STATE_DISABLED);
}

void ui_Ultrasonic_Task(void *argument)
{
    ui_mainstart();

    while (1)
    {
        // 使用互斥锁来保护LVGL操作
        if (pdTRUE == xSemaphoreTake(xMutex, portMAX_DELAY))
        {
            xSemaphoreGive(xMutex); // 释放互斥锁
        }

        // 如果 ultrasonic_state 为 true, 则开启开关，否则关闭开关
        if (UltrasonicRX_Get()) {
            lv_obj_add_state(switch_ult, LV_STATE_CHECKED);  // 开启开关
        } else {
            lv_obj_clear_state(switch_ult, LV_STATE_CHECKED);  // 关闭开关
        }

        vTaskDelay(10);
    }
}
