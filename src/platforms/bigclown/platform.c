/*
 * This file is part of the Black Magic Debug project.
 *
 * Copyright (C) 2011  Black Sphere Technologies Ltd.
 * Written by Gareth McMullin <gareth@blacksphere.co.nz>
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

/* This file implements the platform specific functions for the STM32
 * implementation.
 */

#include "general.h"
#include "cdcacm.h"
#include "usbuart.h"
#include "morse.h"

#include <libopencm3/stm32/l0/rcc.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/syscfg.h>
#include <libopencm3/stm32/crs.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/cortex.h>

jmp_buf fatal_error_jmpbuf;
extern uint32_t _ebss;

void platform_init(void)
{
	/* Check the USER button*/
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);


	//hub rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_48MHZ]);
	/* PLL from HSI16, just to exercise that code */
	struct rcc_clock_scale myclock = {
		.ahb_frequency = 32e6,
		.apb1_frequency = 32e6,
		.apb2_frequency = 32e6,
		.flash_waitstates = 1,
		.pll_source = RCC_CFGR_PLLSRC_HSI16_CLK, /* not even sure there's hse on l053 disco */
		/* .msi_range  doesn't matter */
		.pll_mul = RCC_CFGR_PLLMUL_MUL4,
		.pll_div = RCC_CFGR_PLLDIV_DIV2,
		.hpre = RCC_CFGR_HPRE_NODIV,
		.ppre1 = RCC_CFGR_PPRE1_NODIV,
		.ppre2 = RCC_CFGR_PPRE2_NODIV,
	};
	rcc_clock_setup_pll(&myclock);

	/* HSI48 needs the vrefint turned on */
	rcc_periph_clock_enable(RCC_SYSCFG);
	SYSCFG_CFGR3 |= SYSCFG_CFGR3_ENREF_HSI48 | SYSCFG_CFGR3_EN_VREFINT;
	while (!(SYSCFG_CFGR3 & SYSCFG_CFGR3_REF_HSI48_RDYF));

	/* For USB, but can't use HSI48 as a sysclock on L0 */
	crs_autotrim_usb_enable();
	rcc_set_hsi48_source_rc48();

	rcc_osc_on(RCC_HSI48);
	rcc_wait_for_osc_ready(RCC_HSI48);

	/* Enable peripherals */
	rcc_periph_clock_enable(RCC_USB);
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOD);

	rcc_periph_clock_enable(RCC_GPIOH);
	rcc_periph_clock_enable(RCC_CRC);

	/* Set up USB Pins and alternate function*/
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO11 | GPIO12);
	gpio_set_af(GPIOA, GPIO_AF10, GPIO11 | GPIO12);

	//GPIOC_OSPEEDR &=~0xF30;
	//GPIOC_OSPEEDR |= 0xA20;

	GPIOA_OSPEEDR |= GPIO_OSPEED(SWDIO_PIN, GPIO_SPEED) | GPIO_OSPEED(SWCLK_PIN, GPIO_SPEED); // (0x03 << (1*2)) | (0x03 << (2*2));

	gpio_mode_setup(JTAG_PORT, GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE,
			TMS_PIN | TCK_PIN | TDI_PIN);

	gpio_mode_setup(TDO_PORT, GPIO_MODE_INPUT,
			GPIO_PUPD_NONE,
			TDO_PIN);

	gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE,
			LED_UART | LED_IDLE_RUN | LED_ERROR | LED_BOOTLOADER);


	// USART RX IRQ gpio
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9);
	// TIM USART
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8);

		// set IRQ priorities manually, nvic_set_priority dont work
		// on STM32F0, because this CPU allows only WORD access to these registers
		MMIO32(0xE000E40C) = 0xC0000000;
		MMIO32(0xE000E418) = 0x40000000;
		MMIO32(0xE000E41C) = 0x80000000;

	

	platform_timing_init();
	usbuart_init();
	cdcacm_init();
}

void platform_srst_set_val(bool assert) { (void)assert; }
bool platform_srst_get_val(void) { return false; }

const char *platform_target_voltage(void)
{
	return "ABSENT!";
}

void platform_request_boot(void)
{
	uint32_t *magic = (uint32_t *) &_ebss;
	magic[0] = BOOTMAGIC0;
	magic[1] = BOOTMAGIC1;
	scb_reset_system();
}
