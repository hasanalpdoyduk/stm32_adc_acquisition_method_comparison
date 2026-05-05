/**
 * @file    profiler.h
 * @brief   CPU cycle counter based on the ARM Cortex-M4 DWT unit.
 *
 * Wraps the DWT->CYCCNT hardware register to measure elapsed clock
 * cycles between profiler_start() and profiler_stop(). Used to
 * compare the CPU overhead of polling, interrupt, and DMA ADC methods.
 */

#ifndef PROFILER_H
#define PROFILER_H

#include <stdint.h>

/** @brief Enables the DWT cycle counter. Must be called once at startup. */
void profiler_init(void);

/** @brief Records the current cycle count as the start timestamp. */
void profiler_start(void);

/**
 * @brief  Returns elapsed cycles since profiler_start().
 * @return Number of CPU clock cycles elapsed.
 */
uint32_t profiler_stop(void);

#endif /* PROFILER_H */
