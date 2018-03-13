#include "../user_inc/lcd.h"

void FRAME_DmaToLcd() 
{
	g_FrameTotalBytesCopied = 2;

	LCD_WriteReg(SSD2119_X_RAM_ADDR_REG, 0x00);
	LCD_WriteReg(SSD2119_Y_RAM_ADDR_REG, 0x00);
	LCD_WriteReg(SSD2119_RAM_DATA_REG, lcdFrame_1[0]);

	DMA_Cmd(DMA2_Stream3, DISABLE);
	DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3);
	DMA_SetCurrDataCounter(DMA2_Stream3, 0);
	DMA_Cmd(DMA2_Stream3, ENABLE);
}

void MIC_FRAME_RestoreAreas(uint16_t pcmGraphRectCoors[4], uint16_t afcGraphRectCoors[4], uint16_t* pBaseImage) 
{
	uint32_t framePixelIndex;
	uint16_t* graphRectCoors[] = { pcmGraphRectCoors, afcGraphRectCoors };
	uint16_t i, j, k = 2;

	while (k--) 
    {
		uint32_t x = graphRectCoors[k][0],
				 y = MAX((int16_t)graphRectCoors[k][1]-80, 0);
		
        j = graphRectCoors[k][3] + 130;
		
        do {
			framePixelIndex = x + y++ * LCD_PIXEL_WIDTH;
			i = graphRectCoors[k][2] - 1;
			do {
				if (framePixelIndex <= CCMDATA_SIZE / 2)
					lcdFrame_1[framePixelIndex] = pBaseImage[framePixelIndex];
				else if (framePixelIndex < FRAME_SIZE)
					lcdFrame_2[framePixelIndex - CCMDATA_SIZE / 2 - 1] = pBaseImage[framePixelIndex];
				framePixelIndex = (framePixelIndex + 1) % FRAME_SIZE;
			} while (--i);
		} while (--j);
	}
}
        
void FRAME_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction) 
{
	uint16_t textColor, backColor;
	uint32_t framePixelIndex;
	uint32_t i = 0;

	LCD_GetColors(&textColor, &backColor);
	framePixelIndex = Xpos + Ypos * LCD_PIXEL_WIDTH;

	if (Direction == LCD_DIR_HORIZONTAL) 
    {
		for (i = 0; i <= Length; i++) 
        {
			if (framePixelIndex <= CCMDATA_SIZE / 2) 
				lcdFrame_1[framePixelIndex] = textColor;
			else 
				lcdFrame_2[framePixelIndex - CCMDATA_SIZE / 2 - 1] = textColor;
			framePixelIndex = (framePixelIndex + 1) % FRAME_SIZE;
		}
	} else 
    {
		for (i = 0; i <= Length; i++) 
        {
			if (framePixelIndex <= CCMDATA_SIZE / 2)
				lcdFrame_1[framePixelIndex] = textColor;
			else
				lcdFrame_2[framePixelIndex - CCMDATA_SIZE / 2 - 1] = textColor;
			framePixelIndex = (framePixelIndex + LCD_PIXEL_WIDTH) % FRAME_SIZE;
		}
	}
}

void MIC_FRAME_DrawGraph(int16_t* dataArray, uint16_t dataArraySize, uint16_t graphRectCoors[4]) 
{
	uint16_t ki = dataArraySize / GRAPH_WIDTH;
	
    for (uint16_t i = 0; i < GRAPH_WIDTH-1; i++) 
    {
		uint16_t i_0 = ki * i, i_1 = ki*(i+1);
		uint16_t frame_y_1 = graphRectCoors[1] + graphRectCoors[3]/2 - (dataArray[i_0] >> DATA_SHIFT);
		uint16_t frame_y_2 = graphRectCoors[1] + graphRectCoors[3]/2 - (dataArray[i_1] >> DATA_SHIFT);
		frame_y_1 = (frame_y_1 < 0)? 0 : frame_y_1;
		frame_y_2 = (frame_y_2 < 0)? 0 : frame_y_2;
		frame_y_1 = (frame_y_1 >= LCD_PIXEL_HEIGHT)? (LCD_PIXEL_HEIGHT - 1) : frame_y_1;
		frame_y_2 = (frame_y_2 >= LCD_PIXEL_HEIGHT)? (LCD_PIXEL_HEIGHT - 1) : frame_y_2;
		uint16_t len = ABS(dataArray[i_1]-dataArray[i_0]);
		FRAME_DrawLine ( graphRectCoors[0]+i, MIN(frame_y_1,frame_y_2), len >> DATA_SHIFT, LCD_DIR_VERTICAL);
	}
}
        
void MIC_FRAME_DrawGraphBar(uint16_t* dataArray, uint16_t dataArraySize, uint16_t graphRectCoors[4])
{
	uint16_t ki = dataArraySize / GRAPH_WIDTH;
	uint16_t frame_y_0 = graphRectCoors[1] + graphRectCoors[3];
	
    for (uint16_t i = 0; i < GRAPH_WIDTH; i++) 
    {
		uint16_t frame_y_1 = frame_y_0 - (dataArray[ki*i] >> AFC_SHIFT);
		frame_y_1 = (frame_y_1 < 0)? 0 : frame_y_1;
		frame_y_1 = (frame_y_1 >= LCD_PIXEL_HEIGHT)? (LCD_PIXEL_HEIGHT - 1) : frame_y_1;
		uint16_t len = ABS(frame_y_1 - frame_y_0);
		FRAME_DrawLine ( graphRectCoors[0]+i, MIN(frame_y_0,frame_y_1), len, LCD_DIR_VERTICAL );
	}
}
        
void FRAME_RestoreArea(uint16_t graphRectCoors[4], uint16_t* pBaseImage) 
{
	uint32_t framePixelIndex;
	uint32_t x = graphRectCoors[0], y = graphRectCoors[1];
	uint16_t i, j = graphRectCoors[3] + 1;

	do {
		framePixelIndex = x + y++ * LCD_PIXEL_WIDTH;
		i = graphRectCoors[2];
		do {
			if (framePixelIndex <= CCMDATA_SIZE / 2)
				lcdFrame_1[framePixelIndex] = pBaseImage[framePixelIndex];
			else
				lcdFrame_2[framePixelIndex - CCMDATA_SIZE / 2 - 1] = pBaseImage[framePixelIndex];
			framePixelIndex = (framePixelIndex + 1) % FRAME_SIZE;
		} while (--i);
	} while (--j);
}

// Configure DMA2 to transfer Data from DmaCaptureFrame[] to the LCD
void DMA_LCD_Config() 
{	
	RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_DMA2, ENABLE );

    // DMA2 Stream3 Configuration
    DMA_InitTypeDef  DMA_InitStructure;
	DMA_DeInit (DMA2_Stream3);
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)DmaFrame;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)LCD_BASE_Data;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;
	DMA_InitStructure.DMA_BufferSize = DMA_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream3, &DMA_InitStructure);

	// Enable the DMA2_Stream3 Interrupt
    NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_GROUP);
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
	DMA_ITConfig(DMA2_Stream3, DMA_IT_TC, ENABLE);
}
