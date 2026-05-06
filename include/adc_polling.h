/**
 * @file    adc_polling.h
 * @brief   ADC1 single-channel acquisition using CPU polling.
 *
 * Reads PA1 (ADC1 channel 1) by busy-waiting on the EOC flag.
 * CPU is fully occupied during the conversion window.
 */

#ifndef ADC_POLLING_H
#define ADC_POLLING_H

#include <stdint.h>

/** @brief Initializes ADC1 on PA1 for single-conversion polling mode. */
void adc_polling_init(void);

/** @brief Triggers a conversion and blocks until result is ready.
 *  @return 12-bit ADC result (0–4095). */
uint16_t adc_polling_read(void);

#endif /* ADC_POLLING_H */
