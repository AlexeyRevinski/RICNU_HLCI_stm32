#include "plan_config.h"

extern uint8_t spi_rx_buffer[DATA_SIZE_FLEXSEA];
extern uint8_t spi_tx_buffer[DATA_SIZE_FLEXSEA];
extern uint8_t usart_tx_buffer[DATA_SIZE_P2U];
extern uint8_t usart_rx_buffer[DATA_SIZE_U2P];

extern uint8_t comm_str_1[COMM_STR_BUF_LEN];


//==============================================================================
// FUNCTION RCC_config()
//      - Configures system and peripheral clocks
//==============================================================================
void RCC_confg(void)
{
  // AHB clocks
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA |
                        RCC_AHBPeriph_DMA1,
                        ENABLE);
  // APB2 clocks
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 |
                         RCC_APB2Periph_USART1 |
                         RCC_APB2Periph_SYSCFG,
                         ENABLE);
}


//==============================================================================
// FUNCTION SYSCFG_config()
//      - Remaps some peripheral functions
//==============================================================================
void SYSCFG_config(void)
{
  SYSCFG_DMAChannelRemapConfig(SYSCFG_DMARemap_USART1Tx, ENABLE);
  SYSCFG_DMAChannelRemapConfig(SYSCFG_DMARemap_USART1Rx, ENABLE);
}


//==============================================================================
// FUNCTION GPIO_config()
//      - Configures GPIO pins for: SPI, USART
//==============================================================================
void GPIO_config(void)
{
  // Declare the GPIO initialization structure
  GPIO_InitTypeDef GPIO_InitStructure;
  
  // SPI SCK, MOSI, MISO PINS --------------------------------------------------
  
  // Alternate Function assignments
  GPIO_PinAFConfig(SPIx_GPIO_PORT, SPIx_SCK_SOURCE,  SPIx_AF);
  GPIO_PinAFConfig(SPIx_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_AF);
  GPIO_PinAFConfig(SPIx_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_AF);
  
  // Pin configurations
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_Pin   = SPIx_SCK_PIN | SPIx_MOSI_PIN | SPIx_MISO_PIN;
  GPIO_Init(SPIx_GPIO_PORT, &GPIO_InitStructure);
  
  // SPI NSS PIN  --------------------------------------------------------------
  /*
  // HARDWARE CONTROL
  // Alternate Function assignment
  GPIO_PinAFConfig(SPIx_GPIO_PORT, SPIx_NSS_SOURCE,  SPIx_AF);
  // Pin configurations
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_Pin   = SPIx_NSS_PIN;
  GPIO_Init(SPIx_GPIO_PORT, &GPIO_InitStructure);
  */
  // SOFTWARE CONTROL (MANUAL)
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_Pin = SPIx_NSS_PIN;
  GPIO_Init(SPIx_GPIO_PORT, &GPIO_InitStructure);
    
  // USART GPIO PINS  ----------------------------------------------------------
  
  // Alternate Function assignments
  GPIO_PinAFConfig(USARTx_GPIO_PORT, USARTx_TX_SOURCE, USARTx_AF);
  GPIO_PinAFConfig(USARTx_GPIO_PORT, USARTx_RX_SOURCE, USARTx_AF);
  
  // Pin configuration
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_Pin   = USARTx_TX_PIN | USARTx_RX_PIN;
  GPIO_Init(USARTx_GPIO_PORT, &GPIO_InitStructure);
}


//==============================================================================
// FUNCTION SPI_config()
//      - Configures STM32's SPI peripheral
//      - 1.5MHz clock (48MHz/32)
//==============================================================================
void SPI_config(void)
{
  // Declare SPI initialization structure 
  SPI_InitTypeDef  SPI_InitStructure;
  
  // SPI configuration
  SPI_I2S_DeInit(SPIx);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DATASIZE;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPIx, &SPI_InitStructure);
  
  /*
  // Enable NSS output in master pulse mode
  SPI_SSOutputCmd(SPIx, ENABLE);
  SPI_NSSPulseModeCmd(SPIx, ENABLE);
  */
  
  // Set FIFO threshold to
  SPI_RxFIFOThresholdConfig(SPIx, SPI_RxFIFOThreshold_QF);
  
  // Enable SPI TX and RX interrupts for DMA
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
  
  // Enable SPI peripheral
  SPI_Cmd(SPIx, ENABLE);
}


//==============================================================================
// FUNCTION SPI_config()
//      - Configures STM32's USART peripheral
//      - 115200 Baud rate
//==============================================================================
void USART_config(void)
{
  // Declare USART initialization structure
  USART_InitTypeDef USART_InitStructure;
  
  // USART configuration
  USART_InitStructure.USART_BaudRate = 230400;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USARTx, &USART_InitStructure);
  
  // Enable USART TX and RX interrupts for DMA
  USART_DMACmd(USART1,USART_DMAReq_Tx, ENABLE);
  USART_DMACmd(USART1,USART_DMAReq_Rx, ENABLE);
    
  // Enable USART peripheral
  USART_Cmd(USARTx, ENABLE);
}

//==============================================================================
// FUNCTION DMA_config()
//      - Configures STM32's DMA controller
//      - Channel 2 = SPI RX
//      - Channel 3 = SPI TX
//      - Channel 4 = USART TX
//==============================================================================
void DMA_config(void)
{
  // Declare DMA initialization structure
  DMA_InitTypeDef           DMA_InitStructure;
  
  
  // SPI RECEIVE REGISTER  -----------------------------------------------------
  
  DMA_DeInit(SPIx_DMA_RX_CHANNEL);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPIx_DR_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)spi_rx_buffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = DATA_SIZE_FLEXSEA;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(SPIx_DMA_RX_CHANNEL, &DMA_InitStructure);
  DMA_ITConfig(SPIx_DMA_RX_CHANNEL, DMA_IT_TC, ENABLE);
  DMA_Cmd(SPIx_DMA_RX_CHANNEL, ENABLE);
  
  
   // SPI TRANSMIT REGISTER  ---------------------------------------------------
  
  DMA_DeInit(SPIx_DMA_TX_CHANNEL);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPIx_DR_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)comm_str_1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = DATA_SIZE_FLEXSEA;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(SPIx_DMA_TX_CHANNEL, &DMA_InitStructure);
  DMA_ITConfig(SPIx_DMA_TX_CHANNEL, DMA_IT_TC, ENABLE);
  //DMA_Cmd(SPIx_DMA_TX_CHANNEL, ENABLE);     // Not enabled yet - only when ready
  
  
  // USART RECEIVE REGISTER  ---------------------------------------------------
  
  DMA_DeInit(USARTx_DMA_RX_CHANNEL);    // Alternate channel
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USARTx_RDR_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart_rx_buffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = DATA_SIZE_U2P;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(USARTx_DMA_RX_CHANNEL, &DMA_InitStructure);
  DMA_ITConfig(USARTx_DMA_RX_CHANNEL, DMA_IT_TC, ENABLE);
  DMA_Cmd(USARTx_DMA_RX_CHANNEL, ENABLE);
  
  
  // USART TRANSMIT REGISTER  --------------------------------------------------

  DMA_DeInit(USARTx_DMA_TX_CHANNEL);    // Alternate channel
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USARTx_TDR_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart_tx_buffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = DATA_SIZE_P2U;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(USARTx_DMA_TX_CHANNEL, &DMA_InitStructure);
  DMA_ITConfig(USARTx_DMA_TX_CHANNEL, DMA_IT_TC, ENABLE);
  //DMA_Cmd(USARTx_DMA_TX_CHANNEL, ENABLE);     // Not enabled yet - only when ready
  
}



//==============================================================================
// FUNCTION NVIC_config()
//      - Configures STM32's nested vectored interrupt controller
//==============================================================================
void NVIC_config(void)
{
  // Declare NVIC initialization structure
  NVIC_InitTypeDef NVIC_InitStructure;
  
  // Configure SPI interrupt
  NVIC_InitStructure.NVIC_IRQChannel = DMAx_SPIx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  // Configure USART interrupt
  NVIC_InitStructure.NVIC_IRQChannel = DMAx_USARTx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}