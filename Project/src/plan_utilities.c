#include "plan_utilities.h"

//==============================================================================
// FUNCTION GPIO_ToggleBits()
//      - Negates GPIO output data register
//==============================================================================
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  GPIOx->ODR ^= GPIO_Pin;
}

//==============================================================================
// FUNCTION set_tick()
//      - Sets SysTick to set an interrupt request after specified
//        number of microseconds
//==============================================================================
void set_tick(uint32_t us)
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config((RCC_Clocks.HCLK_Frequency/FACTOR_us_PER_s)*us);
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}

//==============================================================================
// FUNCTION change_spi_mode()
//      - Changes SPI configuration based on SPI mode requested
//==============================================================================
void change_spi_mode(spi_mode mode)
{
  // SPI mode for initializing the SD card in SPI mode
  if(mode==SPI_MODE_SD_INIT)
  {
    SPI_Cmd(SPIx, DISABLE);
    // Disable interactions with DMA
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, DISABLE);
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, DISABLE);
    // SPI clock rate at 375kHz
    SPIx->CR1 &= ~SPI_CR1_BR;                   //Clear baud rate prescaler bits
    SPIx->CR1 |= SPI_BaudRatePrescaler_128;     //Set baud rate prescaler
    SPI_Cmd(SPIx, ENABLE);
  }
  // SPI mode for communicating with Manage
  if(mode==SPI_MODE_MANAGE||mode==SPI_MODE_SD_DATA)
  {
    SPI_Cmd(SPIx, DISABLE);
    // Enable SPI interrupts for DMA
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
    // SPI clock rate at 1.5MHz
    SPIx->CR1 &= ~SPI_CR1_BR;                   //Clear baud rate prescaler bits
    SPIx->CR1 |= SPI_BaudRatePrescaler_32;      //Set baud rate prescaler
    SPI_Cmd(SPIx, ENABLE);
  }
}

void read_line(char* line)
{
  FATFS fs;
  FIL fil;
  UINT br;
  f_mount(&fs,"", 0);   // Mount the default drive
  f_open(&fil, "fsm.jso", FA_READ);
  f_read(&fil,line,TEXT_LENGTH,&br);
  f_close(&fil);
  f_mount(0,"", 0);    // Unmount the default drive
  
    /*
  UINT btr = sizeof line;
  UINT br;
  
  if(pf_mount(&fs))
  {//ERROR STATE
    GPIO_SetBits(LEDx_PORT, LED2_PIN);
  }
  if(pf_open("fsm.json"))
  {
    GPIO_SetBits(LEDx_PORT, LED2_PIN);
  }
  pf_read (line,btr,&br);
  */
}




