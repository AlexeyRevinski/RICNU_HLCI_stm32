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

#define GPIO_SPEED_FAST                 GPIO_Speed_40MHz


// PB10 SPI SD ENABLE (TEMPORARY)                                                // TODO remove
#define GPIO_SPIx_EN_SD_PORT            GPIOB                   //M0: GPIOB
#define GPIO_SPIx_EN_SD_PIN             GPIO_Pin_10             //M0: 0
#define GPIO_SPIx_EN_SD_CLK             RCC_AHBPeriph_GPIOB
#define GPIO_SPIx_EN_SD_MODE            GPIO_Mode_OUT
#define GPIO_SPIx_EN_SD_OTYPE           GPIO_OType_PP
#define GPIO_SPIx_EN_SD_PUPD            GPIO_PuPd_NOPULL
#define GPIO_SPIx_EN_SD_SPEED           GPIO_SPEED_FAST 

// PB11 SPI MANAGE ENABLE (TEMPORARY)                                            //TODO remove
#define GPIO_SPIx_EN_MN_PORT            GPIOB                   //M0: GPIOB
#define GPIO_SPIx_EN_MN_PIN             GPIO_Pin_11             //M0: 1
#define GPIO_SPIx_EN_MN_CLK             RCC_AHBPeriph_GPIOB
#define GPIO_SPIx_EN_MN_MODE            GPIO_Mode_OUT
#define GPIO_SPIx_EN_MN_OTYPE           GPIO_OType_PP
#define GPIO_SPIx_EN_MN_PUPD            GPIO_PuPd_NOPULL
#define GPIO_SPIx_EN_MN_SPEED           GPIO_SPEED_FAST

// PA4  SPI Slave Select Manage pin     // Output, 50Mhz, push-pull
#define GPIO_SPIx_SS_MN_PORT            GPIOA
#define GPIO_SPIx_SS_MN_PIN             GPIO_Pin_4
#define GPIO_SPIx_SS_MN_CLK             RCC_AHBPeriph_GPIOA
#define GPIO_SPIx_SS_MN_MODE            GPIO_Mode_OUT
#define GPIO_SPIx_SS_MN_OTYPE           GPIO_OType_PP
#define GPIO_SPIx_SS_MN_PUPD            GPIO_PuPd_NOPULL
#define GPIO_SPIx_SS_MN_SPEED           GPIO_SPEED_FAST 

/* Second SPI bus: PA5,PA6,PA7
// PB13 SPI Clock pin                  //AF Output, push-pull, 40Mhz,
#define GPIO_SPIx_SCK_PORT              GPIOB                   //M0: GPIOA
#define GPIO_SPIx_SCK_PIN               GPIO_Pin_13             //M0: 5
#define GPIO_SPIx_SCK_CLK               RCC_AHBPeriph_GPIOB
#define GPIO_SPIx_SCK_MODE              GPIO_Mode_AF
#define GPIO_SPIx_SCK_OTYPE             GPIO_OType_PP
#define GPIO_SPIx_SCK_PUPD              GPIO_PuPd_NOPULL
#define GPIO_SPIx_SCK_SPEED             GPIO_SPEED_FAST 
#define GPIO_SPIx_SCK_SOURCE            GPIO_PinSource13
#define GPIO_SPIx_SCK_AF                GPIO_AF_SPI2            //M0: GPIO_AF_0 

// PB14 SPI Master In Slave Out pin     //AF Output, push-pull, 40Mhz,
#define GPIO_SPIx_MISO_PORT             GPIOB                   //M0: GPIOA
#define GPIO_SPIx_MISO_PIN              GPIO_Pin_14             //M0: 6
#define GPIO_SPIx_MISO_CLK              RCC_AHBPeriph_GPIOB
#define GPIO_SPIx_MISO_MODE             GPIO_Mode_AF
#define GPIO_SPIx_MISO_OTYPE            GPIO_OType_PP
#define GPIO_SPIx_MISO_PUPD             GPIO_PuPd_NOPULL
#define GPIO_SPIx_MISO_SPEED            GPIO_SPEED_FAST 
#define GPIO_SPIx_MISO_SOURCE           GPIO_PinSource14
#define GPIO_SPIx_MISO_AF               GPIO_AF_SPI2            //M0: GPIO_AF_0 

// PB15 SPI Master Out Slave In pin     //AF Output, push-pull, 40Mhz,
#define GPIO_SPIx_MOSI_PORT             GPIOB                   //M0: GPIOA
#define GPIO_SPIx_MOSI_PIN              GPIO_Pin_15             //M0: 7
#define GPIO_SPIx_MOSI_CLK              RCC_AHBPeriph_GPIOB
#define GPIO_SPIx_MOSI_MODE             GPIO_Mode_AF
#define GPIO_SPIx_MOSI_OTYPE            GPIO_OType_PP
#define GPIO_SPIx_MOSI_PUPD             GPIO_PuPd_NOPULL
#define GPIO_SPIx_MOSI_SPEED            GPIO_SPEED_FAST 
#define GPIO_SPIx_MOSI_SOURCE           GPIO_PinSource15
#define GPIO_SPIx_MOSI_AF               GPIO_AF_SPI2            //M0: GPIO_AF_0 
*/

// PB12  SPI Slave Select SD Card pin   // Output, 50Mhz, push-pull
#define GPIO_SPIx_SS_SD_PORT            GPIOB                   //M0: GPIOA
#define GPIO_SPIx_SS_SD_PIN             GPIO_Pin_12             //M0: 3
#define GPIO_SPIx_SS_SD_CLK             RCC_AHBPeriph_GPIOB
#define GPIO_SPIx_SS_SD_MODE            GPIO_Mode_OUT
#define GPIO_SPIx_SS_SD_OTYPE           GPIO_OType_PP
#define GPIO_SPIx_SS_SD_PUPD            GPIO_PuPd_NOPULL
#define GPIO_SPIx_SS_SD_SPEED           GPIO_SPEED_FAST 

// PB13 SPI Clock pin                  //AF Output, push-pull, 40Mhz,
#define GPIO_SPIx_SCK_PORT              GPIOB                   //M0: GPIOA
#define GPIO_SPIx_SCK_PIN               GPIO_Pin_13             //M0: 5
#define GPIO_SPIx_SCK_CLK               RCC_AHBPeriph_GPIOB
#define GPIO_SPIx_SCK_MODE              GPIO_Mode_AF
#define GPIO_SPIx_SCK_OTYPE             GPIO_OType_PP
#define GPIO_SPIx_SCK_PUPD              GPIO_PuPd_NOPULL
#define GPIO_SPIx_SCK_SPEED             GPIO_SPEED_FAST 
#define GPIO_SPIx_SCK_SOURCE            GPIO_PinSource13
#define GPIO_SPIx_SCK_AF                GPIO_AF_SPI2            //M0: GPIO_AF_0 

// PB14 SPI Master In Slave Out pin     //AF Output, push-pull, 40Mhz,
#define GPIO_SPIx_MISO_PORT             GPIOB                   //M0: GPIOA
#define GPIO_SPIx_MISO_PIN              GPIO_Pin_14             //M0: 6
#define GPIO_SPIx_MISO_CLK              RCC_AHBPeriph_GPIOB
#define GPIO_SPIx_MISO_MODE             GPIO_Mode_AF
#define GPIO_SPIx_MISO_OTYPE            GPIO_OType_PP
#define GPIO_SPIx_MISO_PUPD             GPIO_PuPd_NOPULL
#define GPIO_SPIx_MISO_SPEED            GPIO_SPEED_FAST 
#define GPIO_SPIx_MISO_SOURCE           GPIO_PinSource14
#define GPIO_SPIx_MISO_AF               GPIO_AF_SPI2            //M0: GPIO_AF_0 

// PB15 SPI Master Out Slave In pin     //AF Output, push-pull, 40Mhz,
#define GPIO_SPIx_MOSI_PORT             GPIOB                   //M0: GPIOA
#define GPIO_SPIx_MOSI_PIN              GPIO_Pin_15             //M0: 7
#define GPIO_SPIx_MOSI_CLK              RCC_AHBPeriph_GPIOB
#define GPIO_SPIx_MOSI_MODE             GPIO_Mode_AF
#define GPIO_SPIx_MOSI_OTYPE            GPIO_OType_PP
#define GPIO_SPIx_MOSI_PUPD             GPIO_PuPd_NOPULL
#define GPIO_SPIx_MOSI_SPEED            GPIO_SPEED_FAST 
#define GPIO_SPIx_MOSI_SOURCE           GPIO_PinSource15
#define GPIO_SPIx_MOSI_AF               GPIO_AF_SPI2            //M0: GPIO_AF_0 

// PA2 USART Tx pin                     //AF Output, push-pull, 40Mhz,
#define GPIO_USARTx_TX_PORT             GPIOA                   //M0: GPIOA
#define GPIO_USARTx_TX_PIN              GPIO_Pin_2              //M0: 9           
#define GPIO_USARTx_TX_CLK              RCC_AHBPeriph_GPIOA
#define GPIO_USARTx_TX_MODE             GPIO_Mode_AF
#define GPIO_USARTx_TX_OTYPE            GPIO_OType_PP
#define GPIO_USARTx_TX_PUPD             GPIO_PuPd_NOPULL
#define GPIO_USARTx_TX_SPEED            GPIO_SPEED_FAST 
#define GPIO_USARTx_TX_SOURCE           GPIO_PinSource9
#define GPIO_USARTx_TX_AF               GPIO_AF_USART2          //M0: GPIO_AF_1

// PA3 USART Rx pin                     //AF Output, push-pull, 40Mhz,
#define GPIO_USARTx_RX_PORT             GPIOA                   //M0: GPIOA
#define GPIO_USARTx_RX_PIN              GPIO_Pin_3              //M0: 10        
#define GPIO_USARTx_RX_CLK              RCC_AHBPeriph_GPIOA
#define GPIO_USARTx_RX_MODE             GPIO_Mode_AF
#define GPIO_USARTx_RX_OTYPE            GPIO_OType_PP
#define GPIO_USARTx_RX_PUPD             GPIO_PuPd_NOPULL
#define GPIO_USARTx_RX_SPEED            GPIO_SPEED_FAST 
#define GPIO_USARTx_RX_SOURCE           GPIO_PinSource10
#define GPIO_USARTx_RX_AF               GPIO_AF_USART2          //M0: GPIO_AF_1

// PA1  Card detect pin                 // Input
#define GPIO_EXTI_CD_PORT               GPIOA                   //M0: GPIOA
#define GPIO_EXTI_CD_PIN                GPIO_Pin_1              //M0: 2
#define GPIO_EXTI_CD_CLK                RCC_AHBPeriph_GPIOA
#define GPIO_EXTI_CD_MODE               GPIO_Mode_IN
#define GPIO_EXTI_CD_PUPD               GPIO_PuPd_UP

// PA0  User button pin                 // Input
#define GPIO_EXTI_UB_PORT               GPIOA
#define GPIO_EXTI_UB_PIN                GPIO_Pin_0
#define GPIO_EXTI_UB_CLK                RCC_AHBPeriph_GPIOA
#define GPIO_EXTI_UB_MODE               GPIO_Mode_IN
#define GPIO_EXTI_UB_PUPD               GPIO_PuPd_NOPULL

// DMA  ------------------------------------------------------------------------

#define DMAx_SPIx_Rx_IRQn               DMA1_Channel4_IRQn
#define DMAx_SPIx_Tx_IRQn               DMA1_Channel5_IRQn
//#define DMAx_SPIx_IRQn                  DMA1_Channel2_3_IRQn  //M0
#define DMAx_USARTx_Rx_IRQn             DMA1_Channel6_IRQn
#define DMAx_USARTx_Tx_IRQn             DMA1_Channel7_IRQn
//#define DMAx_USARTx_IRQn                DMA1_Channel4_5_IRQn  //M0

// EXTI  -----------------------------------------------------------------------

#define EXTI_UB_LINE                    EXTI_Line0
#define EXTI_UB_PORT_SOURCE             EXTI_PortSourceGPIOA
#define EXTI_UB_PIN_SOURCE              EXTI_PinSource0
#define EXTI_UB_IRQn                    EXTI0_IRQn

#define EXTI_CD_LINE                    EXTI_Line1              //M0: 2
#define EXTI_CD_PORT_SOURCE             EXTI_PortSourceGPIOA
#define EXTI_CD_PIN_SOURCE              EXTI_PinSource1
#define EXTI_CD_IRQn                    EXTI1_IRQn

// SPI  ------------------------------------------------------------------------

#define SPIx                            SPI2
#define SPIx_CLK                        RCC_APB1Periph_SPI2
#define SPIx_DMA_RX_CHANNEL             DMA1_Channel4           //M0: 2
#define SPIx_DMA_TX_CHANNEL             DMA1_Channel5           //M0: 3
//#define SPIx_DR_ADDRESS                 0x4001300C            //M0
#define SPIx_DR_ADDRESS                 0x4000380C
#define SPI_DATASIZE                    SPI_DataSize_8b

// USART  ----------------------------------------------------------------------

#define USARTx                          USART2
#define USARTx_CLK                      RCC_APB1Periph_USART2
#define USARTx_DMA_TX_CHANNEL           DMA1_Channel6           //M0: 4
#define USARTx_DMA_RX_CHANNEL           DMA1_Channel7           //M0: 5
//#define USARTx_TDR_ADDRESS              0x40013828            //M0
//#define USARTx_RDR_ADDRESS              0x40013824            //M0
#define USARTx_DR_ADDRESS               0x40004404



// FUNCTION PROTOTYPES  ========================================================

void DMA_config(void);
void EXTI_config(void);
void GPIO_config(void);
void NVIC_config(void);
void RTC_config(void);
void SPI_config(void);
void USART_config(void);

#endif //PLAN_CONFIG_H