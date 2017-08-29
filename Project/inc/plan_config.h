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

// NICKNAMES  ==================================================================

// GPIO PINS  ------------------------------------------------------------------

// PB0 SPI SD ENABLE (TEMPORARY)                                                // TODO remove
#define GPIO_SPIx_EN_SD_PORT            GPIOB
#define GPIO_SPIx_EN_SD_PIN             GPIO_Pin_0
#define GPIO_SPIx_EN_SD_CLK             RCC_AHBPeriph_GPIOB
#define GPIO_SPIx_EN_SD_MODE            GPIO_Mode_OUT
#define GPIO_SPIx_EN_SD_OTYPE           GPIO_OType_PP
#define GPIO_SPIx_EN_SD_PUPD            GPIO_PuPd_NOPULL
#define GPIO_SPIx_EN_SD_SPEED           GPIO_Speed_Level_3

// PB1 SPI MANAGE ENABLE (TEMPORARY)                                            //TODO remove
#define GPIO_SPIx_EN_MN_PORT            GPIOB
#define GPIO_SPIx_EN_MN_PIN             GPIO_Pin_1
#define GPIO_SPIx_EN_MN_CLK             RCC_AHBPeriph_GPIOB
#define GPIO_SPIx_EN_MN_MODE            GPIO_Mode_OUT
#define GPIO_SPIx_EN_MN_OTYPE           GPIO_OType_PP
#define GPIO_SPIx_EN_MN_PUPD            GPIO_PuPd_NOPULL
#define GPIO_SPIx_EN_MN_SPEED           GPIO_Speed_Level_3

// PA5  SPI Clock pin                   //AF Output, push-pull, 50Mhz,
#define GPIO_SPIx_SCK_PORT              GPIOA
#define GPIO_SPIx_SCK_PIN               GPIO_Pin_5
#define GPIO_SPIx_SCK_CLK               RCC_AHBPeriph_GPIOA
#define GPIO_SPIx_SCK_MODE              GPIO_Mode_AF
#define GPIO_SPIx_SCK_OTYPE             GPIO_OType_PP
#define GPIO_SPIx_SCK_PUPD              GPIO_PuPd_NOPULL
#define GPIO_SPIx_SCK_SPEED             GPIO_Speed_Level_3
#define GPIO_SPIx_SCK_SOURCE            GPIO_PinSource5
#define GPIO_SPIx_SCK_AF                GPIO_AF_0

// PA7  SPI Master Out Slave In pin     //AF Output, push-pull, 50Mhz,
#define GPIO_SPIx_MOSI_PORT             GPIOA
#define GPIO_SPIx_MOSI_PIN              GPIO_Pin_7
#define GPIO_SPIx_MOSI_CLK              RCC_AHBPeriph_GPIOA
#define GPIO_SPIx_MOSI_MODE             GPIO_Mode_AF
#define GPIO_SPIx_MOSI_OTYPE            GPIO_OType_PP
#define GPIO_SPIx_MOSI_PUPD             GPIO_PuPd_NOPULL
#define GPIO_SPIx_MOSI_SPEED            GPIO_Speed_Level_3
#define GPIO_SPIx_MOSI_SOURCE           GPIO_PinSource7
#define GPIO_SPIx_MOSI_AF               GPIO_AF_0

// PA6  SPI Master In Slave Out pin     //AF Output, push-pull, 50Mhz,
#define GPIO_SPIx_MISO_PORT             GPIOA
#define GPIO_SPIx_MISO_PIN              GPIO_Pin_6
#define GPIO_SPIx_MISO_CLK              RCC_AHBPeriph_GPIOA
#define GPIO_SPIx_MISO_MODE             GPIO_Mode_AF
#define GPIO_SPIx_MISO_OTYPE            GPIO_OType_PP
#define GPIO_SPIx_MISO_PUPD             GPIO_PuPd_NOPULL
#define GPIO_SPIx_MISO_SPEED            GPIO_Speed_Level_3
#define GPIO_SPIx_MISO_SOURCE           GPIO_PinSource6
#define GPIO_SPIx_MISO_AF               GPIO_AF_0

// PA4  SPI Slave Select Manage pin     // Output, 50Mhz, push-pull
#define GPIO_SPIx_SS_MN_PORT            GPIOA
#define GPIO_SPIx_SS_MN_PIN             GPIO_Pin_4
#define GPIO_SPIx_SS_MN_CLK             RCC_AHBPeriph_GPIOA
#define GPIO_SPIx_SS_MN_MODE            GPIO_Mode_OUT
#define GPIO_SPIx_SS_MN_OTYPE           GPIO_OType_PP
#define GPIO_SPIx_SS_MN_PUPD            GPIO_PuPd_NOPULL
#define GPIO_SPIx_SS_MN_SPEED           GPIO_Speed_Level_3

// PA3  SPI Slave Select SD Card pin     // Output, 50Mhz, push-pull
#define GPIO_SPIx_SS_SD_PORT            GPIOA
#define GPIO_SPIx_SS_SD_PIN             GPIO_Pin_3
#define GPIO_SPIx_SS_SD_CLK             RCC_AHBPeriph_GPIOA
#define GPIO_SPIx_SS_SD_MODE            GPIO_Mode_OUT
#define GPIO_SPIx_SS_SD_OTYPE           GPIO_OType_PP
#define GPIO_SPIx_SS_SD_PUPD            GPIO_PuPd_NOPULL
#define GPIO_SPIx_SS_SD_SPEED           GPIO_Speed_Level_3

// PA9 USART Tx pin                     //AF Output, push-pull, 50Mhz,
#define GPIO_USARTx_TX_PORT             GPIOA
#define GPIO_USARTx_TX_PIN              GPIO_Pin_9                   
#define GPIO_USARTx_TX_CLK              RCC_AHBPeriph_GPIOA
#define GPIO_USARTx_TX_MODE             GPIO_Mode_AF
#define GPIO_USARTx_TX_OTYPE            GPIO_OType_PP
#define GPIO_USARTx_TX_PUPD             GPIO_PuPd_NOPULL
#define GPIO_USARTx_TX_SPEED            GPIO_Speed_Level_3
#define GPIO_USARTx_TX_SOURCE           GPIO_PinSource9
#define GPIO_USARTx_TX_AF               GPIO_AF_1

// PA10 USART Rx pin                    //AF Output, push-pull, 50Mhz,
#define GPIO_USARTx_RX_PORT             GPIOA
#define GPIO_USARTx_RX_PIN              GPIO_Pin_10                   
#define GPIO_USARTx_RX_CLK              RCC_AHBPeriph_GPIOA
#define GPIO_USARTx_RX_MODE             GPIO_Mode_AF
#define GPIO_USARTx_RX_OTYPE            GPIO_OType_PP
#define GPIO_USARTx_RX_PUPD             GPIO_PuPd_NOPULL
#define GPIO_USARTx_RX_SPEED            GPIO_Speed_Level_3
#define GPIO_USARTx_RX_SOURCE           GPIO_PinSource10
#define GPIO_USARTx_RX_AF               GPIO_AF_1

// PA2  Card detect pin                 // Input, 50Mhz
#define GPIO_EXTI_CD_PORT               GPIOA
#define GPIO_EXTI_CD_PIN                GPIO_Pin_2
#define GPIO_EXTI_CD_CLK                RCC_AHBPeriph_GPIOA
#define GPIO_EXTI_CD_MODE               GPIO_Mode_IN
#define GPIO_EXTI_CD_PUPD               GPIO_PuPd_UP

// PA0  User button pin                 // Input, 50Mhz
#define GPIO_EXTI_UB_PORT               GPIOA
#define GPIO_EXTI_UB_PIN                GPIO_Pin_0
#define GPIO_EXTI_UB_CLK                RCC_AHBPeriph_GPIOA
#define GPIO_EXTI_UB_MODE               GPIO_Mode_IN
#define GPIO_EXTI_UB_PUPD               GPIO_PuPd_NOPULL

// DMA  ------------------------------------------------------------------------

#define DMAx_SPIx_IRQn                  DMA1_Channel2_3_IRQn
#define DMAx_USARTx_IRQn                DMA1_Channel4_5_IRQn

// EXTI  -----------------------------------------------------------------------

#define EXTI_UB_LINE                    EXTI_Line0
#define EXTI_UB_PORT_SOURCE             EXTI_PortSourceGPIOA
#define EXTI_UB_PIN_SOURCE              EXTI_PinSource0
#define EXTI_UB_IRQn                    EXTI0_1_IRQn 

#define EXTI_CD_LINE                    EXTI_Line2
#define EXTI_CD_PORT_SOURCE             EXTI_PortSourceGPIOA
#define EXTI_CD_PIN_SOURCE              EXTI_PinSource2
#define EXTI_CD_IRQn                    EXTI2_3_IRQn

// SPI  ------------------------------------------------------------------------

#define SPIx                            SPI1
#define SPIx_IRQn                       SPI1_IRQn
#define SPIx_IRQHandler                 SPI1_IRQHandler
#define SPIx_DMA_RX_CHANNEL             DMA1_Channel2
#define SPIx_DMA_TX_CHANNEL             DMA1_Channel3
#define SPIx_DR_ADDRESS                 0x4001300C
#define SPI_DATASIZE                    SPI_DataSize_8b

// USART  ----------------------------------------------------------------------

#define USARTx                          USART1
#define USARTx_IRQn                     USART1_IRQn
#define USARTx_IRQHandler               USART1_IRQHandler
#define USARTx_DMA_TX_CHANNEL           DMA1_Channel4
#define USARTx_DMA_RX_CHANNEL           DMA1_Channel5
#define USARTx_TDR_ADDRESS              0x40013828
#define USARTx_RDR_ADDRESS              0x40013824


// FUNCTION PROTOTYPES  ========================================================

void DMA_config(void);
void EXTI_config(void);
void GPIO_config(void);
void NVIC_config(void);
void RTC_config(void);
void SPI_config(void);
void USART_config(void);

#endif //PLAN_CONFIG_H