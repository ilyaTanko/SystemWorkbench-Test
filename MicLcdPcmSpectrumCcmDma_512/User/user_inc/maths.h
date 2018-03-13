#ifndef MATHS_H
#define MATHS_H

#include "stm32f4xx_conf.h"

#define ABS(x)			((x) < 0) ? -(x) : x
#define MIN(x,y)		((x) < (y)) ? (x) : (y)
#define MAX(x,y)		((x) > (y)) ? (x) : (y)
#define THRESHOLD(x)	(ABS((x)) > 10) ? (x) : 0

#define PLUS(a,b) \
{ \
	(a).r += (b).r; \
	(a).i += (b).i; \
}

#define MINUS(a,b) \
{ \
	(b).r = (a).r - (b).r; \
	(b).i = (a).i - (b).i; \
}

#define MULi(a,b)	\
{ \
	int32complex_t c = a; \
	(a).r = c.r * (b).r - c.i * (b).i; \
	(a).i = c.r * (b).i + c.i * (b).r; \
}

// Умножение комплексного числа
// на весовой коэффициент, меньший единицы,
// на базе операций арифметического умножения
// и сдвига на POW разрядов вправо */
#define MULSHIFT(a,w) \
{ \
		int32complex_t c = a; \
		(a).r = (c.r * w.r - c.i * w.i) >> POW; \
		(a).i = (c.r * w.i + c.i * w.r) >> POW; \
}

#define FFTi2f(a,b,w) \
{ \
		int32complex_t c = a; \
		PLUS(a,b);		/* a = a + b */ \
		MINUS(c,b);		/* b = a - b */ \
		MULSHIFT(b,w);	/* b = b * w */ \
}

#define SWAPi(f,i)	\
{ \
	uint8_t _i = i, cntr = POW_N; \
	uint16_t j = 0; \
	while (cntr--) \
    { \
		j = (j << 1) | (_i & 1); \
		_i >>= 1; \
	} \
	if (j > i) \
    { \
		int32complex_t cpx = f[i]; \
		f[i] = f[j]; \
		f[j] = cpx; \
	} \
}

extern int32_t realW[];
extern int32_t imageW[];

void FFTif(int16_t f[N], int16_t C[2][N/2]);
uint16_t squareRoot(uint32_t x);

#endif 
