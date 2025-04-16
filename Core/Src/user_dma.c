#include "user_dma.h"

void DMA_Init(void)
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);

  NVIC_SetPriority(DMA2_Stream3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(DMA2_Stream3_IRQn);
}

