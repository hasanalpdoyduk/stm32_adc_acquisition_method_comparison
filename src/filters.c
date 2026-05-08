/**
 * @file    filters.c
 * @brief   Digital filter implementations for ADC signal conditioning.
 */

#include "filters.h"


/* Moving Average Filter *******************************************/

void moving_avg_init(MovingAvgFilter *f)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
        f->buf[i] = 0;
    f->index = 0;
}

uint16_t moving_avg_update(MovingAvgFilter *f, uint16_t sample)
{
    f->buf[f->index] = sample;
    f->index = (f->index + 1) % 8;

    uint32_t sum = 0;
    uint8_t i;
    for (i = 0; i < 8; i++)
        sum += f->buf[i];
    return (uint16_t)(sum / 8);
}


/* IIR Filter *****************************************************/

uint16_t iir_update(uint16_t sample, uint16_t prev)
{
    return (uint16_t)((sample + 3U * (uint32_t)prev) >> 2);
}


/* FIR Filter ****************************************************/

static const uint16_t fir_coeffs[8] = {4, 14, 39, 72, 72, 39, 14, 4};

void fir_init(FirFilter *f)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
        f->buf[i] = 0;
    f->index = 0;
}

uint16_t fir_update(FirFilter *f, uint16_t sample)
{
    f->buf[f->index] = sample;
    f->index = (f->index + 1) % 8;

    uint32_t sum = 0;
    uint8_t i;
    for (i = 0; i < 8; i++)
        sum += (uint32_t)f->buf[i] * fir_coeffs[i];
    return (uint16_t)(sum / 258);
}
