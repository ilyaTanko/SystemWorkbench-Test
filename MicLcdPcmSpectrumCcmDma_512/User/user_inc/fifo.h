#ifndef FIFO_H
#define FIFO_H

#include "stm32f4xx_conf.h"

#define PCM_QUEUE_LEN	(2 * INTERNAL_BUFF_SIZE)
#define PCM_BUFFER_LEN	N
#define AFC_LEN			(PCM_BUFFER_LEN/2)

extern int16_t g_fifoPCM[PCM_QUEUE_LEN];
extern int g_iPCMBegin;
extern int g_iPCMEnd;

ErrorStatus FIFO_GetNextPCMData(int16_t* pResData);

#endif