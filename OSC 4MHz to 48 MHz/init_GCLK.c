/*
 * init_GCLK.c
 *
 * Created: 12/10/2020 1:21:29 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		This function sets up a Generic Clock for any clock.
 * 
 */ 

#include "init_GCLK.h"


void init_GCLK(uint32_t gen_num, uint32_t clk_src, uint16_t divider){
	/* This function initializes generic clocks
	:param gen_num: the generic clock controller number. Possible values: 0, 1, 2, 3, 4, 5, 6, 7, 8
	:param clk_src: selects which clock will be the input into the generic clock controller.
	:param divider: divide the clock by a 16-bit value. */
	
	// Initialize Generic Clock Divider
	GCLK->GENCTRL[gen_num].reg |= GCLK_GENCTRL_DIV(divider);
	
	// Initialize Generic Clock Source
	switch(clk_src){
		case OSCULP32KHz:	GCLK->GENCTRL[gen_num].reg |= GCLK_GENCTRL_SRC_OSCULP32K;	break;
		case OSC32KHz:		GCLK->GENCTRL[gen_num].reg |= GCLK_GENCTRL_SRC_OSC32K;		break;
		case XOSC32KHz:		GCLK->GENCTRL[gen_num].reg |= GCLK_GENCTRL_SRC_XOSC32K;		break;
		case OSC16MHz:		GCLK->GENCTRL[gen_num].reg |= GCLK_GENCTRL_SRC_OSC16M;		break;
		case OSC48MHz:		GCLK->GENCTRL[gen_num].reg |= GCLK_GENCTRL_SRC_DFLL48M;		break;
		default:			GCLK->GENCTRL[gen_num].reg |= GCLK_GENCTRL_SRC_OSC32K;		break;
	}
	
	// Enable Generic Clock
	GCLK->GENCTRL[gen_num].bit.GENEN = 1;
	
	// Wait till GCLK is stable and running
	while(GCLK->SYNCBUSY.bit.GENCTRL1 == 1);
}


void stop_GCLK(uint32_t gen_num){
	/* This function halts a generic clock controller
	:param gen_num: the generic clock controller number. Possible values: 0, 1, 2, 3, 4, 5, 6, 7, 8 
	*/
	GCLK->GENCTRL[gen_num].bit.GENEN = 0; // Stop Generic Clock
}

void switch_cpu_source(uint32_t clk_src){
	/* This function switches the source to the generic clock controller 0 that is connected to the cpu over to 
	   the new clk_src. 
	   :param clk_src: clk_src are defined in sys_def.h */
	
	// ------------------------------- Switch GCLK0 Source ------------------------ (Pg. 135) [1]
	switch(clk_src){
		case OSC48MHz:
			GCLK->GENCTRL[0].bit.SRC = GCLK_GENCTRL_SRC_DFLL48M;
			break;
		case OSC32KHz:
			GCLK->GENCTRL[0].bit.SRC = GCLK_GENCTRL_SRC_OSC32K;
			break;
		case XOSC32KHz:
			GCLK->GENCTRL[0].bit.SRC = GCLK_GENCTRL_SRC_XOSC32K;
			break;
		case OSCULP32KHz:
			GCLK->GENCTRL[0].bit.SRC = GCLK_GENCTRL_SRC_OSCULP32K;
			break;
		default:
			GCLK->GENCTRL[0].bit.SRC = GCLK_GENCTRL_SRC_OSC16M;
			break;
	}	
	while(GCLK->SYNCBUSY.bit.GENCTRL0); // wait till ready and stable
}