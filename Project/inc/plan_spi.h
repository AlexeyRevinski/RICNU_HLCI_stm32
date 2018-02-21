#ifndef PLAN_SPI_H
#define PLAN_SPI_H

#include "plan_include.h"

uint8_t	SPI_ChangeSpeed(SPI_TypeDef* SPIx, uint16_t SPI_BaudratePrescaler);
uint8_t SPI_WriteByte(SPI_TypeDef* SPIx, uint8_t data);
void	SPI_ReadMultByte(SPI_TypeDef* SPIx,uint8_t* buffer,uint32_t numbytes);
void	SPI_SendMultByte(SPI_TypeDef* SPIx,const uint8_t* buffer,uint32_t numbytes);

#endif
