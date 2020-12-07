/*
 * init_clock.h
 *
 * Created: 12/6/2020 7:24:41 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef INIT_CLOCK_H_
#define INIT_CLOCK_H_

/****************************************************************
						   Header Files
****************************************************************/
#include "sam.h"


/****************************************************************
						   Definitions
****************************************************************/
#define OSC4MHz		0xFF & OSCCTRL_OSC16MCTRL_FSEL_4
#define OSC8MHz		0xFF & OSCCTRL_OSC16MCTRL_FSEL_8
#define OSC12MHz	0xFF & OSCCTRL_OSC16MCTRL_FSEL_12
#define OSC16MHz	0xFF & OSCCTRL_OSC16MCTRL_FSEL_16

/****************************************************************
				       Function Prototypes
****************************************************************/
void osc_freq_sel(uint8_t clk_sel);



#endif /* INIT_CLOCK_H_ */