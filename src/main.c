/**
 * @file    main.c
 * @brief   Entry point. Initializes peripherals and runs the ADC comparison loop.
 */

#include "config.h"
#include "profiler.h"
#include "uart_debug.h"
#include "adc_polling.h"

static void SystemClock_Config(void)
{
    // must be set before increasing clock speed; Flash cannot keep up with 168 MHz
    FLASH->ACR = FLASH_ACR_LATENCY_5WS
               | FLASH_ACR_ICEN
               | FLASH_ACR_DCEN
               | FLASH_ACR_PRFTEN;

    // HSI is on by default; wait for hardware to confirm it is stable
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY));

    /*
     * PLLCFGR must be written while PLL is off.
     * HSI(16) / PLLM(8) = 2 MHz -> x PLLN(168) = 336 MHz -> / PLLP(2) = 168 MHz SYSCLK
     *                                                       -> / PLLQ(7) =  48 MHz USB
     */
    RCC->PLLCFGR = (8U   << RCC_PLLCFGR_PLLM_Pos)
                 | (168U << RCC_PLLCFGR_PLLN_Pos)
                 | (0U   << RCC_PLLCFGR_PLLP_Pos)
                 | (7U   << RCC_PLLCFGR_PLLQ_Pos);

    // wait for PLL to lock before switching the clock source
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    /*
    SYSCLK 168 MHz → AHB ÷1 → 168 MHz (CPU, DMA)
                       → APB1 ÷4 →  42 MHz (UART, SPI2/3...)
                       → APB2 ÷2 →  84 MHz (ADC, SPI1...)
    */
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1
               | RCC_CFGR_PPRE1_DIV4
               | RCC_CFGR_PPRE2_DIV2;

    // SW bits select the source; SWS bits are set by hardware when switch completes
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

int main(void)
{
    SystemClock_Config();
    profiler_init();
    uart_init();
    adc_polling_init();

    while (1)
    {
        profiler_start();
        uint16_t result = adc_polling_read();
        uint32_t cycles = profiler_stop();

        uart_print("POLLING result=");
        uart_print_uint(result);
        uart_print(" cycles=");
        uart_print_uint(cycles);
        uart_println("");
    }
}
