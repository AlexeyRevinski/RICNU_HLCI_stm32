#ifndef PLAN_CONFIG_H
#define PLAN_CONFIG_H


// INCLUDES  ===================================================================

#include "plan_include.h"


// DEFINES  ==================================================================

#define SYSTICK                         50      //us
#define UPDATE_RATE                     TIMEBASE_250_HZ
#define DATA_SIZE_FLEXSEA               48      //bytes
#define DATA_SIZE_P2U                   25      //bytes
#define DATA_SIZE_U2P                   2       //bytes

#define SPI_USE_SOFTWARE_CONTROL


// MACROS  =====================================================================

#define SYSTEM_PERIOD_US (FACTOR_us_PER_s/UPDATE_RATE)


// NICKNAMES: SPI  =============================================================

#define SPIx                            SPI1

// GPIO-related nicknames
#define SPIx_GPIO_PORT                  GPIOA
#define SPIx_AF                         GPIO_AF_0
#define SPIx_NSS_PIN                    GPIO_Pin_4
#define SPIx_NSS_SOURCE                 GPIO_PinSource4
#define SPIx_SCK_PIN                    GPIO_Pin_5
#define SPIx_SCK_SOURCE                 GPIO_PinSource5
#define SPIx_MISO_PIN                   GPIO_Pin_6
#define SPIx_MISO_SOURCE                GPIO_PinSource6
#define SPIx_MOSI_PIN                   GPIO_Pin_7
#define SPIx_MOSI_SOURCE                GPIO_PinSource7

// NVIC-related nicknames
#define SPIx_IRQn                       SPI1_IRQn
#define SPIx_IRQHandler                 SPI1_IRQHandler

// DMA related nicknames
#define SPIx_DR_ADDRESS                 0x4001300C
#define SPIx_DMA_RX_CHANNEL             DMA1_Channel2
#define SPIx_DMA_TX_CHANNEL             DMA1_Channel3

// Other nicknames
#define SPI_MASTER
#define SPI_DATASIZE                    SPI_DataSize_8b


// NICKNAMES: USART  ===========================================================

#define USARTx                          USART1

// GPIO-related nicknames
#define USARTx_GPIO_PORT                GPIOA
#define USARTx_AF                       GPIO_AF_1
#define USARTx_TX_PIN                   GPIO_Pin_9
#define USARTx_TX_SOURCE                GPIO_PinSource9
#define USARTx_RX_PIN                   GPIO_Pin_10                   
#define USARTx_RX_SOURCE                GPIO_PinSource10

// NVIC-related nicknames
#define USARTx_IRQn                     USART1_IRQn
#define USARTx_IRQHandler               USART1_IRQHandler

// Data registers
#define USARTx_TDR_ADDRESS              0x40013828
#define USARTx_RDR_ADDRESS              0x40013824
#define USARTx_DMA_TX_CHANNEL           DMA1_Channel4
#define USARTx_DMA_RX_CHANNEL           DMA1_Channel5


// NICKNAMES: DMA  =============================================================

// NVIC-related nicknames
#define DMAx_SPIx_IRQn                  DMA1_Channel2_3_IRQn
#define DMAx_USARTx_IRQn                DMA1_Channel4_5_IRQn


// NICKNAMES: EXTI  ============================================================


#define CARD_DETECT_PIN                GPIO_Pin_0
#define CARD_DETECT_GPIO_PORT          GPIOA
#define CARD_DETECT_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define CARD_DETECT_EXTI_LINE          EXTI_Line0
#define CARD_DETECT_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOA
#define CARD_DETECT_EXTI_PIN_SOURCE    EXTI_PinSource0
#define CARD_DETECT_EXTI_IRQn          EXTI0_1_IRQn 



// FUNCTION PROTOTYPES  ========================================================

void DMA_config(void);
void EXTI_config(void);
void GPIO_config(void);
void NVIC_config(void);
void RCC_confg(void);
void RTC_config(void);
void SPI_config(void);
void SYSCFG_config(void);
void USART_config(void);

#endif //PLAN_CONFIG_H