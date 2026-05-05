/**
 * @file    uart_debug.h
 * @brief   Bare-metal USART2 driver for debug output.
 *
 * Transmits text and numeric data over PA2 (USART2 TX) at 115200 baud.
 * Intended for logging ADC measurements and CPU cycle counts to a
 * serial monitor during development.
 */

#ifndef UART_DEBUG_H
#define UART_DEBUG_H

#include <stdint.h>

/** @brief Initializes USART2 on PA2 at the baud rate defined in board.h. */
void uart_init(void);

/** @brief Transmits a null-terminated string over USART2. */
void uart_print(const char *str);

/** @brief Transmits a string followed by CR+LF. */
void uart_println(const char *str);

/** @brief Transmits a 32-bit unsigned integer as decimal text. */
void uart_print_uint(uint32_t value);

#endif /* UART_DEBUG_H */
