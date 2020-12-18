/*
 * init_IntOSC.c
 *
 * Created: 12/10/2020 2:00:37 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 * 
 * Description:
 *		Initialize the internal oscillator frequency
 *
 * Resources:
 *		[1] SAM L21 Family Data Sheet
 */ 

#include "init_IntOSC.h"

void init_IntOSC(uint32_t clk_hz, uint16_t divider){
	
	if (clk_hz != F_CPU){
		// Oscillator Frequency Selection (Pg. 234) [1]
		switch(clk_hz){
			case 16000000:
				OSCCTRL->OSC16MCTRL.reg = (OSCCTRL_OSC16MCTRL_ENABLE | OSCCTRL_OSC16MCTRL_RUNSTDBY | OSCCTRL_OSC16MCTRL_FSEL_16);
				break;
			case 12000000:
				OSCCTRL->OSC16MCTRL.reg = (OSCCTRL_OSC16MCTRL_ENABLE | OSCCTRL_OSC16MCTRL_RUNSTDBY | OSCCTRL_OSC16MCTRL_FSEL_12);
				break;
			case 8000000:
				OSCCTRL->OSC16MCTRL.reg = (OSCCTRL_OSC16MCTRL_ENABLE | OSCCTRL_OSC16MCTRL_RUNSTDBY | OSCCTRL_OSC16MCTRL_FSEL_8);
				break;
			case 4000000:
				OSCCTRL->OSC16MCTRL.reg = (OSCCTRL_OSC16MCTRL_ENABLE | OSCCTRL_OSC16MCTRL_RUNSTDBY |OSCCTRL_OSC16MCTRL_FSEL_4);
				break;		
		}
		while(OSCCTRL->STATUS.bit.OSC16MRDY == 0);
	
		// If divider greater than 1, then divide down the clock
		if (divider > 1){
			GCLK->GENCTRL[0].bit.DIV = GCLK_GENCTRL_DIV(divider);
		}	
		switch_cpu_source(clk_hz); // switch source on GCLK0 for CPU to run at the new clock source.
	}
	
}
