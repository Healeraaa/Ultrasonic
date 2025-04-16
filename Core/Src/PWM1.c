#include "PWM1.h"

#define AUTORELOAD 50 - 1 // 40K

void PWM1_Init(void)
{
  // 瀹氫箟骞跺垵濮嬪寲TIM鍜孫C锛圤utput Compare锛夌粨鏋勪綋
  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};

  // 瀹氫箟骞跺垵濮嬪寲GPIO缁撴瀯浣�
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  // 浣胯兘TIM3鏃堕挓锛孴IM3鎸傝浇鍦ˋPB1鎬荤嚎涓�
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

  // 閰嶇疆TIM3鐨勫熀鏈弬鏁�
  TIM_InitStruct.Prescaler = 42 - 1;                       // 25鍒嗛
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;       // 鍚戜笂璁℃暟妯″紡
  TIM_InitStruct.Autoreload = AUTORELOAD;                   // 鑷姩閲嶈杞藉€硷紝鏈€澶т负65535
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1; // 鏃堕挓鍒嗛涓�1
  LL_TIM_Init(TIM3, &TIM_InitStruct);                       // 鍒濆鍖朤IM3

  // 绂佺敤TIM3鐨勮嚜鍔ㄩ噸瑁呰浇棰勮杞藉姛鑳�
  LL_TIM_DisableARRPreload(TIM3);

  // 浣胯兘TIM3閫氶亾3鐨勮緭鍑烘瘮杈冮瑁呰浇鍔熻兘
  LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH3);
  // 浣胯兘TIM3閫氶亾4鐨勮緭鍑烘瘮杈冮瑁呰浇鍔熻兘
  LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH4);

  // 閰嶇疆TIM3閫氶亾3鐨勮緭鍑烘瘮杈冩ā寮�
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;                // PWM妯″紡1
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;           // 鍒濆鐘舵€佷笅绂佺敤杈撳嚭姣旇緝
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;          // 鍒濆鐘舵€佷笅绂佺敤浜掕ˉ杈撳嚭姣旇緝
  TIM_OC_InitStruct.CompareValue = 0;                           // 姣旇緝鍊间负0锛屽嵆鍗犵┖姣斾负0%
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;        // 杈撳嚭鏋佹€т负楂樼數骞虫湁鏁�
  LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH3, &TIM_OC_InitStruct); // 鍒濆鍖朤IM3閫氶亾3
  // 閰嶇疆TIM3閫氶亾4鐨勮緭鍑烘瘮杈冩ā寮�
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM2;                // PWM妯″紡2
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;         // 杈撳嚭鏋佹€т负浣庣數骞虫湁鏁�
  LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH4, &TIM_OC_InitStruct); // 鍒濆鍖朤IM3閫氶亾3

  // 绂佺敤TIM3閫氶亾3鐨勫揩閫熻緭鍑烘瘮杈冩ā寮�
  LL_TIM_OC_DisableFast(TIM3, LL_TIM_CHANNEL_CH3);
  // 绂佺敤TIM3閫氶亾4鐨勫揩閫熻緭鍑烘瘮杈冩ā寮�
  LL_TIM_OC_DisableFast(TIM3, LL_TIM_CHANNEL_CH4);

  // 璁剧疆TIM3鐨勮Е鍙戣緭鍑轰负澶嶄綅
  LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);

  // 绂佺敤TIM3鐨勪富浠庢ā寮�
  LL_TIM_DisableMasterSlaveMode(TIM3);

  // 浣胯兘GPIOB鏃堕挓锛孏PIOB鎸傝浇鍦ˋHB1鎬荤嚎涓�
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  // 閰嶇疆GPIOB鐨凱in 0涓哄鐢ㄥ姛鑳�
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1;  // 閫夋嫨Pin 0
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;        // 澶嶇敤妯″紡
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;      // 浣庨€熸ā寮�
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL; // 鎺ㄦ尳杈撳嚭
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;               // 鏃犱笂涓嬫媺鐢甸樆
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;             // 澶嶇敤鍔熻兘閫夋嫨AF2锛圱IM3锛�
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);                // 鍒濆鍖朑PIOB

  // 鍚姩TIM3璁℃暟鍣�
  LL_TIM_EnableCounter(TIM3);

  // 浣胯兘TIM3閫氶亾3鐨勮緭鍑�
  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH3);
  // 浣胯兘TIM3閫氶亾4鐨勮緭鍑�
  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);

  // 璁剧疆鍗犵┖姣旓紙0%鍗犵┖姣旓級
  LL_TIM_OC_SetCompareCH3(TIM3, 0);
   LL_TIM_OC_SetCompareCH4(TIM3, 0);
}

/**
 * @brief  璁剧疆 TIM3 閫氶亾 3 鐨� PWM 鍗犵┖姣�
 * @param  duty_cycle: PWM 鍗犵 ┖姣旓紝鑼冨洿涓� 0 ~ 100锛�0% ~ 100%锛�
 * @retval None
 */
void PWM1_SetDutyCycle(uint8_t duty_cycle)
{
  // 纭繚鍗犵┖姣斿湪鏈夋晥鑼冨洿鍐咃紙0 ~ 100锛�
  if (duty_cycle > 100)
  {
    duty_cycle = 100; // 濡傛灉瓒呰繃 100%锛屽垯闄愬埗涓� 100%
  }

  // 璁＄畻姣旇緝鍊�
  // 鍏紡锛欳ompareValue = (Autoreload + 1) * (duty_cycle / 100)
  // 鍏朵腑 Autoreload 鏄� TIM3 鐨勮嚜鍔ㄩ噸瑁呰浇鍊�
  uint32_t compare_value = (AUTORELOAD + 1) * duty_cycle / 100;

  LL_TIM_OC_SetCompareCH3(TIM3, compare_value);
  LL_TIM_OC_SetCompareCH4(TIM3, compare_value);
}

/**
 * @brief  鍏抽棴 TIM3 閫氶亾 3 鐨� PWM 杈撳嚭
 * @param  None
 * @retval None
 */
void PWM1_Disable(void)
{
  LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH3);
  LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
}

/**
 * @brief  寮€鍚� TIM3 閫氶亾 3 鐨� PWM 杈撳嚭
 * @param  None
 * @retval None
 */
void PWM1_Enable(void)
{
  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH3);
  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
}