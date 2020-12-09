/*
 * init_32KHz.h
 *
 * Created: 12/8/2020 8:34:28 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef INIT_32KHZ_H_
#define INIT_32KHZ_H_

/****************************************************************
						   Header Files
****************************************************************/
#include "sam.h"


/****************************************************************
						  Definitions
****************************************************************/
#define OSC4MHz		0xFF & OSCCTRL_OSC16MCTRL_FSEL_4	// Val = 0
#define OSC8MHz		0xFF & OSCCTRL_OSC16MCTRL_FSEL_8	// Val = 1
#define OSC12MHz	0xFF & OSCCTRL_OSC16MCTRL_FSEL_12	// Val = 2
#define OSC16MHz	0xFF & OSCCTRL_OSC16MCTRL_FSEL_16   // Val = 3
#define OSCULP32KHz	4
#define OSC32KHz	5
#define XOSC32KHz	6
#define OSC48MHz	255

// Table 11-5 NVM Software Calibration Area Mapping (Pg. 45)
#define OSC32KCAL_MASK				0x00001FC0
#define OSC32KCAL_POS				6

/****************************************************************
					   Function Prototypes 
****************************************************************/
void init_32KHz(uint8_t clk_sel);
void stop_32KHz(uint8_t clk_sel);



#endif /* INIT_32KHZ_H_ */