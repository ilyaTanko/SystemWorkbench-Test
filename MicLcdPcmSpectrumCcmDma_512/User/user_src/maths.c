#include "../user_inc/maths.h"

// Целочисленное быстрое преобразование Фурье
// Алгоритм Кули-Тьюки
// с прореживанием по частоте
void FFTif ( int16_t f[N], int16_t C[2][N/2] ) 
{
	// 1.209 ms COS+SIN (OPTIM=-O0) 
	// 315.928 mcs COS+SIN (OPTIM=-O1) 
	GPIO_SetBits(GPIOC, GPIO_Pin_15);
	
	int32complex_t cplx[N];
	int32complex_t * pcplx = cplx;
	int16_t* pf = f;
	uint16_t k = N;

	while (k--) 
    {
		(*pcplx).r = *pf++;
		(*pcplx).i = 0;
		pcplx++;
	}

	// Непосредственное БПФ
	uint16_t N_2 = N >> 1;
	uint16_t step = 1, n, q;

	int32complex_t *pCplx_0, *pCplx_1;
	int32_t *prealW, *pimageW;

	k = POW_N;
	while (--k) 
    {
		pCplx_0 = cplx, pCplx_1 = cplx + N_2;
		q = step;
		while (q--) 
        {
			prealW = realW, pimageW = imageW;
			n = N_2;
			while (n--) 
            {
				// Реализация диаграммы "бабочка"
				// для алгоритма с прореживанием по частоте
				int32complex_t W = {*prealW, *pimageW};
				FFTi2f (*pCplx_0, *pCplx_1, W);
				prealW += step, pimageW += step,
				pCplx_0++, pCplx_1++;
			}
			pCplx_0 += N_2, pCplx_1 += N_2;
		}
		step <<= 1;
		N_2 >>= 1;
	}

	// Последний итерационный цикл двухточечных БПФ
	//  (без домножения на поворачивающие множители)
	pCplx_0 = cplx, pCplx_1 = cplx + 1;
	q = step;
	while (q--) 
    {
		int32complex_t c = *pCplx_0;
		PLUS ( *pCplx_0, *pCplx_1 );
		MINUS ( c, *pCplx_1 );
		pCplx_0 += 2, pCplx_1 += 2;
	}
	
	// Учёт двоично-инверсной перестановки индексов
	uint16_t i = 0;
	k = N/2;
	while (k--) 
    {
		SWAPi(cplx,i);
		i++;
	}

	// Деление элементов на N
	//  и окончательная запись результата
	//  в массив C[2][N/2]
	int16_t* pC = C[0];	/* Cos */
	int16_t* pS = C[1];	/* Sin */
	pcplx = cplx + 1;
	k = N/2;
	while (k--) 
    {
		int32complex_t cpx = *pcplx++;
		*pC++ = cpx.r >> POW_N;
		*pS++ = cpx.i >> POW_N;
	}

	GPIO_ResetBits ( GPIOC, GPIO_Pin_15 );
}

// Извлечение квадратного корня методом Ньютона
uint16_t squareRoot(uint32_t x) 
{
	uint32_t y = x, z = x;
	uint8_t bits = 0;

	// Поиск первого приближения
	// путём подсчёта длины битов в исходном числе
	// и последующего его сдвига "пополам"
	while (y >>= 1) 
		bits++;
	y = x >> (bits >> 1);

	// Непосредственный итерационный цикл алгоритма Ньютона
	while (ABS(y-z) > 1) 
    {
		z = y;
		y = (z + x/z) >> 1;
	}

	return y;
}
