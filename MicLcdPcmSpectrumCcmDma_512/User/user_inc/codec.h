#ifndef CODEC_H
#define CODEC_H

#include "stm32f4xx_conf.h"
#include "constants.h"

// Codec control interface 
// I2C1_SDA	PB9
// I2C1_SCL	PB6

// Codec audio interface 
// I2S3_MCK	PC7
// I2S3_SCK	PC10
// I2S3_SD	PC12
// I2S3_WS	PA4

// Codec reset pin PD4 

#define CODEC_ADDRESS	0x94   // The 7 bits Codec address (sent through I2C interface)

#define CODEC_I2C_SCL_RCC		RCC_AHB1Periph_GPIOB
#define CODEC_I2C_SCL_PORT		GPIOB
//#define CODEC_I2C_SCL_PINSOURCE	GPIO_PinSource6
#define CODEC_I2C_SCL_PINSOURCE	GPIO_PinSource8
#define CODEC_I2C_SCL_AF		GPIO_AF_I2C1

#define CODEC_I2C_SDA_RCC		RCC_AHB1Periph_GPIOB
#define CODEC_I2C_SDA_PORT		GPIOB
#define CODEC_I2C_SDA_PINSOURCE	GPIO_PinSource9
#define CODEC_I2C_SDA_AF		GPIO_AF_I2C1

#define CODEC_I2S_MCK_RCC		RCC_AHB1Periph_GPIOC
#define CODEC_I2S_MCK_PORT		GPIOC
#define CODEC_I2S_MCK_PIN		GPIO_Pin_7
#define CODEC_I2S_MCK_PINSOURCE	GPIO_PinSource7
#define CODEC_I2S_MCK_AF		GPIO_AF_SPI3

#define CODEC_I2S_SCK_RCC		RCC_AHB1Periph_GPIOC
#define CODEC_I2S_SCK_PORT		GPIOC
#define CODEC_I2S_SCK_PIN		GPIO_Pin_10
#define CODEC_I2S_SCK_PINSOURCE	GPIO_PinSource10
#define CODEC_I2S_SCK_AF		GPIO_AF_SPI3

#define CODEC_I2S_SD_RCC		RCC_AHB1Periph_GPIOC
#define CODEC_I2S_SD_PORT		GPIOC
#define CODEC_I2S_SD_PIN		GPIO_Pin_12
#define CODEC_I2S_SD_PINSOURCE	GPIO_PinSource12
#define CODEC_I2S_SD_AF			GPIO_AF_SPI3

#define CODEC_I2S_WS_RCC		RCC_AHB1Periph_GPIOA
#define CODEC_I2S_WS_PORT		GPIOA
#define CODEC_I2S_WS_PIN		GPIO_Pin_4
#define CODEC_I2S_WS_PINSOURCE	GPIO_PinSource4
#define CODEC_I2S_WS_AF			GPIO_AF_SPI3

#define	CODEC_RESET_RCC			RCC_AHB1Periph_GPIOD
#define CODEC_RESET_PORT		GPIOD
#define CODEC_RESET_PIN			GPIO_Pin_4

#define CODEC_I2S_RCC			RCC_APB1Periph_SPI3
#define CODEC_I2S_PORT			SPI3

#define CODEC_I2C_RCC			RCC_APB1Periph_I2C1
#define CODEC_I2C_PORT			I2C1

#define CODEC_DMA_RCC			RCC_AHB1Periph_DMA1
#define CODEC_DMA_STREAM		DMA1_Stream7
#define CODEC_DMA_FLAG			DMA_FLAG_TCIF7
#define CODEC_DMA_CHANNEL		DMA_Channel_0
#define CODEC_IRQ_CHANNEL		DMA1_Stream7_IRQn

#define TIMEOUT_MAX	 10000 

extern uint16_t AudioBuffer[INTERNAL_BUFF_SIZE];
extern PDMFilter_InitStruct Filter;

uint8_t CODEC_I2C_SingleRandomWrite(I2C_TypeDef* I2Cx, uint8_t Device, uint16_t Addr, uint8_t Data);
uint8_t CODEC_I2C_SingleRandomRead(I2C_TypeDef* I2Cx, uint8_t Device, uint16_t Addr);
uint32_t CODEC_VolumeCtrl(uint8_t Volume);

void CODEC_CtrlLinesConfig();
void CODEC_I2CConfig();
void CODEC_I2SConfig();
void CODEC_Config();
void CODEC_DMAConfig();
void CODEC_DMA_NVIC_Config();

void CODEC_Init();

#endif