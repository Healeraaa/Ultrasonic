#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"
/* Private includes ----------------------------------------------------------*/
#include "Timer.h"
#include "Lcd_init.h"
#include "LCD.h"
#include "CST816.h"
#include "key.h"
#include "delay.h"
// #include "AToD.h"
// #include "OLED.h"
#include "LED.h"

// gui
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lv_conf.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

int main(void)
{

  HAL_Init();
  Key_Init();
  // LED_Init();
  LED1_RGB_Init();

  SystemClock_Config();

  MX_GPIO_Init();
 
  /* USER CODE BEGIN 2 */
  delay_init(168);
  // MX_USART1_UART_Init();



#ifdef UltrasonicTX


#endif

#ifdef UltrasonicRX
  // touch
  CST816_GPIO_Init();
  CST816_RESET();
  // 	//lcd
  delay_ms(100);
  LCD_Init();
  delay_ms(1000);
  LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
  delay_ms(50);
  LCD_Set_Light(50);
  LCD_ShowString(72, LCD_H / 2 - 20, (uint8_t *)"Welcome!", BLUE, RED, 24, 0); // 12*6,16*8,24*12,32*16
  delay_ms(1000);
  LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
  // // LVGL init
  // lv_init();
  // lv_port_disp_init();
  // lv_port_indev_init();
  // delay_ms(100);
#endif

  /* USER CODE END 2 */
  osKernelInitialize();
  MX_FREERTOS_Init();    // 任务初始化
  vTaskStartScheduler(); // 开启任务调度器

  while (1)
  {

  }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
  while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSE_Enable();

  /* Wait till HSE is ready */
  while (LL_RCC_HSE_IsReady() != 1)
  {
  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 336, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();

  /* Wait till PLL is ready */
  while (LL_RCC_PLL_IsReady() != 1)
  {
  }
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

  /* Wait till System clock is ready */
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }
  LL_SetSystemCoreClock(168000000);

  /* Update the time base */
  if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM4 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
