#ifndef MEMS_H
#define MEMS_H

#include "stm32f4xx_conf.h"

// MEMS microphone interface
// I2S2_PDM_OUT	 PC3
// I2S3_CLK_IN	 PB10

#define MEMS_MIC_CLKIN_RCC		    RCC_AHB1Periph_GPIOB
#define MEMS_MIC_CLKIN_PORT		    GPIOB
#define MEMS_MIC_CLKIN_PIN		    GPIO_Pin_10
#define MEMS_MIC_CLKIN_PINSOURCE	GPIO_PinSource10
#define MEMS_MIC_CLKIN_AF		    GPIO_AF_SPI2

#define MEMS_MIC_PDMOUT_RCC		    RCC_AHB1Periph_GPIOC
#define MEMS_MIC_PDMOUT_PORT	    GPIOC
#define MEMS_MIC_PDMOUT_PIN		    GPIO_Pin_3
#define MEMS_MIC_PDMOUT_PINSOURCE	GPIO_PinSource3
#define MEMS_MIC_PDMOUT_AF		    GPIO_AF_SPI2

#define MEMS_MIC_I2S_RCC		    RCC_APB1Periph_SPI2
#define MEMS_MIC_I2S_PORT		    SPI2

#define MEMS_MIC_DMA_RCC		    RCC_AHB1Periph_DMA1
#define MEMS_MIC_DMA_STREAM		    DMA1_Stream3
#define MEMS_MIC_DMA_FLAG_HT	    DMA_FLAG_HTIF3
#define MEMS_MIC_DMA_FLAG_TC	    DMA_FLAG_TCIF3
#define MEMS_MIC_DMA_CHANNEL	    DMA_Channel_0
#define MEMS_MIC_IRQ_CHANNEL	    DMA1_Stream3_IRQn

extern PDMFilter_InitStruct Filter;
extern uint16_t InternalBuffer[2 * INTERNAL_BUFF_SIZE]; // Temporary data sample

void MEMS_MicCtrlLinesConfig();
void MEMS_MicI2SConfig();
void MEMS_MicDMAConfig();
void MEMS_MicDMA_NVIC_Config();
void MEMS_MicInit();

void MEMS_MicInit();

#endif