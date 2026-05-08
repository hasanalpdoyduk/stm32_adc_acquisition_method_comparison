/**
 * @file    filters.h
 * @brief   Digital filter implementations for ADC signal conditioning.
 */

#ifndef FILTERS_H
#define FILTERS_H

#include <stdint.h>


/* Moving Average Filter *******************************************/

typedef struct
{
    uint16_t buf[8];
    uint8_t  index;
} MovingAvgFilter;

/** @brief Clears the moving average buffer and resets the index. */
void moving_avg_init(MovingAvgFilter *f);

/** @brief Adds a new sample and returns the average of the last 8 values.
 *  @param f      Pointer to the filter state.
 *  @param sample New ADC sample to add.
 *  @return       Averaged 12-bit result. */
uint16_t moving_avg_update(MovingAvgFilter *f, uint16_t sample);


/* IIR Filter *****************************************************/

/** @brief Applies a 1st-order IIR low-pass filter (alpha = 1/4).
 *  @param sample New ADC sample.
 *  @param prev   Previous filter output.
 *  @return       Filtered result. */
uint16_t iir_update(uint16_t sample, uint16_t prev);


/* FIR Filter ****************************************************/

typedef struct
{
    uint16_t buf[8];
    uint8_t  index;
} FirFilter;

/** @brief Clears the FIR buffer and resets the index. */
void fir_init(FirFilter *f);

/** @brief Adds a new sample and returns the Hamming-windowed FIR output.
 *  @param f      Pointer to the filter state.
 *  @param sample New ADC sample to add.
 *  @return       Filtered result. */
uint16_t fir_update(FirFilter *f, uint16_t sample);

#endif /* FILTERS_H */
