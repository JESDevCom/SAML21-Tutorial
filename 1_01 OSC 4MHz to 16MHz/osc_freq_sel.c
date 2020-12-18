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
 * Notes:
 *		"When changing to a higher performance level, the bus frequency can be increased only after
 *		the Performance Level Ready flag in the Interrupt Flag Status and Clear (INTFLAG.PLRDY) bit
 *		set to '1', indicating that the performance level transition is complete." [1]
 */ 

#include "osc_freq_sel.h"

void osc_freq_sel(uint8_t clk_sel){


	// Enable Highest Performance Power Mode (Pg. 201) [1]
	PM->INTFLAG.bit.PLRDY = 1;	// Writing a '1' to this bit clears the Performance Ready Interrupt flag (Pg. 204) [1]
	PM->PLCFG.bit.PLSEL = 2;	// Enable Performance Level 2 (Pg. 201) [1]
	while(PM->INTFLAG.bit.PLRDY == 0); // Wait till Ready
	
	// Oscillator Frequency Selection (Pg. 234) [1]
	OSCCTRL->OSC16MCTRL.reg = (OSCCTRL_OSC16MCTRL_ENABLE | clk_sel);

}






