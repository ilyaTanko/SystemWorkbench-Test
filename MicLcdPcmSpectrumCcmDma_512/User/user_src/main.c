#include "stdint.h"
#include "../user_inc/stm32f4xx_conf.h"

#include <string.h>

#include "../user_inc/maths.h"
#include "../user_inc/codec.h"
#include "../user_inc/mems.h"
#include "../user_inc/lcd.h"
#include "../user_inc/fifo.h"
#include "../user_inc/constants.h"
#include "../user_inc/startup.h"

#include "../../discovery/discovery_inc/pdm_filter.h"

uint16_t AudioBuffer[INTERNAL_BUFF_SIZE];
uint16_t InternalBuffer[2 * INTERNAL_BUFF_SIZE]; // Temporary data sample
PDMFilter_InitStruct Filter;

uint16_t RecBuf[PCM_OUT_SIZE];

extern const unsigned char gImage_BaseImage[];
unsigned short BaseImage_coor[] = {0, 0, LCD_PIXEL_WIDTH, LCD_PIXEL_HEIGHT};

uint16_t* lcdFrame_1 = (uint16_t*)CCMDATARAM_BASE;	 // 65534 bytes ~= 64 KB
uint16_t lcdFrame_2[FRAME_SIZE - CCMDATA_SIZE / 2]; // 88066 bytes

uint16_t pcmGraphCoor[] = {42, 77, GRAPH_WIDTH, 64};
uint16_t afcGraphCoor[] = {42, 166, GRAPH_WIDTH, 32};

uint16_t DmaFrame[DMA_BUFFER_SIZE];
uint32_t g_FrameTotalBytesCopied = 0;

int16_t	g_fifoPCM[PCM_QUEUE_LEN];
int	g_iPCMBegin = 0;
int	g_iPCMEnd = 0;

int main() 
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE); // Enable CRC module
    
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	// SysTick end of count event each 10ms
    RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

	BaseImage_coor[2] = *(uint16_t*)(gImage_BaseImage + 2);
	BaseImage_coor[3] = *(uint16_t*)(gImage_BaseImage + 4);
 
	STM32f4_Discovery_LCD_Init(); // Initialize the LCD
	LCD_SetColors(LCD_COLOR_BLUE, LCD_COLOR_BLACK);

	for (uint32_t framePixelIndex = 0; framePixelIndex < FRAME_SIZE; framePixelIndex++) 
    {
		if (framePixelIndex <= CCMDATA_SIZE / 2)
			lcdFrame_1[framePixelIndex] = *((uint16_t *)(gImage_BaseImage + 8) + framePixelIndex);
		else
			lcdFrame_2[framePixelIndex - CCMDATA_SIZE / 2 - 1] = *((uint16_t*)(gImage_BaseImage + 8) + framePixelIndex);
	}

	DMA_LCD_Config();

	// Filter LP & HP Init
	Filter.LP_HZ = 0;
	Filter.HP_HZ = 0;

	Filter.Fs = I2S_AudioFreq_16k;
	Filter.Out_MicChannels = 1;
	Filter.In_MicChannels = 1;

	PDM_Filter_Init((PDMFilter_InitStruct *)&Filter);
	MEMS_MicInit();
	CODEC_Init();

	ResetTickCount();
	uint32_t lcdTick = GetTickCount();
    
    static int16_t micPCM[PCM_BUFFER_LEN + 1];
    static int iMicPCMBufferLen = 0;
	static uint16_t	micAFC[PCM_BUFFER_LEN/2];
    
    int16_t C[2][N/2];

	while (1) 
    {      
		// Считывание PCM-данных от микрофона
		// из циклических буферов FIFO
		//  в линейныё буфер micPCM[]
        int16_t	curPCMData;
		while (FIFO_GetNextPCMData(&curPCMData) == SUCCESS) 
        {
			micPCM[iMicPCMBufferLen++] = curPCMData;
			if (iMicPCMBufferLen > PCM_BUFFER_LEN) 
            {
				memmove(micPCM, micPCM + 1, PCM_BUFFER_LEN * sizeof(micPCM[0]));
				iMicPCMBufferLen--;
			}
		}

		if ((uint32_t)GetTickCount() - lcdTick > 50) 
        {
			lcdTick = GetTickCount();
			
			// Вычисление БПФ и обновление сигналов на дисплее каждые 50 мс
			 
			// Целочисленное быстрое преобразование Фурье
			// с прореживанием по частоте
			// 4.5 мс уровень оптимизации O0 (выборка 512 элементов)
			// 730 мкс уровень оптимизации O1 (выборка 512 элементов)
			FFTif (micPCM, C);
            
			for (int16_t n = 0; n < (N/2 - 1); n++)
				micAFC[n] = squareRoot(C[0][n] * C[0][n] + C[1][n] * C[1][n]);

			MIC_FRAME_RestoreAreas(pcmGraphCoor, afcGraphCoor, (uint16_t *)(gImage_BaseImage + 8) );
			MIC_FRAME_DrawGraph(micPCM, PCM_BUFFER_LEN, pcmGraphCoor);
			MIC_FRAME_DrawGraphBar(micAFC, AFC_LEN, afcGraphCoor);
			FRAME_DmaToLcd();
		}
	}

	return 0;
}


// Handle AUDIO_REC_SPI global interrupt request.
void MEMS_MIC_DMA_STREAM_IRQHandler() 
{
	uint16_t* pInternalBuffer_s = InternalBuffer; // Начальный адрес фильтруемой части массива
	uint16_t* pInternalBuffer;

	if (DMA_GetFlagStatus(MEMS_MIC_DMA_STREAM, MEMS_MIC_DMA_FLAG_TC) != RESET)
		pInternalBuffer_s += INTERNAL_BUFF_SIZE;

	DMA_ClearFlag(MEMS_MIC_DMA_STREAM, MEMS_MIC_DMA_FLAG_HT | MEMS_MIC_DMA_FLAG_TC);

	pInternalBuffer = pInternalBuffer_s;

	uint8_t i = INTERNAL_BUFF_SIZE;
	do {
		uint16_t app = *pInternalBuffer;
		*pInternalBuffer++ = HTONS ( app );
	} while (--i);

	// 22.6 mcs (44100 Hz)
	PDM_Filter_64_LSB((uint8_t*)pInternalBuffer_s, RecBuf, 10,	&Filter );
}

// Handles main Media layer interrupt.
// Return 0 if correct communication, else wrong communication
void CODEC_DMA_STREAM_IRQHandler() 
{
	// Transfer complete interrupt
	if (DMA_GetFlagStatus(CODEC_DMA_STREAM, CODEC_DMA_FLAG) != RESET) 
    {
		DMA_Cmd(CODEC_DMA_STREAM, DISABLE); // Disable the I2S DMA Stream
		DMA_ClearFlag(CODEC_DMA_STREAM, CODEC_DMA_FLAG); // Clear the Interrupt flag
        
		// Manage the remaining file size and new address offset: 
        // This function should be coded by user 
        // (its prototype is already declared in stm32f4_discovery_audio_codec.h)
		for (uint8_t i = 0; i < (INTERNAL_BUFF_SIZE / 2); i++) 
        {
			uint16_t audioData = RecBuf[i/2];
			AudioBuffer[i] = audioData;

			if (i & 1) 
            {
				g_fifoPCM[g_iPCMEnd++] = (int16_t)audioData;
				g_iPCMEnd %= PCM_QUEUE_LEN;
			}
		}
        
		DMA_Cmd(CODEC_DMA_STREAM, ENABLE); // Enable the I2S DMA Stream
 	}
}

void DMA2_Stream3_IRQHandler() 
{
	int32_t sizeToCopy;
	DMA_ClearITPendingBit(DMA2_Stream3, DMA_FLAG_TCIF3);

	if (g_FrameTotalBytesCopied < (sizeof(DmaFrame[0]) * FRAME_SIZE)) 
    {
		if (g_FrameTotalBytesCopied < CCMDATA_SIZE) 
        {
			sizeToCopy = MIN (DMA_BUFFER_SIZE, (CCMDATA_SIZE - g_FrameTotalBytesCopied) / sizeof(DmaFrame[0]));
			memmove(DmaFrame, lcdFrame_1 + 1 + g_FrameTotalBytesCopied / sizeof(lcdFrame_1[0]), sizeToCopy * sizeof(DmaFrame[0]));
		} else 
        {
			sizeToCopy = MIN(DMA_BUFFER_SIZE, FRAME_SIZE - g_FrameTotalBytesCopied / sizeof(DmaFrame[0]));
			memmove(DmaFrame, lcdFrame_2 + (g_FrameTotalBytesCopied - CCMDATA_SIZE) / sizeof(lcdFrame_2[0]), sizeToCopy * sizeof(DmaFrame[0]) );
		}
		g_FrameTotalBytesCopied += sizeToCopy * sizeof(DmaFrame[0]);

		DMA_Cmd(DMA2_Stream3, DISABLE);
		DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3);
		DMA_SetCurrDataCounter(DMA2_Stream3, sizeToCopy);
		DMA_Cmd(DMA2_Stream3, ENABLE);
	}
}

