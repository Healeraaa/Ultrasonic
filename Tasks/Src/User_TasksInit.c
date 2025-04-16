/* Private includes -----------------------------------------------------------*/
#include "user_TasksInit.h"
#include "User_UltrasonictxTask.h"
#include "User_UltrasonicrxTask.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lv_demo_stress.h"
#include "stdio.h"
// #include "OLED.h"
// #include "key.h"
#include "LED.h"
#include "ui_mainstart.h"


void LvHandlerTask(void *argument);

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Timers --------------------------------------------------------------------*/

/* Tasks ---------------------------------------------------------------------*/

TaskHandle_t RGBLEDToggle_Task_Handle;
TaskHandle_t LvHandlerTask_Handle;
TaskHandle_t LVGL_Task_Handle;
TaskHandle_t XPCollection_Task_Handle;
TaskHandle_t UltrasonicSend_Task_Handle;


/* Queues --------------------------------------------------------------------*/

/* Events --------------------------------------------------------------------*/

// 定义互斥锁
SemaphoreHandle_t xMutex;

/**
 * @brief  RGB间隔0.5s变化一次
 * @param  argument: Not used
 * @retval None
 */
void RGBLEDToggle_Task(void *argument)
{
  uint8_t Color = 0;
  while (1)
  {
    (++Color > 6) ? (Color = 0) : (Color = Color);
    LED1_RGB_ON(Color);
    vTaskDelay(500);
  }
}

/**
 * @brief  LVGL实验
 * @param  argument: Not used
 * @retval None
 */
void LVGL_Task(void *argument)
{
  ui_mainstart();

  while (1)
  {
    // 使用互斥锁来保护LVGL操作
    if (pdTRUE == xSemaphoreTake(xMutex, portMAX_DELAY))
    {
      xSemaphoreGive(xMutex); // 释放互斥锁
    }
    vTaskDelay(10);
  }
}






/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void User_Tasks_Init(void)
{

#ifdef UltrasonicTX
  xTaskCreate(RGBLEDToggle_Task, "RGBLEDToggle_Task", 128, NULL, 1, &RGBLEDToggle_Task_Handle);
  xTaskCreate(XPCollection_Task, "XPCollection_Task", 128, NULL, 2, &XPCollection_Task_Handle);
  xTaskCreate(UltrasonicSend_Task, "UltrasonicSend_Task", 128, NULL, 2, &UltrasonicSend_Task_Handle);
  
#endif

#ifdef UltrasonicRX
  // 创建互斥锁
  xMutex = xSemaphoreCreateMutex();
  // xTaskCreate(LvHandlerTask, "LvHandlerTask", 128 * 24, NULL, 2, &LvHandlerTask_Handle);
  xTaskCreate(UltrasonicReceive_Task, "UltrasonicReceive_Task", 128, NULL, 3, &LVGL_Task_Handle);
  xTaskCreate(RGBLEDToggle_Task, "RGBLEDToggle_Task", 128, NULL, 1, &RGBLEDToggle_Task_Handle);
#endif
}

/**
 * @brief  FreeRTOS Tick Hook, to increase the LVGL tick
 * @param  None
 * @retval None
 */
void TaskTickHook(void)
{
  // to increase the LVGL tick
  lv_tick_inc(1);
  // to increase the timerpage's timer(put in here is to ensure the Real Time)
}

/**
 * @brief  LVGL Handler task, to run the lvgl
 * @param  argument: Not used
 * @retval None
 */
void LvHandlerTask(void *argument)
{
  while (1)
  {
    if (pdTRUE == xSemaphoreTake(xMutex, portMAX_DELAY))
    {
      lv_task_handler();      // 执行LVGL的任务处理
      xSemaphoreGive(xMutex); // 释放互斥锁
    }
    vTaskDelay(5);
  }
}
