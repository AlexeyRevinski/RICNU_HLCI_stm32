#ifndef PLAN_CONFIG_H
#define PLAN_CONFIG_H


// INCLUDES  ===================================================================

#include "plan_include.h"

/*
===================================================================
         RICNU Plan STM32F103RD Default Pin Configuration         |
===================================================================
 Pin | Name  | Function   | Default | Port | Pin | Mode   | Remap | DONE?
===================================================================
 1   | VBAT  | PWR        | YES     |      |     |        |       |
 2   | PC13  | not used   |         |  C   | 13  | ???    |       | Check mode - maybe need to do something
 3   | PC14  | OSC_32_IN  | YES     |  C   | 14  |        |       |
 4   | PC15  | OSC_32_OUT | YES     |  C   | 15  |        |       |
 5   | PD0   | not used   |         |  D   | 0   | ???    |       | Check mode - maybe need to do something 
 6   | PD1   | not used   |         |  D   | 1   | ???    |       | Check mode - maybe need to do something
 7   | NRST  | NRST       | YES     |      |     |        |       |
 8   | PC0   | CAN_SHDN   |         |  C   | 0   | Out_PP |       |
 9   | PC1   | CAN_STBY   |         |  C   | 1   | Out_PP |       |
 10  | PC2   | not used   |         |  C   | 2   | ???    |       |
 11  | PC3   | not used   |         |  C   | 3   | ???    |       |
 12  | VSSA  | PWR        | YES     |      |     |        |       |
 13  | VDDA  | PWR        | YES     |      |     |        |       |
 14  | PA0   | USR        |         |  A   | 0   | In_PU  |       |
 15  | PA1   | LED4 DRIVE |         |  A   | 1   | Out_PP |       |
 16  | PA2   | BTMOD_TX   |         |  A   | 2   | AF_PP  |       |  Check mode
 17  | PA3   | BTMOD_RX   |         |  A   | 3   | In_PU  |       |  Check mode
 18  | VSS4  | PWR        | YES     |      |     |        |       |
 19  | VDD4  | PWR        | YES     |      |     |        |       |
 20  | PA4   | SD_NSS     |         |  A   | 4   | Out_PP |       |
 21  | PA5   | SD_SCK     |         |  A   | 5   | AF_PP  |       |
 22  | PA6   | SD_MISO    |         |  A   | 6   | In_PU  |       |  Check mode
 23  | PA7   | SD_MOSI    |         |  A   | 7   | AF_PP  |       |
 24  | PC4   | SD_CD      |         |  C   | 4   | In_PU  |       |
 25  | PC5   | LED1 DRIVE |         |  C   | 5   | Out_PP |       |
 26  | PB0   | LED2 DRIVE |         |  B   | 0   | Out_PP |       |
 27  | PB1   | LED3 DRIVE |         |  B   | 1   | Out_PP |       |
 28  | PB2   | BOOT1      | ???     |  B   | 2   | ???    |       |
 29  | PB10  | USR        |         |  B   | 10  | In_PU  |       |
 30  | PB11  | USR        |         |  B   | 11  | In_PU  |       |
 31  | VSS1  | PWR        | YES     |      |     |        |       |
 32  | VDD1  | PWR        | YES     |      |     |        |       |
 33  | PB12  | MN_NSS     |         |  B   | 12  | Out_PP |       |
 34  | PB13  | MN_SCK     |         |  B   | 13  | AF_PP  |       |
 35  | PB14  | MN_MISO    |         |  B   | 14  | In_PU  |       |  Check mode
 36  | PB15  | MN_MOSI    |         |  B   | 15  | AF_PP  |       |
 37  | PC6   | USR        |         |  C   | 6   | In_PU  |       |
 38  | PC7   | USR        |         |  C   | 7   | In_PU  |       |
 39  | PC8   | FTDI_ENUM  |         |  C   | 8   | In_PU  |       |  Check mode
 40  | PC9   | FTDI_PWR   |         |  C   | 9   | In_PU  |       |  Check mode
 41  | PA8   | USR        |         |  A   | 8   | In_PU  |       | 
 42  | PA9   | USR        |         |  A   | 9   | In_PU  |       |
 43  | PA10  | USR        |         |  A   | 10  | In_PU  |       |
 44  | PA11  | USR        |         |  A   | 11  | In_PU  |       |
 45  | PA12  | USR        |         |  A   | 12  | In_PU  |       |
 46  | PA13  | SWDIO	  | YES     |  A   | 13  | ???    |       |
 47  | VSS2  | PWR        | YES     |      |     |        |       |
 48  | VDD2  | PWR        | YES     |      |     |        |       |
 49  | PA14  | SWCLK	  | YES     |  A   | 14  | ???    |       |
 50  | PA15  | RS485_REN  |         |  A   | 15  | Out_PP |       |
 51  | PC10  | RS485_TX   |         |  C   | 10  | AF_PP  |       |  Check mode
 52  | PC11  | RS485_RX   |         |  C   | 11  | In_PU  |       |  Check mode
 53  | PC12  | RS485_DEN  |         |  C   | 12  | Out_PP |       |
 54  | PD2   | USR        |         |  D   | 2   | In_PU  |       |
 55  | PB3   | USR        |         |  B   | 3   | In_PD  |       |  PD externally
 56  | PB4   | USR        |         |  B   | 4   | In_PU  |       |
 57  | PB5   | USR        |         |  B   | 5   | In_PU  |       |
 58  | PB6   | USR        |         |  B   | 6   | In_PU  |       |
 59  | PB7   | USR        |         |  B   | 7   | In_PU  |       |
 60  | BOOT0 | BOOT       | YES     |      |     |        |       |
 61  | PB8   | CAN_RX     |         |  B   | 8   | In_PU  | Yes   |
 62  | PB9   | CAN_TX     |         |  B   | 9   | AF_PP  | Yes   |
 63  | VSS3  | PWR        | YES     |      |     |        |       |
 64  | VDD3  | PWR        | YES     |      |     |        |       |
 ==================================================================

*/

// DEFINES       =============================================================
#define TIMEBASE_100_HZ                 100     //Hz
#define TIMEBASE_250_HZ                 250     //Hz
#define TIMEBASE_500_HZ                 500     //Hz
#define FACTOR_us_PER_s                 1000000 //us per 1s

#define SYS_TICK_US                     50                              //us
#define SYS_UPDFRQ_HZ                   TIMEBASE_100_HZ                 // System update
#define SYS_PERIOD_US                   (FACTOR_us_PER_s/SYS_UPDFRQ_HZ)   // Update period
#define GPIO_Speed_FAST                 GPIO_Speed_50MHz

#define RCC_PLLSOURCE                   RCC_PLLSource_HSI_Div2
#define RCC_PLLMUL                      RCC_PLLMul_12


// 8    PC0     CAN                     Shutdown Control
#define GPIO_CAN_SHDN_PORT              GPIOC
#define GPIO_CAN_SHDN_PIN               GPIO_Pin_0
#define GPIO_CAN_SHDN_SPEED             GPIO_Speed_FAST
#define GPIO_CAN_SHDN_MODE              GPIO_Mode_Out_PP

// 9    PC1     CAN                     Standby Control
#define GPIO_CAN_STBY_PORT              GPIOC
#define GPIO_CAN_STBY_PIN               GPIO_Pin_1
#define GPIO_CAN_STBY_SPEED             GPIO_Speed_FAST
#define GPIO_CAN_STBY_MODE              GPIO_Mode_Out_PP

// 61   PB8     CAN                     Receive
#define GPIO_CAN_RX_PORT                GPIOB
#define GPIO_CAN_RX_PIN                 GPIO_Pin_8
#define GPIO_CAN_RX_SPEED               GPIO_Speed_FAST
#define GPIO_CAN_RX_MODE                GPIO_Mode_IPU           // Check mode

// 62   PB9     CAN                     Transmit
#define GPIO_CAN_TX_PORT                GPIOB
#define GPIO_CAN_TX_PIN                 GPIO_Pin_9
#define GPIO_CAN_TX_SPEED               GPIO_Speed_FAST
#define GPIO_CAN_TX_MODE                GPIO_Mode_AF_PP         // Check mode

// 25   PC5     LED                     1 RED                   ERROR           // a bit too bright
#define GPIO_LED_1_PORT                 GPIOC
#define GPIO_LED_1_PIN                  GPIO_Pin_5
#define GPIO_LED_1_SPEED                GPIO_Speed_FAST
#define GPIO_LED_1_MODE                 GPIO_Mode_Out_PP

// 26   PB0     LED                     2 YELLOW                CONTROL         // OKAY
#define GPIO_LED_2_PORT                 GPIOB
#define GPIO_LED_2_PIN                  GPIO_Pin_0
#define GPIO_LED_2_SPEED                GPIO_Speed_FAST
#define GPIO_LED_2_MODE                 GPIO_Mode_Out_PP

// 27   PB1     LED                     3 GREEN                 HEARTBEAT       // TOO BRIGHT
#define GPIO_LED_3_PORT                 GPIOB
#define GPIO_LED_3_PIN                  GPIO_Pin_1
#define GPIO_LED_3_SPEED                GPIO_Speed_FAST
#define GPIO_LED_3_MODE                 GPIO_Mode_Out_PP

// 15   PA1     LED                     4 BLUE                  DATA LOGGING    // OKAY
#define GPIO_LED_4_PORT                 GPIOA
#define GPIO_LED_4_PIN                  GPIO_Pin_1
#define GPIO_LED_4_SPEED                GPIO_Speed_FAST
#define GPIO_LED_4_MODE                 GPIO_Mode_Out_PP

// 16   PA2     Bluetooth Module        UART Transmit
#define GPIO_BT_TX_PORT                 GPIOA
#define GPIO_BT_TX_PIN                  GPIO_Pin_2
#define GPIO_BT_TX_SPEED                GPIO_Speed_FAST
#define GPIO_BT_TX_MODE                 GPIO_Mode_AF_PP

// 17   PA3     Bluetooth Module        UART Receive
#define GPIO_BT_RX_PORT                 GPIOA
#define GPIO_BT_RX_PIN                  GPIO_Pin_3
#define GPIO_BT_RX_SPEED                GPIO_Speed_FAST
#define GPIO_BT_RX_MODE                 GPIO_Mode_IPU

// 20   PA4     SD Card                 SPI Slave Select
#define GPIO_SD_NSS_PORT                GPIOA
#define GPIO_SD_NSS_PIN                 GPIO_Pin_4
#define GPIO_SD_NSS_SPEED               GPIO_Speed_FAST
#define GPIO_SD_NSS_MODE                GPIO_Mode_Out_PP

// 21   PA5     SD Card                 SPI Clock Line
#define GPIO_SD_SCK_PORT                GPIOA
#define GPIO_SD_SCK_PIN                 GPIO_Pin_5
#define GPIO_SD_SCK_SPEED               GPIO_Speed_FAST
#define GPIO_SD_SCK_MODE                GPIO_Mode_AF_PP

// 22   PA6     SD Card                 SPI Master-In-Slave-Out Line
#define GPIO_SD_MISO_PORT               GPIOA
#define GPIO_SD_MISO_PIN                GPIO_Pin_6
#define GPIO_SD_MISO_SPEED              GPIO_Speed_FAST
#define GPIO_SD_MISO_MODE               GPIO_Mode_IPU

// 23   PA7     SD Card                 SPI Master-Out-Slave-In Line
#define GPIO_SD_MOSI_PORT               GPIOA
#define GPIO_SD_MOSI_PIN                GPIO_Pin_7
#define GPIO_SD_MOSI_SPEED              GPIO_Speed_FAST
#define GPIO_SD_MOSI_MODE               GPIO_Mode_AF_PP

// 24   PC4     SD Card                 Card Detect
#define GPIO_SD_CD_PORT                 GPIOC
#define GPIO_SD_CD_PIN                  GPIO_Pin_4
#define GPIO_SD_CD_SPEED                GPIO_Speed_FAST
#define GPIO_SD_CD_MODE                 GPIO_Mode_IPU

// 33   PB12     FlexSEA Manage         SPI Slave Select
#define GPIO_MN_NSS_PORT                GPIOB
#define GPIO_MN_NSS_PIN                 GPIO_Pin_12
#define GPIO_MN_NSS_SPEED               GPIO_Speed_FAST
#define GPIO_MN_NSS_MODE                GPIO_Mode_Out_PP

// 34   PB13     FlexSEA Manage         SPI Clock Line
#define GPIO_MN_SCK_PORT                GPIOB
#define GPIO_MN_SCK_PIN                 GPIO_Pin_13
#define GPIO_MN_SCK_SPEED               GPIO_Speed_FAST
#define GPIO_MN_SCK_MODE                GPIO_Mode_AF_PP

// 35   PB14     FlexSEA Manage         SPI Master-In-Slave-Out Line
#define GPIO_MN_MISO_PORT               GPIOB
#define GPIO_MN_MISO_PIN                GPIO_Pin_14
#define GPIO_MN_MISO_SPEED              GPIO_Speed_FAST
#define GPIO_MN_MISO_MODE               GPIO_Mode_IPU

// 36   PB15     FlexSEA Manage         SPI Master-Out-Slave-In Line
#define GPIO_MN_MOSI_PORT               GPIOB
#define GPIO_MN_MOSI_PIN                GPIO_Pin_15
#define GPIO_MN_MOSI_SPEED              GPIO_Speed_FAST
#define GPIO_MN_MOSI_MODE               GPIO_Mode_AF_PP

// 39   PC8     FTDI                    Enumeration Complete input
#define GPIO_FTDI_ENUM_PORT             GPIOC
#define GPIO_FTDI_ENUM_PIN              GPIO_Pin_8
#define GPIO_FTDI_ENUM_SPEED            GPIO_Speed_FAST
#define GPIO_FTDI_ENUM_MODE             GPIO_Mode_IPU

// 40   PC9     FTDI                    USB Power Available
#define GPIO_FTDI_PWR_PORT              GPIOC
#define GPIO_FTDI_PWR_PIN               GPIO_Pin_9
#define GPIO_FTDI_PWR_SPEED             GPIO_Speed_FAST
#define GPIO_FTDI_PWR_MODE              GPIO_Mode_IPU

// 50   PA15    RS485                   Receiver Enable
#define GPIO_RS485_REN_PORT             GPIOA
#define GPIO_RS485_REN_PIN              GPIO_Pin_15
#define GPIO_RS485_REN_SPEED            GPIO_Speed_FAST
#define GPIO_RS485_REN_MODE             GPIO_Mode_Out_PP

// 51   PC10    RS485                   Transmit
#define GPIO_RS485_TX_PORT              GPIOC
#define GPIO_RS485_TX_PIN               GPIO_Pin_10
#define GPIO_RS485_TX_SPEED             GPIO_Speed_FAST
#define GPIO_RS485_TX_MODE              GPIO_Mode_AF_PP

// 52   PC11    RS485                   Receive
#define GPIO_RS485_RX_PORT              GPIOC
#define GPIO_RS485_RX_PIN               GPIO_Pin_11
#define GPIO_RS485_RX_SPEED             GPIO_Speed_FAST
#define GPIO_RS485_RX_MODE              GPIO_Mode_IPU

// 53   PC12    RS485                   Driver Enable
#define GPIO_RS485_DEN_PORT             GPIOC
#define GPIO_RS485_DEN_PIN              GPIO_Pin_12
#define GPIO_RS485_DEN_SPEED            GPIO_Speed_FAST
#define GPIO_RS485_DEN_MODE             GPIO_Mode_Out_PP

// 14   PA0     Digital I/O             1
#define GPIO_DIO_01_PORT                GPIOA
#define GPIO_DIO_01_PIN                 GPIO_Pin_0
#define GPIO_DIO_01_SPEED               GPIO_Speed_FAST
#define GPIO_DIO_01_MODE                GPIO_Mode_IPU

// 42   PA9     Digital I/O             2
#define GPIO_DIO_02_PORT                GPIOA
#define GPIO_DIO_02_PIN                 GPIO_Pin_9
#define GPIO_DIO_02_SPEED               GPIO_Speed_FAST
#define GPIO_DIO_02_MODE                GPIO_Mode_IPU

// 41   PA8     Digital I/O             3       MCO
#define GPIO_DIO_03_PORT                GPIOA
#define GPIO_DIO_03_PIN                 GPIO_Pin_8
#define GPIO_DIO_03_SPEED               GPIO_Speed_FAST
#define GPIO_DIO_03_MODE                GPIO_Mode_IPU

// 43   PA10    Digital I/O             4
#define GPIO_DIO_04_PORT                GPIOA
#define GPIO_DIO_04_PIN                 GPIO_Pin_10
#define GPIO_DIO_04_SPEED               GPIO_Speed_FAST
#define GPIO_DIO_04_MODE                GPIO_Mode_IPU

// 38   PC7     Digital I/O             5
#define GPIO_DIO_05_PORT                GPIOC
#define GPIO_DIO_05_PIN                 GPIO_Pin_7
#define GPIO_DIO_05_SPEED               GPIO_Speed_FAST
#define GPIO_DIO_05_MODE                GPIO_Mode_IPU

// 44   PA11    Digital I/O             6
#define GPIO_DIO_06_PORT                GPIOA
#define GPIO_DIO_06_PIN                 GPIO_Pin_11
#define GPIO_DIO_06_SPEED               GPIO_Speed_FAST
#define GPIO_DIO_06_MODE                GPIO_Mode_IPU

// 37   PC6     Digital I/O             7
#define GPIO_DIO_07_PORT                GPIOC
#define GPIO_DIO_07_PIN                 GPIO_Pin_6
#define GPIO_DIO_07_SPEED               GPIO_Speed_FAST
#define GPIO_DIO_07_MODE                GPIO_Mode_IPU

// 45   PA12    Digital I/O             8
#define GPIO_DIO_08_PORT                GPIOA
#define GPIO_DIO_08_PIN                 GPIO_Pin_12
#define GPIO_DIO_08_SPEED               GPIO_Speed_FAST
#define GPIO_DIO_08_MODE                GPIO_Mode_IPU

// 59   PB7     Digital I/O             9
#define GPIO_DIO_09_PORT                GPIOB
#define GPIO_DIO_09_PIN                 GPIO_Pin_7
#define GPIO_DIO_09_SPEED               GPIO_Speed_FAST
#define GPIO_DIO_09_MODE                GPIO_Mode_IPU

// 54   PD2     Digital I/O             10
#define GPIO_DIO_10_PORT                GPIOD
#define GPIO_DIO_10_PIN                 GPIO_Pin_2
#define GPIO_DIO_10_SPEED               GPIO_Speed_FAST
#define GPIO_DIO_10_MODE                GPIO_Mode_IPU

// 58   PB6     Digital I/O             11
#define GPIO_DIO_11_PORT                GPIOB
#define GPIO_DIO_11_PIN                 GPIO_Pin_6
#define GPIO_DIO_11_SPEED               GPIO_Speed_FAST
#define GPIO_DIO_11_MODE                GPIO_Mode_IPU

// 55   PB3     Digital I/O             12
#define GPIO_DIO_12_PORT                GPIOB
#define GPIO_DIO_12_PIN                 GPIO_Pin_3
#define GPIO_DIO_12_SPEED               GPIO_Speed_FAST
#define GPIO_DIO_12_MODE                GPIO_Mode_IPD   // External pull down as well

// 30   PB11    Digital I/O             13
#define GPIO_DIO_13_PORT                GPIOB
#define GPIO_DIO_13_PIN                 GPIO_Pin_11
#define GPIO_DIO_13_SPEED               GPIO_Speed_FAST
#define GPIO_DIO_13_MODE                GPIO_Mode_IPU

// 56   PB4     Digital I/O             14
#define GPIO_DIO_14_PORT                GPIOB
#define GPIO_DIO_14_PIN                 GPIO_Pin_4
#define GPIO_DIO_14_SPEED               GPIO_Speed_FAST
#define GPIO_DIO_14_MODE                GPIO_Mode_IPU

// 29   PB10    Digital I/O             15
#define GPIO_DIO_15_PORT                GPIOB
#define GPIO_DIO_15_PIN                 GPIO_Pin_10
#define GPIO_DIO_15_SPEED               GPIO_Speed_FAST
#define GPIO_DIO_15_MODE                GPIO_Mode_IPU

// 57   PB5     Digital I/O             16
#define GPIO_DIO_16_PORT                GPIOB
#define GPIO_DIO_16_PIN                 GPIO_Pin_5
#define GPIO_DIO_16_SPEED               GPIO_Speed_FAST
#define GPIO_DIO_16_MODE                GPIO_Mode_IPU

//==============================================================================


/*
 2   | PC13  | not used   |         |  C   | 13  | ???    |       | Check mode - maybe need to do something
 3   | PC14  | OSC_32_IN  | YES     |  C   | 14  |        |       |
 4   | PC15  | OSC_32_OUT | YES     |  C   | 15  |        |       |
 5   | PD0   | not used   |         |  D   | 0   | ???    |       | Check mode - maybe need to do something 
 6   | PD1   | not used   |         |  D   | 1   | ???    |       | Check mode - maybe need to do something
 10  | PC2   | not used   |         |  C   | 2   | ???    |       |
 11  | PC3   | not used   |         |  C   | 3   | ???    |       |
 46  | PA13  | SWDIO	  | YES     |  A   | 13  | ???    |       |
 49  | PA14  | SWCLK	  | YES     |  A   | 14  | ???    |       |
 60  | BOOT0 | BOOT       | YES     |      |     |        |       |
 28  | PB2   | BOOT1      | ???     |  B   | 2   | ???    |       |

*/


// EXTI:        Card Detect

#define EXTI_SD_CD_LINE         EXTI_Line4
#define EXTI_SD_CD_MODE         EXTI_Mode_Interrupt
#define EXTI_SD_CD_TRIG         EXTI_Trigger_Rising_Falling
#define EXTI_SD_CD_IRQn         EXTI4_IRQn
#define EXTI_SD_CD_PORTSRS      GPIO_PortSourceGPIOC
#define EXTI_SD_CD_PINSRS       GPIO_PinSource4

// DMA Interrupts

#define DMA_SD_RX_IRQn          DMA1_Channel2_IRQn
#define DMA_SD_TX_IRQn          DMA1_Channel3_IRQn
#define DMA_MN_RX_IRQn          DMA1_Channel4_IRQn
#define DMA_MN_TX_IRQn          DMA1_Channel5_IRQn
#define DMA_BT_RX_IRQn          DMA1_Channel6_IRQn
#define DMA_BT_TX_IRQn          DMA1_Channel7_IRQn

// SPI:         SD Card

#define SPI_SD                  SPI1
#define SPI_SD_CLK              RCC_APB2Periph_SPI1
#define SPI_SD_DMA_RX_CHAN      DMA1_Channel2
#define SPI_SD_DMA_TX_CHAN      DMA1_Channel3
#define SPI_SD_DR               0x4001300C
#define SPI_SD_DATASIZE         SPI_DataSize_8b

// SPI:         FlexSEA Manage

#define SPI_MN                  SPI2
#define SPI_MN_CLK              RCC_APB1Periph_SPI2
#define SPI_MN_DMA_RX_CHAN      DMA1_Channel4
#define SPI_MN_DMA_TX_CHAN      DMA1_Channel5
#define SPI_MN_DR               0x4000380C
#define SPI_MN_DATASIZE         SPI_DataSize_8b

// USART:       Bluetooth Module

#define USART_BT                USART2
#define USART_BT_CLK            RCC_APB1Periph_USART2
#define USART_BT_DMA_RX_CHAN    DMA1_Channel6
#define USART_BT_DMA_TX_CHAN    DMA1_Channel7
#define USART_BT_DR             0x40004404
#define USART_BT_BAUDRATE       230400


// FUNCTION PROTOTYPES  ========================================================
void hardware_config(void);
void DMA_config(void);
void EXTI_config(void);
void GPIO_config(void);
void GPIO_PinConfig(GPIO_TypeDef*,uint16_t,GPIOSpeed_TypeDef,GPIOMode_TypeDef);
void NVIC_config(void);
void RTC_config(void);
void SPI_config(void);
void USART_config(void);
void RCC_config(void);
void TIM_config(void);

#endif //PLAN_CONFIG_H
