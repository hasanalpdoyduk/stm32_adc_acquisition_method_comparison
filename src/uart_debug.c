/**
 * @file    uart_debug.c
 * @brief   USART2 initialization and transmit implementation.
 *
 * Configures PA2 as USART2 TX (AF7), sets baud rate from board.h,
 * and provides blocking byte-by-byte transmission functions.
 */

#include "uart_debug.h"
#include "config.h"

void uart_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Set PA2 to Alternate Function mode (MODER = 0b10)
    GPIOA->MODER &= ~(3U << (UART_TX_PIN * 2));
    GPIOA->MODER |=  (2U << (UART_TX_PIN * 2));

    // Set PA2 alternate function to AF7 (USART2)
    GPIOA->AFR[0] &= ~(0xFU << (UART_TX_PIN * 4));
    GPIOA->AFR[0] |=  (7U   << (UART_TX_PIN * 4));

    // BRR = APB1 clock / baud rate (HSI default: 16 MHz)
    USART2->BRR = APB1_CLK_HZ / UART_BAUD;

    USART2->CR1 = USART_CR1_UE | USART_CR1_TE;
}

static void uart_send_byte(uint8_t byte)
{
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = byte;
}

void uart_print(const char *str)
{
    while (*str)
        uart_send_byte((uint8_t)*str++);
}

void uart_println(const char *str)
{
    uart_print(str);
    uart_send_byte('\r');
    uart_send_byte('\n');
}

void uart_print_uint(uint32_t value)
{
    char buf[10];
    uint8_t i = 0;

    if (value == 0)
    {
        uart_send_byte('0');
        return;
    }

    while (value > 0)
    {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }

    // digits were collected in reverse order
    while (i > 0)
        uart_send_byte((uint8_t)buf[--i]);
}
