/**
 * @file    adc_dma.h
 * @brief   ADC1 single-channel acquisition using DMA.
 *
 * Reads PA1 (ADC1 channel 1) by configuring DMA2 Stream0 to transfer
 * the result directly to RAM. CPU is not involved in the data transfer.
 */

#ifndef ADC_DMA_H
#define ADC_DMA_H

#include <stdint.h>

extern volatile uint16_t adc_dma_result;
extern volatile uint8_t adc_dma_done;

/** @brief Initializes ADC1 on PA1 and configures DMA2 Stream0 Ch0 for single-conversion mode. */
void adc_dma_init(void);

/** @brief Triggers a single conversion and returns immediately.
 *  Result is written to adc_dma_result by DMA2_Stream0_IRQHandler. */
void adc_dma_start(void);

#endif /* ADC_DMA_H */
