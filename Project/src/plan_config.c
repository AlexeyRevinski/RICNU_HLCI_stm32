#include "plan_config.h"

extern uint8_t spi_rx_buffer[DATA_SIZE_FLEXSEA];
extern uint8_t spi_tx_buffer[DATA_SIZE_FLEXSEA];
extern uint8_t usart_tx_buffer[DATA_SIZE_P2U];
extern uint8_t usart_rx_buffer[DATA_SIZE_U2P];
extern uint8_t comm_str_1[DATA_SIZE_FLEXSEA];


void hardware_config(void)
{
  RCC_config();         // Clock configuration
  GPIO_config();        // GPIOs - for SPI, USART, etc.
  SPI_config();         // Serial Peripheral Interface Controller
  USART_config();       // Universal Sync/Async Receiver/Transmitter
  DMA_config();         // Direct Memory Access Controller
  EXTI_config();        // External Interrupt Controller
  RTC_config();         // Real Time Clock configuration
  TIM_config();         // Configuring timers
  NVIC_config();        // Nested Vector Interrupt Controller
  
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config((RCC_Clocks.HCLK_Frequency/FACTOR_us_PER_s)*SYS_TICK_US);
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}


//==============================================================================
// FUNCTION GPIO_config()
//      - Configures GPIO pins for: SPI, USART
//==============================================================================

void GPIO_config(void)
{
  // Enable GPIO and alternate function I/O clocks
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|
                         RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|
                         RCC_APB2Periph_AFIO, ENABLE);
  
  // PIN CONFIGURATION ==========================================================
  
  GPIO_PinConfig(GPIO_CAN_SHDN_PORT,    GPIO_CAN_SHDN_PIN,
                 GPIO_CAN_SHDN_SPEED,   GPIO_CAN_SHDN_MODE);    //CAN SHDN
  GPIO_PinConfig(GPIO_CAN_STBY_PORT,    GPIO_CAN_STBY_PIN,
                 GPIO_CAN_STBY_SPEED,   GPIO_CAN_STBY_MODE);    //CAN STBY
  GPIO_PinConfig(GPIO_CAN_RX_PORT,      GPIO_CAN_RX_PIN,
                 GPIO_CAN_RX_SPEED,     GPIO_CAN_RX_MODE);      //CAN RX
  GPIO_PinConfig(GPIO_CAN_TX_PORT,      GPIO_CAN_TX_PIN,
                 GPIO_CAN_TX_SPEED,     GPIO_CAN_TX_MODE);      //CAN TX 
  GPIO_PinConfig(GPIO_LED_1_PORT,       GPIO_LED_1_PIN,
                 GPIO_LED_1_SPEED,      GPIO_LED_1_MODE);       //LED 1 (R)
  GPIO_PinConfig(GPIO_LED_2_PORT,       GPIO_LED_2_PIN,
                 GPIO_LED_2_SPEED,      GPIO_LED_2_MODE);       //LED 2 (Y)
  GPIO_PinConfig(GPIO_LED_3_PORT,       GPIO_LED_3_PIN,
                 GPIO_LED_3_SPEED,      GPIO_LED_3_MODE);       //LED 3 (G)
  GPIO_PinConfig(GPIO_LED_4_PORT,       GPIO_LED_4_PIN,
                 GPIO_LED_4_SPEED,      GPIO_LED_4_MODE);       //LED 4 (B)
  GPIO_PinConfig(GPIO_BT_TX_PORT,       GPIO_BT_TX_PIN,
                 GPIO_BT_TX_SPEED,      GPIO_BT_TX_MODE);       //BTMOD TX
  GPIO_PinConfig(GPIO_BT_RX_PORT,       GPIO_BT_RX_PIN,
                 GPIO_BT_RX_SPEED,      GPIO_BT_RX_MODE);       //BTMOD RX
  GPIO_PinConfig(GPIO_SD_NSS_PORT,      GPIO_SD_NSS_PIN,
                 GPIO_SD_NSS_SPEED,     GPIO_SD_NSS_MODE);      //SD NSS
  GPIO_PinConfig(GPIO_SD_SCK_PORT,      GPIO_SD_SCK_PIN,
                 GPIO_SD_SCK_SPEED,     GPIO_SD_SCK_MODE);      //SD SCK
  GPIO_PinConfig(GPIO_SD_MISO_PORT,     GPIO_SD_MISO_PIN,
                 GPIO_SD_MISO_SPEED,    GPIO_SD_MISO_MODE);     //SD MISO
  GPIO_PinConfig(GPIO_SD_MOSI_PORT,     GPIO_SD_MOSI_PIN,
                 GPIO_SD_MOSI_SPEED,    GPIO_SD_MOSI_MODE);     //SD MOSI
  GPIO_PinConfig(GPIO_SD_CD_PORT,       GPIO_SD_CD_PIN,
                 GPIO_SD_CD_SPEED,      GPIO_SD_CD_MODE);       //SD CD
  GPIO_PinConfig(GPIO_MN_NSS_PORT,      GPIO_MN_NSS_PIN,
                 GPIO_MN_NSS_SPEED,     GPIO_MN_NSS_MODE);      //MN NSS
  GPIO_PinConfig(GPIO_MN_SCK_PORT,      GPIO_MN_SCK_PIN,
                 GPIO_MN_SCK_SPEED,     GPIO_MN_SCK_MODE);      //MN SCK
  GPIO_PinConfig(GPIO_MN_MISO_PORT,     GPIO_MN_MISO_PIN,
                 GPIO_MN_MISO_SPEED,    GPIO_MN_MISO_MODE);     //MN MISO
  GPIO_PinConfig(GPIO_MN_MOSI_PORT,     GPIO_MN_MOSI_PIN,
                 GPIO_MN_MOSI_SPEED,    GPIO_MN_MOSI_MODE);     //MN MOSI
  GPIO_PinConfig(GPIO_FTDI_ENUM_PORT,   GPIO_FTDI_ENUM_PIN,
                 GPIO_FTDI_ENUM_SPEED,  GPIO_FTDI_ENUM_MODE);   //FTDI ENUM
  GPIO_PinConfig(GPIO_FTDI_PWR_PORT,    GPIO_FTDI_PWR_PIN,
                 GPIO_FTDI_PWR_SPEED,   GPIO_FTDI_PWR_MODE);    //FTDI PWR
  GPIO_PinConfig(GPIO_RS485_REN_PORT,   GPIO_RS485_REN_PIN,
                 GPIO_RS485_REN_SPEED,  GPIO_RS485_REN_MODE);   //RS485 REN
  GPIO_PinConfig(GPIO_RS485_TX_PORT,    GPIO_RS485_TX_PIN,
                 GPIO_RS485_TX_SPEED,   GPIO_RS485_TX_MODE);    //RS485 TX
  GPIO_PinConfig(GPIO_RS485_RX_PORT,    GPIO_RS485_RX_PIN,
                 GPIO_RS485_RX_SPEED,   GPIO_RS485_RX_MODE);    //RS485 RX
  GPIO_PinConfig(GPIO_RS485_DEN_PORT,   GPIO_RS485_DEN_PIN,
                 GPIO_RS485_DEN_SPEED,  GPIO_RS485_DEN_MODE);   //RS485 DEN
  GPIO_PinConfig(GPIO_DIO_01_PORT,      GPIO_DIO_01_PIN,
                 GPIO_DIO_01_SPEED,     GPIO_DIO_01_MODE);      //DIO1
  GPIO_PinConfig(GPIO_DIO_02_PORT,      GPIO_DIO_02_PIN,
                 GPIO_DIO_02_SPEED,     GPIO_DIO_02_MODE);      //DIO2
  GPIO_PinConfig(GPIO_DIO_03_PORT,      GPIO_DIO_03_PIN,
                 GPIO_DIO_03_SPEED,     GPIO_DIO_03_MODE);      //DIO3
  GPIO_PinConfig(GPIO_DIO_04_PORT,      GPIO_DIO_04_PIN,
                 GPIO_DIO_04_SPEED,     GPIO_DIO_04_MODE);      //DIO4
  GPIO_PinConfig(GPIO_DIO_05_PORT,      GPIO_DIO_05_PIN,
                 GPIO_DIO_05_SPEED,     GPIO_DIO_05_MODE);      //DIO5
  GPIO_PinConfig(GPIO_DIO_06_PORT,      GPIO_DIO_06_PIN,
                 GPIO_DIO_06_SPEED,     GPIO_DIO_06_MODE);      //DIO6
  GPIO_PinConfig(GPIO_DIO_07_PORT,      GPIO_DIO_07_PIN,
                 GPIO_DIO_07_SPEED,     GPIO_DIO_07_MODE);      //DIO7
  GPIO_PinConfig(GPIO_DIO_08_PORT,      GPIO_DIO_08_PIN,
                 GPIO_DIO_08_SPEED,     GPIO_DIO_08_MODE);      //DIO8
  GPIO_PinConfig(GPIO_DIO_09_PORT,      GPIO_DIO_09_PIN,
                 GPIO_DIO_09_SPEED,     GPIO_DIO_09_MODE);      //DIO9
  GPIO_PinConfig(GPIO_DIO_10_PORT,      GPIO_DIO_10_PIN,
                 GPIO_DIO_10_SPEED,     GPIO_DIO_10_MODE);      //DIO10
  GPIO_PinConfig(GPIO_DIO_11_PORT,      GPIO_DIO_11_PIN,
                 GPIO_DIO_11_SPEED,     GPIO_DIO_11_MODE);      //DIO11
  GPIO_PinConfig(GPIO_DIO_12_PORT,      GPIO_DIO_12_PIN,
                 GPIO_DIO_12_SPEED,     GPIO_DIO_12_MODE);      //DIO12
  GPIO_PinConfig(GPIO_DIO_13_PORT,      GPIO_DIO_13_PIN,
                 GPIO_DIO_13_SPEED,     GPIO_DIO_13_MODE);      //DIO13
  GPIO_PinConfig(GPIO_DIO_14_PORT,      GPIO_DIO_14_PIN,
                 GPIO_DIO_14_SPEED,     GPIO_DIO_14_MODE);      //DIO14
  GPIO_PinConfig(GPIO_DIO_15_PORT,      GPIO_DIO_15_PIN,
                 GPIO_DIO_15_SPEED,     GPIO_DIO_15_MODE);      //DIO15
  GPIO_PinConfig(GPIO_DIO_16_PORT,      GPIO_DIO_16_PIN,
                 GPIO_DIO_16_SPEED,     GPIO_DIO_16_MODE);      //DIO16
  
  
  // PIN REMAPS ================================================================
  GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
  
  
  // INITIAL OUTPUT LEVELS =====================================================
  GPIO_SetBits(GPIO_LED_1_PORT,GPIO_LED_1_PIN);           // LED RED OFF
  GPIO_SetBits(GPIO_LED_2_PORT,GPIO_LED_2_PIN);           // LED YEL OFF
  GPIO_SetBits(GPIO_LED_3_PORT,GPIO_LED_3_PIN);           // LED GRN OFF
  GPIO_SetBits(GPIO_LED_4_PORT,GPIO_LED_4_PIN);           // LED BLU OFF
  GPIO_ResetBits(GPIO_RS485_DEN_PORT,GPIO_RS485_DEN_PIN); // RS485DEN OFF
  GPIO_SetBits(GPIO_RS485_DEN_PORT,GPIO_RS485_DEN_PIN);   // RS485REN OFF
  GPIO_SetBits(GPIO_MN_NSS_PORT,GPIO_MN_NSS_PIN);         // Manage unselected
  GPIO_SetBits(GPIO_SD_NSS_PORT,GPIO_SD_NSS_PIN);         // SD Card unselected
  GPIO_SetBits(GPIO_CAN_SHDN_PORT,GPIO_CAN_SHDN_PIN);     // CAN SHUTDOWN MODE
  GPIO_SetBits(GPIO_CAN_STBY_PORT,GPIO_CAN_STBY_PIN);     // CAN STANDBY MODE
  
}


//==============================================================================
// FUNCTION GPIO_PinConfig()
//      - Configures a GPIO pin based on given peripheral, pin number, speed,
//              and mode
//==============================================================================
void GPIO_PinConfig(GPIO_TypeDef*       port,
                    uint16_t            pin, 
                    GPIOSpeed_TypeDef   speed,
                    GPIOMode_TypeDef    mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
  GPIO_InitStructure.GPIO_Pin   = pin;
  GPIO_InitStructure.GPIO_Speed = speed;
  GPIO_InitStructure.GPIO_Mode  = mode;
  GPIO_Init(port, &GPIO_InitStructure);
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
  
  // SD CARD
  RCC_APB2PeriphClockCmd(SPI_SD_CLK,ENABLE);
  SPI_I2S_DeInit(SPI_SD);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_SD_DATASIZE;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; // 48/32 = 1.5
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI_SD, &SPI_InitStructure);
  //SPI_I2S_DMACmd(SPI_SD, SPI_I2S_DMAReq_Tx, ENABLE);
  //SPI_I2S_DMACmd(SPI_SD, SPI_I2S_DMAReq_Rx, ENABLE);
  SPI_Cmd(SPI_SD, ENABLE);
  
  // MANAGE
  RCC_APB1PeriphClockCmd(SPI_MN_CLK,ENABLE);
  SPI_I2S_DeInit(SPI_MN);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_MN_DATASIZE;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI_MN, &SPI_InitStructure);
  SPI_I2S_DMACmd(SPI_MN, SPI_I2S_DMAReq_Tx, ENABLE);
  SPI_I2S_DMACmd(SPI_MN, SPI_I2S_DMAReq_Rx, ENABLE);
  SPI_Cmd(SPI_MN, ENABLE);
}


//==============================================================================
// FUNCTION USART_config()
//      - Configures STM32's USART peripheral
//      - 230400 Baud rate
//==============================================================================
void USART_config(void)
{
  USART_DeInit(USART_BT);
  
  // Declare USART initialization structure
  USART_InitTypeDef USART_InitStructure;
  
  // Start USART clock
  RCC_APB1PeriphClockCmd(USART_BT_CLK, ENABLE);
  
  // USART configuration
  USART_InitStructure.USART_BaudRate = USART_BT_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART_BT, &USART_InitStructure);
  
  // Enable USART TX and RX interrupts for DMA
  USART_DMACmd(USART_BT,USART_DMAReq_Tx, ENABLE);
  USART_DMACmd(USART_BT,USART_DMAReq_Rx, ENABLE);
    
  // Enable USART peripheral
  USART_Cmd(USART_BT, ENABLE);
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
  
  // Start DMA clock
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
  
  // SPI RECEIVE CHANNEL -------------------------------------------------------
  DMA_DeInit(SPI_SD_DMA_RX_CHAN);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI_SD_DR;
  //DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)spi_rx_buffer;               // CHANGE
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  //DMA_InitStructure.DMA_BufferSize = DATA_SIZE_FLEXSEA;                         // CHANGE
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(SPI_SD_DMA_RX_CHAN, &DMA_InitStructure);
  DMA_Cmd(SPI_SD_DMA_RX_CHAN, ENABLE);

  //DMA_ITConfig(SPI_SD_DMA_RX_CHAN, DMA_IT_TC, ENABLE);
  
   // SPI TRANSMIT CHANNEL -----------------------------------------------------
  DMA_DeInit(SPI_SD_DMA_TX_CHAN);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI_SD_DR;
  //DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)comm_str_1;                  // CHANGE
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  //DMA_InitStructure.DMA_BufferSize = DATA_SIZE_FLEXSEA;                         // CHANGE
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(SPI_SD_DMA_TX_CHAN, &DMA_InitStructure);
  DMA_Cmd(SPI_SD_DMA_TX_CHAN, ENABLE);
  //DMA_ITConfig(SPI_SD_DMA_TX_CHAN, DMA_IT_TC, ENABLE);

  // SPI RECEIVE CHANNEL -------------------------------------------------------
  DMA_DeInit(SPI_MN_DMA_RX_CHAN);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI_MN_DR;
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
  DMA_Init(SPI_MN_DMA_RX_CHAN, &DMA_InitStructure);
  DMA_ITConfig(SPI_MN_DMA_RX_CHAN, DMA_IT_TC, ENABLE);
  
   // SPI TRANSMIT CHANNEL -----------------------------------------------------
  DMA_DeInit(SPI_MN_DMA_TX_CHAN);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI_MN_DR;
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
  DMA_Init(SPI_MN_DMA_TX_CHAN, &DMA_InitStructure);
  DMA_ITConfig(SPI_MN_DMA_TX_CHAN, DMA_IT_TC, ENABLE);

    // USART RECEIVE CHANNEL -----------------------------------------------------
  DMA_DeInit(USART_BT_DMA_RX_CHAN);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART_BT_DR;
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
  DMA_Init(USART_BT_DMA_RX_CHAN, &DMA_InitStructure);
  DMA_ITConfig(USART_BT_DMA_RX_CHAN, DMA_IT_TC, ENABLE);
  DMA_Cmd(USART_BT_DMA_RX_CHAN, ENABLE);
  
  // USART TRANSMIT CHANNEL ----------------------------------------------------
  DMA_DeInit(USART_BT_DMA_TX_CHAN);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART_BT_DR;
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
  DMA_Init(USART_BT_DMA_TX_CHAN, &DMA_InitStructure);
  DMA_ITConfig(USART_BT_DMA_TX_CHAN, DMA_IT_TC, ENABLE); 
}


//==============================================================================
// FUNCTION NVIC_config()
//      - Configures STM32's nested vectored interrupt controller
//==============================================================================
void NVIC_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;				// Initialization structure
  
	// SPI RX interrupt for SD Card --------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel = DMA_SD_RX_IRQn;// Specify IRQ channel
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		// Enable the channel
	NVIC_Init(&NVIC_InitStructure);						// Implement above
  
	// SPI TX interrupt for SD Card --------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel = DMA_SD_TX_IRQn;// Specify IRQ channel
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		// Enable the channel
	NVIC_Init(&NVIC_InitStructure);						// Implement above
  
	// SPI RX interrupt for FlexSEA --------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel = DMA_MN_RX_IRQn;// Specify IRQ channel
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		// Enable the channel
	NVIC_Init(&NVIC_InitStructure);						// Implement above
  
	// SPI TX interrupt for FlexSEA --------------------------------------------
    NVIC_InitStructure.NVIC_IRQChannel = DMA_MN_TX_IRQn;// Specify IRQ channel
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		// Enable the channel
    NVIC_Init(&NVIC_InitStructure);						// Implement above

    // UART RX interrupt for BT121 ---------------------------------------------
    NVIC_InitStructure.NVIC_IRQChannel = DMA_BT_RX_IRQn;// Specify IRQ channel
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		// Enable the channel
    NVIC_Init(&NVIC_InitStructure);						// Implement above

    // UART TX interrupt for BT121 ---------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel = DMA_BT_TX_IRQn;// Specify IRQ channel
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		// Enable the channel
	NVIC_Init(&NVIC_InitStructure);						// Implement above

	// EXTI interrupt for Card Detect ------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel = EXTI_SD_CD_IRQn;	// Specify IRQ channel
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		// Enable the channel
	NVIC_Init(&NVIC_InitStructure);						// Implement above
  
	// TIM2 Update interrupt for LEDs ------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;		// Specify IRQ channel
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		// Enable the channel
	NVIC_Init(&NVIC_InitStructure);						// Implement above

	// RTC Second Update interrupt ---------------------------------------------
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		// Select priority grouping
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		// Specify IRQ channel
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	// Specify priority
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	// Specify subpriority
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		// Enable the channel
	NVIC_Init(&NVIC_InitStructure);						// Implement above

}


//==============================================================================
// FUNCTION RTC_config()
//      - Configures real time clock peripheral
//==============================================================================
void RTC_config(void)
{
	RCC_APB1PeriphClockCmd(								// Enable PWR and BKP clocks
		  RCC_APB1Periph_PWR |
		  RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);						// Allow access to BKP Domain
	BKP_DeInit();										// Reset Backup Domain
	RCC_LSEConfig(RCC_LSE_ON);							// Enable LSE
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);	// Wait: LSE is ready
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);				// Select LSE as RTC clock source
	RCC_RTCCLKCmd(ENABLE);								// Enable RIC clock
	RTC_WaitForSynchro();								// Wait: RTC synchronization
	RTC_WaitForLastTask();								// Wait: last write operation
	RTC_ITConfig(RTC_IT_SEC, ENABLE);					// Enable RTC second update
	RTC_WaitForLastTask();								// Wait: last write operation
	RTC_SetPrescaler(32767); 							// Set RTC period to 1 second
	RTC_WaitForLastTask();								// Wait: last write operation
  //RTC_SetCounter(19396); //5:23:16					// Set counter to specific time
    RTC_WaitForLastTask();								// Wait until last write operation
}


//==============================================================================
// FUNCTION RCC_config()
//      - Configures clocks
//==============================================================================
void RCC_config(void)
{
  RCC_PLLCmd(DISABLE);                          		// Disable PLL
  RCC_PLLConfig(RCC_PLLSOURCE, RCC_PLLMUL);     		// PLL: 8M/2*12 = 48M
  RCC_PLLCmd(ENABLE);                           		// Enable PLL
  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);		// Wait: PLL is ready
  FLASH_SetLatency(FLASH_Latency_1);            		// Increase FLASH latency
  RCC_HCLKConfig(RCC_SYSCLK_Div1);              		// HCLK configuration
  RCC_PCLK2Config(RCC_HCLK_Div1);               		// PCLK2 configuration
  RCC_PCLK1Config(RCC_HCLK_Div2);               		// PCLK1 configuration
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);    		// Set PLL as system clock source
  while (RCC_GetSYSCLKSource() != 0x08){;}      		// Wait: PLL is system clock source
  SystemCoreClockUpdate();                      		// Udpate variables
}


//==============================================================================
// FUNCTION EXTI_config()
//      - Configures sd card detect pin external interrupt
//==============================================================================
void EXTI_config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;				// Initialization structure

	// EXTI Card Detect --------------------------------------------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	// Enable AFIO clock
	GPIO_EXTILineConfig(EXTI_SD_CD_PORTSRS,				// Select GPIO to use
			EXTI_SD_CD_PINSRS);
	EXTI_InitStructure.EXTI_Line = EXTI_SD_CD_LINE;		// Specify EXTI line
	EXTI_InitStructure.EXTI_Mode = EXTI_SD_CD_MODE;		// Specify EXTI mode
	EXTI_InitStructure.EXTI_Trigger = EXTI_SD_CD_TRIG;	// Specify EXTI trigger type
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;			// Enable specified line
	EXTI_Init(&EXTI_InitStructure);						// Implement above
}


//==============================================================================
// FUNCTION TIM_config()
//      - configures various timers
//==============================================================================
void TIM_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	// Initialization structure

	// TIM2 --------------------------------------------------------------------
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);// Enable TIM2 clock
	TIM_TimeBaseInitStructure.TIM_CounterMode = 		// Specify counter mode
			TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;	// Specify clock division
	TIM_TimeBaseInitStructure.TIM_Prescaler =			// Specify prescaler
			LED_TIM_PRESCALER;
	TIM_TimeBaseInitStructure.TIM_Period = 				// Specify period
			LED_TIM_PERIOD;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure); // Implement above
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);			// Enable update interrupt
	TIM_Cmd(TIM2,ENABLE);								// Enable timer

	// TIM3 --------------------------------------------------------------------	//1kHz
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);// Enable TIM3 clock
	TIM_TimeBaseInitStructure.TIM_CounterMode = 		// Specify counter mode
			TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;	// Specify clock division
	TIM_TimeBaseInitStructure.TIM_Prescaler = 48000;	// Specify prescaler
	TIM_TimeBaseInitStructure.TIM_Period = 999;			// Specify period
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); // Implement above
	//TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);			// Enable update interrupt
	TIM_Cmd(TIM3,ENABLE);								// Enable timer
}

