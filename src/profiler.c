/**
 * @file    profiler.c
 * @brief   DWT cycle counter implementation.
 *
 * Directly accesses CoreDebug and DWT registers to enable and
 * read the hardware cycle counter without any HAL dependency.
 */

#include "profiler.h"
#include "config.h"

static uint32_t _start;

void profiler_init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // required before DWT can be used
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // start counting cycles
}

void profiler_start(void)
{
    _start = DWT->CYCCNT;
}

uint32_t profiler_stop(void)
{
    return DWT->CYCCNT - _start;
}
