/**
 * @file    main.c
 * @brief   Entry point. Initializes peripherals and runs the ADC comparison loop.
 */

#include "config.h"
#include "profiler.h"
#include "uart_debug.h"
#include "adc_polling.h"
#include "adc_interrupt.h"
#include "adc_dma.h"
#include "filters.h"

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
    adc_interrupt_init();
    adc_dma_init();

    volatile uint32_t bg_counter;

    MovingAvgFilter polling_ma_f, interrupt_ma_f, dma_ma_f;
    FirFilter       polling_fir_f, interrupt_fir_f, dma_fir_f;
    uint16_t        polling_iir_prev = 0, interrupt_iir_prev = 0, dma_iir_prev = 0;

    moving_avg_init(&polling_ma_f);
    moving_avg_init(&interrupt_ma_f);
    moving_avg_init(&dma_ma_f);
    fir_init(&polling_fir_f);
    fir_init(&interrupt_fir_f);
    fir_init(&dma_fir_f);

    while (1)
    {
        bg_counter = 0;
        adc_polling_done = 0;
        profiler_start();
        adc_polling_start();
        while (!adc_polling_done)
        {
            adc_polling_poll();
            bg_counter++;
        }
        uint32_t adc_polling_cycles = profiler_stop();
        uart_print("POLLING result=");
        uart_print_uint(adc_polling_result);
        uart_print(" cycles=");
        uart_print_uint(adc_polling_cycles);
        uart_print(" bg=");
        uart_print_uint(bg_counter);
        uart_println("");

        profiler_start();
        uint16_t polling_ma = moving_avg_update(&polling_ma_f, adc_polling_result);
        uint32_t polling_ma_cycles = profiler_stop();
        uart_print("POLLING ma=");
        uart_print_uint(polling_ma);
        uart_print(" cycles=");
        uart_print_uint(polling_ma_cycles);
        uart_println("");

        profiler_start();
        polling_iir_prev = iir_update(adc_polling_result, polling_iir_prev);
        uint32_t polling_iir_cycles = profiler_stop();
        uart_print("POLLING iir=");
        uart_print_uint(polling_iir_prev);
        uart_print(" cycles=");
        uart_print_uint(polling_iir_cycles);
        uart_println("");

        profiler_start();
        uint16_t polling_fir = fir_update(&polling_fir_f, adc_polling_result);
        uint32_t polling_fir_cycles = profiler_stop();
        uart_print("POLLING fir=");
        uart_print_uint(polling_fir);
        uart_print(" cycles=");
        uart_print_uint(polling_fir_cycles);
        uart_println("");

        bg_counter = 0;
        adc_interrupt_done = 0;
        profiler_start();
        adc_interrupt_start();
        while (!adc_interrupt_done)
            bg_counter++;
        uint32_t adc_interrupt_cycles = profiler_stop();
        uart_print("INTERRUPT result=");
        uart_print_uint(adc_interrupt_result);
        uart_print(" cycles=");
        uart_print_uint(adc_interrupt_cycles);
        uart_print(" bg=");
        uart_print_uint(bg_counter);
        uart_println("");

        profiler_start();
        uint16_t interrupt_ma = moving_avg_update(&interrupt_ma_f, adc_interrupt_result);
        uint32_t interrupt_ma_cycles = profiler_stop();
        uart_print("INTERRUPT ma=");
        uart_print_uint(interrupt_ma);
        uart_print(" cycles=");
        uart_print_uint(interrupt_ma_cycles);
        uart_println("");

        profiler_start();
        interrupt_iir_prev = iir_update(adc_interrupt_result, interrupt_iir_prev);
        uint32_t interrupt_iir_cycles = profiler_stop();
        uart_print("INTERRUPT iir=");
        uart_print_uint(interrupt_iir_prev);
        uart_print(" cycles=");
        uart_print_uint(interrupt_iir_cycles);
        uart_println("");

        profiler_start();
        uint16_t interrupt_fir = fir_update(&interrupt_fir_f, adc_interrupt_result);
        uint32_t interrupt_fir_cycles = profiler_stop();
        uart_print("INTERRUPT fir=");
        uart_print_uint(interrupt_fir);
        uart_print(" cycles=");
        uart_print_uint(interrupt_fir_cycles);
        uart_println("");

        bg_counter = 0;
        adc_dma_done = 0;
        profiler_start();
        adc_dma_start();
        while (!adc_dma_done)
            bg_counter++;
        uint32_t adc_dma_cycles = profiler_stop();
        uart_print("DMA result=");
        uart_print_uint(adc_dma_result);
        uart_print(" cycles=");
        uart_print_uint(adc_dma_cycles);
        uart_print(" bg=");
        uart_print_uint(bg_counter);
        uart_println("");

        profiler_start();
        uint16_t dma_ma = moving_avg_update(&dma_ma_f, adc_dma_result);
        uint32_t dma_ma_cycles = profiler_stop();
        uart_print("DMA ma=");
        uart_print_uint(dma_ma);
        uart_print(" cycles=");
        uart_print_uint(dma_ma_cycles);
        uart_println("");

        profiler_start();
        dma_iir_prev = iir_update(adc_dma_result, dma_iir_prev);
        uint32_t dma_iir_cycles = profiler_stop();
        uart_print("DMA iir=");
        uart_print_uint(dma_iir_prev);
        uart_print(" cycles=");
        uart_print_uint(dma_iir_cycles);
        uart_println("");

        profiler_start();
        uint16_t dma_fir = fir_update(&dma_fir_f, adc_dma_result);
        uint32_t dma_fir_cycles = profiler_stop();
        uart_print("DMA fir=");
        uart_print_uint(dma_fir);
        uart_print(" cycles=");
        uart_print_uint(dma_fir_cycles);
        uart_println("");
    }
}
