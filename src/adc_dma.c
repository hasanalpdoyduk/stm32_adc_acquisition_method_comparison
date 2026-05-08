/**
 * @file    adc_dma.c
 * @brief   ADC1 DMA mode implementation.
 *
 * Configures PA1 as analog input, sets up DMA2 Stream0 Channel0 to transfer
 * the ADC result directly to adc_dma_result in RAM, and signals completion
 * via DMA2_Stream0_IRQHandler.
 */

#include "adc_dma.h"
#include "config.h"

volatile uint16_t adc_dma_result = 0;
volatile uint8_t adc_dma_done = 0;

void adc_dma_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_DMA2EN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    GPIOA->MODER |= GPIO_MODER_MODER1;  // analog mode

    ADC1->CR1 &= ~ADC_CR1_RES;  // 00 = 12-bit resolution
    ADC1->SQR3 |= 1U;  // convert channel 1 first in sequence
    ADC1->SMPR2 &= ~ADC_SMPR2_SMP1;
    ADC1->SMPR2 |= ADC_SMPR2_SMP1_2;  // 84 cycles
    ADC1->CR2 |= ADC_CR2_DMA | ADC_CR2_DDS;  // enable DMA, reissue on each conversion
    ADC1->CR2 |= ADC_CR2_ADON;  // enable ADC

    DMA2_Stream0->CR = (0U << DMA_SxCR_CHSEL_Pos)  // channel 0
                 | DMA_SxCR_PSIZE_0              // peripheral data: 16-bit
                 | DMA_SxCR_MSIZE_0              // memory data: 16-bit
                 | DMA_SxCR_MINC                 // increment memory address after each transfer
                 | DMA_SxCR_TCIE;                // transfer complete interrupt

    DMA2_Stream0->NDTR = 1;                          // transfer 1 data item per conversion
    DMA2_Stream0->PAR  = (uint32_t)&ADC1->DR;        // source: ADC data register
    DMA2_Stream0->M0AR = (uint32_t)&adc_dma_result;  // destination: our variable

    NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void adc_dma_start(void)
{
    adc_dma_done = 0;  // reset completion flag
    DMA2_Stream0->NDTR = 1;
    DMA2_Stream0->CR |= DMA_SxCR_EN;
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

void DMA2_Stream0_IRQHandler(void)
{
    if (DMA2->LISR & DMA_LISR_TCIF0)  // check transfer complete flag for Stream0
    {
        DMA2->LIFCR |= DMA_LIFCR_CTCIF0;  // clear the flag
        adc_dma_done = 1;  // signal that conversion result is ready in adc_dma_result
    }
}
