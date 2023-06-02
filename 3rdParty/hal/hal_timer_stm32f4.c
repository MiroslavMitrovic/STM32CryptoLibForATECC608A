/*
 * hal_timer_stm32f4.c
 *
 *  Created on: May 5, 2023
 *      Author: mmitr
 */


#include "hal_timer_stm32f4.h"






__STATIC_INLINE void DWT_Delay_us(volatile uint32_t au32_microseconds)
{
  uint32_t au32_initial_ticks = DWT->CYCCNT;
  uint32_t au32_ticks = (HAL_RCC_GetHCLKFreq() / 1000000);
  au32_microseconds = au32_microseconds * au32_ticks;
  while ((DWT->CYCCNT - au32_initial_ticks) < au32_microseconds-au32_ticks);
}

void hal_delay_us(uint32_t delay)
{
	DWT_Delay_us(delay);
}

void hal_delay_ms(uint32_t msec)
{
    HAL_Delay(msec);
}
