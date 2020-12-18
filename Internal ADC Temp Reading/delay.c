/*
 * delay.c
 *
 * Created: 11/30/2020 6:34:19 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 *
 * Description:
 *		Software based delays. Receives a 32 unsigned integer value.
 * 
 * Notes:
 *		According to ARM, a nop is 1 clock cycle operation.
 * 
 * Resources:
 *		[2] https://developer.arm.com/documentation/ddi0484/c/Programmers-Model/Instruction-set-summary
 */ 

#include "delay.h"

/*      Timing Equation: (14+(x*14)+6)/F_CPU_Hz = delay_time_seconds					*/
/*		Delay Time | 32768	| 4MHz		| 8MHz		| 12MHz		| 16MHz		| 48MHz		|
		-----------------------------------------------------------------------------
		  1 [s]	   | 2339	| 285713	| 571427	| 857141	| 1142855	| 3428570	|
	    100 [ms]   | 232	| 28570		| 57141		| 85713		| 114284	| 342856	|
		 10 [ms]   | 22		| 2855		| 5713		| 8570		| 11427		| 34284		|
		  1 [ms]   | 0.91	| 284		| 570		| 856		| 1141		| 3427		|
	    100 [us]   |   X	| 27		| 56		| 84		| 113		| 341		|	*/
	
void __attribute__ ((__noinline__, __optimize__("O0"))) delayus_32bit(uint32_t CPU_Hz, uint32_t delay_us) {
	
	float delay_s = (float)delay_us / 1000000;								// convert from [us] to [s]
	float limit_f = ((delay_s *  (float)CPU_Hz) - 20) / 14;	// calculate delay limit
	
	if (limit_f < 0){
		limit_f = 1;		// Run at least one iteration in case 32 KHz clock is used.
	}
	uint32_t limit = (uint32_t)limit_f;
	
	// ------------------------------------------------- Time Equation Calculated for Loop Below
	for (uint32_t x=0; x < limit; x++){
		__asm("nop");
	}
}

void __attribute__ ((__noinline__, __optimize__("O0"))) delayms_32bit(uint32_t CPU_Hz, uint32_t delay_ms) {
	
	float delay_s = (float)delay_ms / 1000;									// convert from [us] to [s]
	float limit_f = ((delay_s *  (float)CPU_Hz) - 20) / 14;	// calculate delay limit
	
	if (limit_f < 0){
		limit_f = 1;		// Run at least one iteration in case 32 KHz clock is used.
	}
	uint32_t limit = (uint32_t)limit_f;
	
	// ------------------------------------------------- Time Equation Calculated for Loop Below
	for (uint32_t x=0; x < limit; x++){
		__asm("nop");
	}
}


void __attribute__ ((__noinline__, __optimize__("O0"))) cycle_delay(uint32_t number_of_cycles) {
	// ------------------------------------------------- Time Equation Calculated for Loop Below
	for (uint32_t x=0; x < number_of_cycles; x++){
		__asm("nop");
	}
}