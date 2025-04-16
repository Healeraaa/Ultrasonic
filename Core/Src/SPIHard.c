#include "SPIHard.h"
#include "delay.h"

void SPI1_Init(void)
{
  // 定义并初始化SPI和GPIO的配置结构体
  LL_SPI_InitTypeDef SPI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  // 使能SPI1和GPIOA的时钟
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);  // 使能SPI1时钟
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); // 使能GPIOA时钟

  /*SPI1 GPIO Configuration
    PA5   ------> SPI1_SCK
    PA7   ------> SPI1_MOSI
  */
  // 配置GPIO引脚为SPI功能
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5 | LL_GPIO_PIN_7;  // 选择PA5和PA7引脚
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;        // 设置为复用模式
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH; // 设置GPIO速度为非常高
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL; // 设置输出类型为推挽输出
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;               // 不使用上下拉电阻
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;             // 选择复用功能5（SPI1）
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);                // 初始化GPIOA

  /* SPI1 DMA Init */
  /* SPI1_TX Init */
  LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_3, LL_DMA_CHANNEL_3);                       // 配置DMA2的流3，通道3
  LL_DMA_SetDataTransferDirection(DMA2, LL_DMA_STREAM_3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH); // 设置数据传输方向为内存到外设
  LL_DMA_SetStreamPriorityLevel(DMA2, LL_DMA_STREAM_3, LL_DMA_PRIORITY_LOW);                 // 设置流优先级为低
  LL_DMA_SetMode(DMA2, LL_DMA_STREAM_3, LL_DMA_MODE_NORMAL);                                 // 设置DMA模式为普通模式
  LL_DMA_SetPeriphIncMode(DMA2, LL_DMA_STREAM_3, LL_DMA_PERIPH_NOINCREMENT);                 // 设置外设地址不递增
  LL_DMA_SetMemoryIncMode(DMA2, LL_DMA_STREAM_3, LL_DMA_MEMORY_INCREMENT);                   // 设置内存地址递增
  LL_DMA_SetPeriphSize(DMA2, LL_DMA_STREAM_3, LL_DMA_PDATAALIGN_HALFWORD);                   // 设置外设数据宽度为半字（16位）
  LL_DMA_SetMemorySize(DMA2, LL_DMA_STREAM_3, LL_DMA_MDATAALIGN_HALFWORD);                   // 设置内存数据宽度为半字（16位）
  LL_DMA_DisableFifoMode(DMA2, LL_DMA_STREAM_3);                                             // 禁用FIFO模式

  // 配置SPI1的参数
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;         // 全双工模式
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;                      // 主模式
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;              // 数据宽度为8位
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;           // 时钟极性为高电平
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;                // 时钟相位为第二个边沿采样
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;                          // 软件控制NSS信号
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;       // 波特率预分频为2
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;                    // 高位先发送
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE; // 禁用CRC计算
  SPI_InitStruct.CRCPoly = 10;                                   // CRC多项式（未使用）
  LL_SPI_Init(SPI1, &SPI_InitStruct);                            // 初始化SPI1

  // 设置SPI协议为Motorola模式
  LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);

  // 使能SPI1的DMA传输请求
  LL_SPI_EnableDMAReq_TX(SPI1);

  // 使能 SPI1
  LL_SPI_Enable(SPI1);
}

void SPI1_Transmit8(uint8_t data)
{
  // 等待发送缓冲区为空（可以写入新数据）
  while (!LL_SPI_IsActiveFlag_TXE(SPI1))
  {
  }
  // 发送数据
  LL_SPI_TransmitData8(SPI1, data);
  // 等待数据从发送缓冲区转移到移位寄存器
  while (!LL_SPI_IsActiveFlag_TXE(SPI1))
  {
  }
  // 等待 SPI 总线空闲（数据从移位寄存器发送完成）
  while (LL_SPI_IsActiveFlag_BSY(SPI1))
  {
  }
}

void SPI1_Transmit8_Time(uint8_t data, uint32_t Timeout)
{
  uint32_t tickstart = HAL_GetTick(); // 获取当前时间戳

  // 等待发送缓冲区为空（可以写入新数据）
  while (!LL_SPI_IsActiveFlag_TXE(SPI1))
  {
    // 检查是否超时
    if ((HAL_GetTick() - tickstart) >= Timeout)
    {
      // 超时退出
      return; // 可以根据需要处理超时错误
    }
  }

  // 发送数据
  LL_SPI_TransmitData8(SPI1, data);

  // 等待数据从发送缓冲区转移到移位寄存器
  while (!LL_SPI_IsActiveFlag_TXE(SPI1))
  {
    // 检查是否超时
    if ((HAL_GetTick() - tickstart) >= Timeout)
    {
      // 超时退出
      return; // 可以根据需要处理超时错误
    }
  }

  // 等待 SPI 总线空闲（数据从移位寄存器发送完成）
  while (LL_SPI_IsActiveFlag_BSY(SPI1))
  {
    // 检查是否超时
    if ((HAL_GetTick() - tickstart) >= Timeout)
    {
      // 超时退出
      return; // 可以根据需要处理超时错误
    }
  }
}

void SPI1_Transmit16(uint16_t data)
{
  // 等待发送缓冲区为空（可以写入新数据）
  while (!LL_SPI_IsActiveFlag_TXE(SPI1))
  {
  }
  // 发送数据
  LL_SPI_TransmitData16(SPI1, data);
  // 等待发送完成（确保数据从移位寄存器中发送出去）
  while (!LL_SPI_IsActiveFlag_TXE(SPI1))
  {
  }
  while (LL_SPI_IsActiveFlag_BSY(SPI1))
  {
  }
}

void SPI1_Transmit16_Time(uint16_t data, uint32_t Timeout)
{
  uint32_t tickstart = HAL_GetTick(); // 获取当前时间戳

  // 等待发送缓冲区为空（可以写入新数据）
  while (!LL_SPI_IsActiveFlag_TXE(SPI1))
  {
    // 检查是否超时
    if ((HAL_GetTick() - tickstart) >= Timeout)
    {
      // 超时退出
      return; // 可以根据需要处理超时错误
    }
  }

  // 发送数据
  LL_SPI_TransmitData16(SPI1, data);

  // 等待数据从发送缓冲区转移到移位寄存器
  while (!LL_SPI_IsActiveFlag_TXE(SPI1))
  {
    // 检查是否超时
    if ((HAL_GetTick() - tickstart) >= Timeout)
    {
      // 超时退出
      return; // 可以根据需要处理超时错误
    }
  }

  // 等待 SPI 总线空闲（数据从移位寄存器发送完成）
  while (LL_SPI_IsActiveFlag_BSY(SPI1))
  {
    // 检查是否超时
    if ((HAL_GetTick() - tickstart) >= Timeout)
    {
      // 超时退出
      return; // 可以根据需要处理超时错误
    }
  }
}

/**
 * @brief  SPI1 数据位宽转换
 * @param  dataWidth This parameter can be one of the following values:
          @arg @ref LL_SPI_DATAWIDTH_8BIT
          @arg @ref LL_SPI_DATAWIDTH_16BIT
 * @retval None
 */
void SPI1_SetDataWidth(uint32_t dataWidth)
{
  // 禁用 SPI，准备更改配置
  LL_SPI_Disable(SPI1);
  // 设置新的数据位宽（8位或16位）
  LL_SPI_SetDataWidth(SPI1, dataWidth);
  // 启用 SPI
  LL_SPI_Enable(SPI1);
}

void SPI1_Transmit_DMA(uint8_t *data, uint32_t size)
{
  while (LL_DMA_IsEnabledStream(DMA2, LL_DMA_STREAM_3))
  {
  }

  LL_DMA_ClearFlag_TC3(DMA2); // 清除传输完成标志

  LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_3);

  // 设置DMA的外设地址为SPI1的数据寄存器地址
  LL_DMA_SetPeriphAddress(DMA2, LL_DMA_STREAM_3, LL_SPI_DMA_GetRegAddr(SPI1));

  // 设置DMA的内存地址为要传输的数据的起始地址
  LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_3, (uint32_t)data);

  // 设置DMA传输的数据长度
  LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_3, size);

  // 使能DMA流
  LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_3);

  // 等待DMA传输完成（可选）
  while (LL_DMA_IsEnabledStream(DMA2, LL_DMA_STREAM_3))
  {
    // 等待传输完成
  }
  while (!LL_DMA_IsActiveFlag_TC3(DMA2))
  {
    /* code */
  }

  LL_DMA_ClearFlag_TC3(DMA2); // 清除传输完成标志
}
