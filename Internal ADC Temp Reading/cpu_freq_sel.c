/*
 * init_clock.c
 *
 * Created: 12/6/2020 7:24:23 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		This file contains the top level handler for selecting the CPU frequency.
 *
 * Input Parameters:
 *		Definition	| Value
 *      ------------------------
 *		OSC4MHz		| 4000000
 *		OSC8MHz		| 8000000
 *		OSC12MHz	| 12000000
 *		OSC16MHz	| 16000000
 *		OSCULP32KHz | 32001
 *		OSC32KHz	| 32002
 *		XOSC32KHz	| 32003
 *		OSC48MHz	| 48000000
 *
 * Notes:
 *		[1] "When changing to a higher performance level, the bus frequency can be increased only after
 *		the Performance Level Ready flag in the Interrupt Flag Status and Clear (INTFLAG.PLRDY) bit
 *		set to '1', indicating that the performance level transition is complete." [1]
 *
 *		[2] Maximum Peripheral Clock Frequency are limited by the CPU frequency, which is limited by
 *		the Performance Level Value. (See Table 46-6, Pg. 1041) for more info. Also, PL0 can run the
 *		internal OSC at 4MHz, 8MHz, and 12 MHz. You must go PL2 for OSC16M to run at the full 16MHz.
 *
 *		[3] Startup Timing: (Resource: [1])
 *				48M			|	9.1 [us] max
 *				32K IntOSC	|	2  Clock Cycles max
 *		16/12/8/4M	IntOSC	|   3.1 [us] max
 *				32K ExtOSC	|   8.2k Clock Cycles max
 *
 *		[4] cpu_freq_sel designed with while statements to check flags to make sure that the
 *		clock sources have fully initialized before being switched from the current stable OSC 
 *		to the new OSC source.
 *
 * Resources:
 *		[1] SAM L21 Family Data Sheet
 */ 

#include "cpu_freq_sel.h"

void cpu_freq_sel(uint32_t clk_hz){

	const uint16_t freq_divider = 1;
	
	PD_sel(clk_hz);	/* Set Performance Level */	
	
	switch(clk_hz){
		case OSC48MHz:		
			init_DFLL48M(OSCULP32KHz);
			F_CPU = 48000000;			
			break;
		case XOSC32KHz:		
			init_32KHz(XOSC32KHz);
			switch_cpu_source(XOSC32KHz); // switch source on GCLK0 for CPU to run at the new clock source.
			F_CPU = 32768;
			break;
		case OSC32KHz:
			init_32KHz(OSC32KHz);
			switch_cpu_source(OSC32KHz); 
			F_CPU = 32768;
			break;
		case OSCULP32KHz:
			init_32KHz(OSCULP32KHz);
			switch_cpu_source(OSCULP32KHz); 
			F_CPU = 32768;
			break;
		default:
			init_IntOSC(clk_hz, freq_divider);	
			F_CPU = clk_hz;
			break;
	}
	
	delayus_32bit(clk_hz, 400); // run non-critical code for 400 [us]
}



/*****************************************************************************************************
									 Performance Domain Select
******************************************************************************************************
Description:
	This function selects the Performance Power Mode based on the CPUs Frequency requirement.

 * Input Parameters:
 *		Definition	| Value
 *      ------------------------
 *		OSC4MHz		| 4000000
 *		OSC8MHz		| 8000000
 *		OSC12MHz	| 12000000
 *		OSC16MHz	| 16000000
 *		OSCULP32KHz | 32001
 *		OSC32KHz	| 32002
 *		XOSC32KHz	| 32003
 *		OSC48MHz	| 48000000
 

Notes: 
	[1] Enable Highest Performance Power Mode (Pg. 201) [1] 
	[2] Maximum Peripheral Clock Frequency are limited by the CPU frequency, which is limited by
		the Performance Level Value. (See Table 46-6, Pg. 1041) for more info. Also, PL0 can run the
		internal OSC at 4MHz, 8MHz, and 12 MHz. You must go PL2 for OSC16M to run at the full 16MHz. 
----------------------------------------------------------------------------------------------------*/							
	
void PD_sel(uint32_t clk_src){
	
	switch(clk_src){
		case OSC16MHz: case OSC48MHz:
			PM->INTFLAG.bit.PLRDY = 1;	// Writing a '1' to this bit clears the Performance Ready Interrupt flag (Pg. 204 [1])
			PM->PLCFG.bit.PLSEL = 2;	// Enable Performance Level 2 (Pg. 201 [1])
			while(PM->INTFLAG.bit.PLRDY == 0); // Wait till Ready
			break;
		
		default:
			/*
			PM->INTFLAG.bit.PLRDY = 1;	// Writing a '1' to this bit clears the Performance Ready Interrupt flag (Pg. 204 [1])
			PM->PLCFG.bit.PLSEL = 0;	// Enable Performance Level 0 (Pg. 201 [1])
			while(PM->INTFLAG.bit.PLRDY == 0); // Wait till Ready
			*/
			break;			
		}
}



