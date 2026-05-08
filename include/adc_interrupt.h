/**
 * @file    adc_interrupt.h
 * @brief   ADC1 single-channel acquisition using interrupt.
 *
 * Reads PA1 (ADC1 channel 1) by enabling EOCIE and handling
 * the result in ADC_IRQHandler. CPU is free during conversion.
 */

#ifndef ADC_INTERRUPT_H
#define ADC_INTERRUPT_H

#include <stdint.h>

extern volatile uint16_t adc_interrupt_result;
extern volatile uint8_t adc_interrupt_done;

/** @brief Initializes ADC1 on PA1 for single-conversion interrupt mode. */
void adc_interrupt_init(void);

/** @brief Triggers a single conversion and returns immediately.
 *  Result is written to adc_interrupt_result by ADC_IRQHandler. */
void adc_interrupt_start(void);

#endif /* ADC_INTERRUPT_H */