/*
 * This file is part of the Black Magic Debug project.
 *
 * Copyright (C) 2015 Gareth McMullin <gareth@blacksphere.co.nz>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "general.h"
#include "morse.h"

#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>


uint8_t running_status;
static volatile uint32_t time_ms;

void platform_timing_init(void)
{
	/* Setup heartbeat timer */
	//hub systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
	//hub systick_set_reload(900000);	/* Interrupt us at 10 Hz */

	systick_interrupt_disable();
	systick_counter_disable();

	systick_set_frequency(10, rcc_ahb_frequency);
	#if defined(STM32L0)
	//SCB_SHPR3 |= 0x03 << 24;
	//MMIO32(SCB_BASE + 0x20) |= 0x03 << 24;
	SCB_SHPR(11) &= ~((15 << 4) & 0xff);
	SCB_SHPR(11) |= ((0x03 << 4) & 0xff);
	#else
	SCB_SHPR(11) &= ~((15 << 4) & 0xff);
	SCB_SHPR(11) |= ((14 << 4) & 0xff);
	#endif
	systick_interrupt_enable();
	systick_counter_enable();
}

void platform_delay(uint32_t ms)
{
	platform_timeout timeout;
	platform_timeout_set(&timeout, ms);
	while (!platform_timeout_is_expired(&timeout));
}

void sys_tick_handler(void)
{
	if(running_status)
		gpio_toggle(LED_PORT, LED_IDLE_RUN);

	time_ms += 100;

	SET_ERROR_STATE(morse_update());
}

uint32_t platform_time_ms(void)
{
	return time_ms;
}

