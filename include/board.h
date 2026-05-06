/**
 * @file    board.h
 * @brief   Hardware-specific definitions for STM32F407VG-DISC1.
 *
 * Contains pin assignments, clock frequency, and peripheral
 * settings tied to this specific board. Swap this file to
 * port the project to a different target.
 */

#ifndef BOARD_H
#define BOARD_H

#include "stm32f4xx.h"

// CPU frequency in Hz. STM32F407 maximum is 168 MHz
#define SYS_CLOCK_HZ    168000000UL

// APB1 bus frequency: 42 MHz (HCLK 168 MHz, APB1 prescaler /4)
#define APB1_CLK_HZ     42000000U

// ADC1 channel connected to PA1
#define ADC_CHANNEL     1

// USART2 baud rate. PA2 is the TX pin (Alternate Function 7)
#define UART_BAUD       115200

// PA2 pin number for USART2 TX
#define UART_TX_PIN     2

#endif /* BOARD_H */
