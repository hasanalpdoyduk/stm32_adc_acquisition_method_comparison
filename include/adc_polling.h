/**
 * @file    adc_polling.h
 * @brief   ADC1 single-channel acquisition using CPU polling.
 *
 * Reads PA1 (ADC1 channel 1) by busy-checking the EOC flag in the
 * caller's loop. CPU is occupied polling the flag during conversion.
 */

#ifndef ADC_POLLING_H
#define ADC_POLLING_H

#include <stdint.h>

extern volatile uint16_t adc_polling_result;
extern volatile uint8_t  adc_polling_done;

/** @brief Initializes ADC1 on PA1 for single-conversion polling mode. */
void adc_polling_init(void);

/** @brief Triggers a single conversion and returns immediately. */
void adc_polling_start(void);

/** @brief Checks EOC flag; writes result and sets adc_polling_done when ready.
 *  Call repeatedly in a loop until adc_polling_done is set. */
void adc_polling_poll(void);

#endif /* ADC_POLLING_H */
