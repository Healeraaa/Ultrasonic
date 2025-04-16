#ifndef __SPIHARD_H__
#define __SPIHARD_H__

#include "main.h"

void SPI1_Init(void);
void SPI1_Transmit8(uint8_t data);
void SPI1_Transmit16(uint16_t data);
void SPI1_SetDataWidth(uint32_t dataWidth);
void SPI1_Transmit_DMA(uint8_t *data, uint32_t size);
void SPI1_Transmit8_Time(uint8_t data, uint32_t Timeout);
void SPI1_Transmit16_Time(uint16_t data, uint32_t Timeout);

#endif 

