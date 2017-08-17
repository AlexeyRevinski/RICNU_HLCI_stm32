#include "plan_config.h"

extern uint8_t spi_rx_buffer[DATA_SIZE_FLEXSEA];
extern uint8_t spi_tx_buffer[DATA_SIZE_FLEXSEA];
extern uint8_t usart_tx_buffer[DATA_SIZE_P2U];
extern uint8_t usart_rx_buffer[DATA_SIZE_U2P];
extern uint8_t comm_str_1[COMM_STR_BUF_LEN];


//==============================================================================
// FUNCTION GPIO_config()
//      - Configures GPIO pins for: SPI, USART
//==============================================================================
void GPIO_config(void)
{
  // Declare the GPIO initialization structure
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //============================================================================
  // INPUT PINS
  //============================================================================
  
  // EXTI CARD DETECT PIN  -----------------------------------------------------
  RCC_AHBPeriphClockCmd(GPIO_EXTI_CD_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode  = GPIO_EXTI_CD_MODE;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_EXTI_CD_PUPD;
  GPIO_InitStructure.GPIO_Pin   = GPIO_EXTI_CD_PIN;
  GPIO_Init(GPIO_EXTI_CD_PORT, &GPIO_InitStructure);
  
  // EXTI USER BUTTON PIN  -----------------------------------------------------
  RCC_AHBPeriphClockCmd(GPIO_EXTI_UB_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode  = GPIO_EXTI_UB_MODE;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_EXTI_UB_PUPD;
  GPIO_InitStructure.GPIO_Pin   = GPIO_EXTI_UB_PIN;
  GPIO_Init(GPIO_EXTI_UB_PORT, &GPIO_InitStructure);
  
  //============================================================================
  // OUTPUT PINS
  //============================================================================
  
  // SPI SCK PIN  --------------------------------------------------------------
  RCC_AHBPeriphClockCmd(GPIO_SPIx_SCK_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode  = GPIO_SPIx_SCK_MODE;
  GPIO_InitStructure.GPIO_OType = GPIO_SPIx_SCK_OTYPE;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPIx_SCK_SPEED;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_SPIx_SCK_PUPD;
  GPIO_InitStructure.GPIO_Pin   = GPIO_SPIx_SCK_PIN;
  GPIO_Init(GPIO_SPIx_SCK_PORT, &GPIO_InitStructure);
  
  // SPI MOSI PIN  -------------------------------------------------------------
  RCC_AHBPeriphClockCmd(GPIO_SPIx_MOSI_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode  = GPIO_SPIx_MOSI_MODE;
  GPIO_InitStructure.GPIO_OType = GPIO_SPIx_MOSI_OTYPE;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPIx_MOSI_SPEED;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_SPIx_MOSI_PUPD;
  GPIO_InitStructure.GPIO_Pin   = GPIO_SPIx_MOSI_PIN;
  GPIO_Init(GPIO_SPIx_MOSI_PORT, &GPIO_InitStructure);
  
  // SPI MISO PIN  -------------------------------------------------------------
  RCC_AHBPeriphClockCmd(GPIO_SPIx_MISO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode  = GPIO_SPIx_MISO_MODE;
  GPIO_InitStructure.GPIO_OType = GPIO_SPIx_MISO_OTYPE;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPIx_MISO_SPEED;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_SPIx_MISO_PUPD;
  GPIO_InitStructure.GPIO_Pin   = GPIO_SPIx_MISO_PIN;
  GPIO_Init(GPIO_SPIx_MISO_PORT, &GPIO_InitStructure);
  
  // SPI SS MANAGE PIN  --------------------------------------------------------
  RCC_AHBPeriphClockCmd(GPIO_SPIx_SS_MN_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode  = GPIO_SPIx_SS_MN_MODE;
  GPIO_InitStructure.GPIO_OType = GPIO_SPIx_SS_MN_OTYPE;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_SPIx_SS_MN_PUPD;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPIx_SS_MN_SPEED;
  GPIO_InitStructure.GPIO_Pin   = GPIO_SPIx_SS_MN_PIN;
  GPIO_Init(GPIO_SPIx_SS_MN_PORT, &GPIO_InitStructure);
  
  // SPI SS SD CARD PIN  -------------------------------------------------------
  RCC_AHBPeriphClockCmd(GPIO_SPIx_SS_SD_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode  = GPIO_SPIx_SS_SD_MODE;
  GPIO_InitStructure.GPIO_OType = GPIO_SPIx_SS_SD_OTYPE;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_SPIx_SS_SD_PUPD;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPIx_SS_SD_SPEED;
  GPIO_InitStructure.GPIO_Pin   = GPIO_SPIx_SS_SD_PIN;
  GPIO_Init(GPIO_SPIx_SS_SD_PORT, &GPIO_InitStructure);
  
  // USART TX PIN  -------------------------------------------------------------
  RCC_AHBPeriphClockCmd(GPIO_USARTx_TX_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode  = GPIO_USARTx_TX_MODE;
  GPIO_InitStructure.GPIO_OType = GPIO_USARTx_TX_OTYPE;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_USARTx_TX_PUPD;
  GPIO_InitStructure.GPIO_Speed = GPIO_USARTx_TX_SPEED;
  GPIO_InitStructure.GPIO_Pin   = GPIO_USARTx_TX_PIN;
  GPIO_Init(GPIO_USARTx_TX_PORT, &GPIO_InitStructure);
  
  // USART RX PIN  -------------------------------------------------------------
  RCC_AHBPeriphClockCmd(GPIO_USARTx_RX_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Mode  = GPIO_USARTx_RX_MODE;
  GPIO_InitStructure.GPIO_OType = GPIO_USARTx_RX_OTYPE;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_USARTx_RX_PUPD;
  GPIO_InitStructure.GPIO_Speed = GPIO_USARTx_RX_SPEED;
  GPIO_InitStructure.GPIO_Pin   = GPIO_USARTx_RX_PIN;
  GPIO_Init(GPIO_USARTx_RX_PORT, &GPIO_InitStructure);
  
  // ALTERNATE FUNCTION PIN CONFIGURATIONS
  GPIO_PinAFConfig(GPIO_SPIx_SCK_PORT ,GPIO_SPIx_SCK_SOURCE ,GPIO_SPIx_SCK_AF );
  GPIO_PinAFConfig(GPIO_SPIx_MOSI_PORT,GPIO_SPIx_MOSI_SOURCE,GPIO_SPIx_MOSI_AF);
  GPIO_PinAFConfig(GPIO_SPIx_MISO_PORT,GPIO_SPIx_MISO_SOURCE,GPIO_SPIx_MISO_AF);
  GPIO_PinAFConfig(GPIO_USARTx_TX_PORT,GPIO_USARTx_TX_SOURCE,GPIO_USARTx_TX_AF);
  GPIO_PinAFConfig(GPIO_USARTx_RX_PORT,GPIO_USARTx_RX_SOURCE,GPIO_USARTx_RX_AF);
  
  // INITIAL OUTPUT LOGIC LEVELS  ----------------------------------------------
  GPIO_ResetBits(GPIO_SPIx_SCK_PORT,GPIO_SPIx_SCK_PIN);   // SPI CLK low
  GPIO_SetBits(GPIO_SPIx_MOSI_PORT,GPIO_SPIx_MOSI_PIN);   // SPI MOSI high
  GPIO_SetBits(GPIO_SPIx_MISO_PORT,GPIO_SPIx_MISO_PIN);   // SPI MISO high
  GPIO_SetBits(GPIO_SPIx_SS_SD_PORT,GPIO_SPIx_SS_SD_PIN); // SPI SS SD high
  GPIO_SetBits(GPIO_SPIx_SS_MN_PORT,GPIO_SPIx_SS_MN_PIN); // SPI SS MN high
  GPIO_SetBits(GPIO_USARTx_TX_PORT,GPIO_USARTx_TX_PIN);   // USART TX high
  GPIO_SetBits(GPIO_USARTx_RX_PORT,GPIO_USARTx_RX_PIN);   // USART RX high
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
  
  // Start SPI 1 clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
  
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
    
  // Enable SPI interrupts for DMA
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
  
  // Set FIFO threshold to
  SPI_RxFIFOThresholdConfig(SPIx, SPI_RxFIFOThreshold_QF);
  
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
  
  // Start USART clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
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
  
  // Start SYSCFG clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  SYSCFG_DMAChannelRemapConfig(SYSCFG_DMARemap_USART1Tx, ENABLE);
  SYSCFG_DMAChannelRemapConfig(SYSCFG_DMARemap_USART1Rx, ENABLE);
  
  // Start DMA clock
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
  
  // SPI RECEIVE CHANNEL -------------------------------------------------------
  
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
  //DMA_Cmd(SPIx_DMA_RX_CHANNEL, ENABLE);
  
  
   // SPI TRANSMIT CHANNEL -----------------------------------------------------
  
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
  
  
  // USART RECEIVE CHANNEL -----------------------------------------------------
  
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
  //DMA_Cmd(USARTx_DMA_RX_CHANNEL, ENABLE);
  
  
  // USART TRANSMIT CHANNEL ----------------------------------------------------

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
  
  // Configure EXTI2 Interrupt
  NVIC_InitStructure.NVIC_IRQChannel = EXTI_CD_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  // Configure EXTI0 Interrupt
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}


//==============================================================================
// FUNCTION RTC_config()
//      - Configures real time clock peripheral
//==============================================================================
void RTC_config(void)
{
  ;
}

//==============================================================================
// FUNCTION EXTI_config()
//      - Configures sd card detect pin external interrupt
//==============================================================================
void EXTI_config(void)
{
  // Declare the GPIO initialization structure
  EXTI_InitTypeDef EXTI_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  // Configure EXTI line
  SYSCFG_EXTILineConfig(EXTI_CD_PORT_SOURCE,EXTI_CD_PIN_SOURCE);
  EXTI_InitStructure.EXTI_Line = EXTI_CD_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  // Configure EXTI line
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}