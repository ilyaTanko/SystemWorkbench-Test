#include "../user_inc/fifo.h"

ErrorStatus FIFO_GetNextPCMData(int16_t* pResData) 
{
	if (g_iPCMBegin != g_iPCMEnd) 
    {
		*pResData = g_fifoPCM[g_iPCMBegin++];
		g_iPCMBegin %= PCM_QUEUE_LEN;
		return SUCCESS;
	}   
	return ERROR;
}
