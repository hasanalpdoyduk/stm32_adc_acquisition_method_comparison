/**
 * @file    adc_interrupt.c
 * @brief   ADC1 interrupt mode implementation.
 *
 * Configures PA1 as analog input, enables EOCIE so the ADC fires
 * ADC_IRQHandler on conversion complete, and enables the IRQ in NVIC.
 */

#include "adc_interrupt.h"
#include "config.h"

volatile uint16_t adc_interrupt_result = 0;
volatile uint8_t adc_interrupt_done = 0;

void adc_interrupt_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    GPIOA->MODER |= GPIO_MODER_MODER1;  // analog mode
    
    ADC1->CR1 &= ~ADC_CR1_RES;  // 00 = 12-bit resolution
    ADC1->SQR3 |= 1U;  // convert channel 1 first in sequence
    ADC1->SMPR2 &= ~ADC_SMPR2_SMP1;
    ADC1->SMPR2 |= ADC_SMPR2_SMP1_2;  // 84 cycles
    ADC1->CR1 |= ADC_CR1_EOCIE;  // enable end-of-conversion interrupt
    ADC1->CR2 |= ADC_CR2_ADON;  // enable ADC

    NVIC_EnableIRQ(ADC_IRQn);
}

void adc_interrupt_start(void)
{
    ADC1->CR2 |= ADC_CR2_SWSTART;  // start conversion
}

void ADC_IRQHandler(void)
{
    if (ADC1->SR & ADC_SR_EOC)
    {
        adc_interrupt_result = (uint16_t)(ADC1->DR);  // read result (clears EOC flag)
        adc_interrupt_done = 1;  // indicate conversion is complete
    }
}