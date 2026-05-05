/**
 * @file    config.h
 * @brief   Project-wide configuration parameters.
 *
 * Central place for tunable constants: buffer sizes and filter
 * parameters. Board-specific definitions (pins, clocks) live in
 * board.h, which is included here so the rest of the project only
 * needs to include config.h.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "board.h"

// Number of ADC samples held in the DMA circular buffer
#define DMA_BUFFER_SIZE     256

// Moving Average window size. Larger = smoother but more lag
#define MA_WINDOW_SIZE      16

/*
 * IIR low-pass alpha expressed as a fraction (NUM / DEN).
 * alpha = 204 / 256 ≈ 0.8
 * output = alpha * prev_output + (1 - alpha) * new_sample
 * Integer arithmetic is used to avoid floating-point overhead.
 */
#define IIR_ALPHA_NUM       204
#define IIR_ALPHA_DEN       256

// Number of FIR filter taps (coefficients)
#define FIR_TAP_COUNT       16

#endif /* CONFIG_H */
