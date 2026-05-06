/**
 * @file    adc_polling.c
 * @brief   ADC1 polling mode implementation.
 *
 * Configures PA1 as analog input, enables ADC1 with 12-bit resolution,
 * and reads conversions by busy-waiting on the EOC flag.
 */

#include "adc_polling.h"
#include "config.h"

void adc_polling_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    GPIOA->MODER |= GPIO_MODER_MODER1;  // analog mode
    
    ADC1->CR1 &= ~ADC_CR1_RES;  // 00 = 12-bit resolution
    ADC1->SQR3 |= 1U;  // convert channel 1 first in sequence
    ADC1->SMPR2 &= ~ADC_SMPR2_SMP1;
    ADC1->SMPR2 |= ADC_SMPR2_SMP1_2;  // 84 cycles
    ADC1->CR2 |= ADC_CR2_ADON;  // enable ADC
}

uint16_t adc_polling_read(void)
{
    ADC1->CR2 |= ADC_CR2_SWSTART;  // start conversion
    while (!(ADC1->SR & ADC_SR_EOC));  // wait for conversion to complete
    return (uint16_t)(ADC1->DR);  // read result (clears EOC flag)
}

