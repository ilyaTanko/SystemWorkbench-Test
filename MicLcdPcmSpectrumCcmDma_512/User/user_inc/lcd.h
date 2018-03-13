#ifndef LCD_H
#define LCD_H

#include "stm32f4xx_conf.h"
#include "constants.h"
#include "maths.h"

#define DATA_SHIFT		5 // 10
#define AFC_SHIFT		1 // 9

#define GRAPH_WIDTH		256

extern const unsigned char gImage_BaseImage[];
extern unsigned short BaseImage_coor[];

extern uint16_t pcmGraphCoor[];
extern uint16_t afcGraphCoor[];

extern uint32_t g_FrameTotalBytesCopied;

extern uint16_t* lcdFrame_1;
extern uint16_t lcdFrame_2[];

extern uint16_t DmaFrame[];

void FRAME_DmaToLcd();
void FRAME_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction);
void FRAME_RestoreArea(uint16_t graphRectCoors[4], uint16_t* pBaseImage);
void MIC_FRAME_RestoreAreas(uint16_t pcmGraphRectCoors[4], uint16_t afcGraphRectCoors[4], uint16_t* pBaseImage);
void MIC_FRAME_DrawGraph(int16_t* dataArray, uint16_t dataArraySize, uint16_t graphRectCoors[4]);
void MIC_FRAME_DrawGraphBar(uint16_t* dataArray, uint16_t dataArraySize, uint16_t graphRectCoors[4]);

void DMA_LCD_Config();

#endif
