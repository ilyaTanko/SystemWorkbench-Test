#include "../user_inc/mems.h"

void MEMS_MicCtrlLinesConfig() 
{
	RCC_AHB1PeriphClockCmd(MEMS_MIC_CLKIN_RCC | MEMS_MIC_PDMOUT_RCC, ENABLE); // PC3, PB10 clock enable

	// I2S3_CLK_IN (PB10) for MEMS microphone
    GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = MEMS_MIC_CLKIN_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(MEMS_MIC_CLKIN_PORT, &GPIO_InitStruct);
	GPIO_PinAFConfig(MEMS_MIC_CLKIN_PORT, MEMS_MIC_CLKIN_PINSOURCE, MEMS_MIC_CLKIN_AF);

	// I2S2_PDM_OUT (PC3) for MEMS microphone
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = MEMS_MIC_PDMOUT_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(MEMS_MIC_PDMOUT_PORT, &GPIO_InitStruct);
	GPIO_PinAFConfig (MEMS_MIC_PDMOUT_PORT, MEMS_MIC_PDMOUT_PINSOURCE, MEMS_MIC_PDMOUT_AF );
}

// I2S2 (SPI2) initialization for MEMS microphone
void MEMS_MicI2SConfig() 
{
	RCC_APB1PeriphClockCmd(MEMS_MIC_I2S_RCC, ENABLE);

    I2S_InitTypeDef	I2S_InitStruct;
	I2S_InitStruct.I2S_AudioFreq = Filter.Fs;
	I2S_InitStruct.I2S_CPOL = I2S_CPOL_Low;
	I2S_InitStruct.I2S_DataFormat = I2S_DataFormat_16b;
	I2S_InitStruct.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
	I2S_InitStruct.I2S_Mode = I2S_Mode_MasterRx;
	I2S_InitStruct.I2S_Standard = I2S_Standard_LSB;
	I2S_Init(MEMS_MIC_I2S_PORT, &I2S_InitStruct);
	I2S_Cmd(MEMS_MIC_I2S_PORT, ENABLE);
}

void MEMS_MicDMAConfig() 
{
	RCC_AHB1PeriphClockCmd(MEMS_MIC_DMA_RCC, ENABLE); // Enable DMA1 clock

    
	// DMA1 Stream3 Configuration
    static DMA_InitTypeDef	DMA_InitStructureMic;
	DMA_DeInit(MEMS_MIC_DMA_STREAM);
	DMA_InitStructureMic.DMA_Channel = MEMS_MIC_DMA_CHANNEL;
	DMA_InitStructureMic.DMA_PeripheralBaseAddr = (uint32_t)&MEMS_MIC_I2S_PORT->DR;
	DMA_InitStructureMic.DMA_Memory0BaseAddr = (uint32_t) InternalBuffer;
	DMA_InitStructureMic.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructureMic.DMA_BufferSize = 2 * INTERNAL_BUFF_SIZE;
	DMA_InitStructureMic.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructureMic.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructureMic.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructureMic.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructureMic.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructureMic.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(MEMS_MIC_DMA_STREAM, &DMA_InitStructureMic);
	DMA_Cmd(MEMS_MIC_DMA_STREAM, ENABLE);
	SPI_I2S_DMACmd(MEMS_MIC_I2S_PORT, SPI_I2S_DMAReq_Rx, ENABLE);
}

// Configure the SPI interrupt priority
void MEMS_MicDMA_NVIC_Config() 
{
    DMA_ITConfig ( MEMS_MIC_DMA_STREAM, DMA_IT_HT | DMA_IT_TC, ENABLE );
    
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = MEMS_MIC_IRQ_CHANNEL;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void MEMS_MicInit() 
{
	MEMS_MicCtrlLinesConfig();
	MEMS_MicI2SConfig();
	MEMS_MicDMAConfig();
	MEMS_MicDMA_NVIC_Config();
}
