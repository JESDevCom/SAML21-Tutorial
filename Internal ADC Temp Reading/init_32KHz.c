/*
 * init_32KHz.c
 *
 * Created: 12/8/2020 8:34:11 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Initializes and turns on the a 32768 Hz clock. 
 *		Choices are (1) internal 32K OSC, (2) external 32K OSC, and (3) Ultra Low Power 32K RC oscillator
 *		The internal 32K OSC is supposed to be very accurate, but in my testing, the external 32K appears
 *		more accurate.
 * 
 * Resources:
 *		[1] SAM L21 Family Data Sheet
 */ 

#include "init_32KHz.h"

/*****************************************************************************************************
										Start 32KHz Clock
******************************************************************************************************
Description:
	This function enables different 32KHz
Use:
	Input		| Value
	OSCULP32KHz	| 32001
	OSC32KHz	| 32002
	XOSC32KHz	| 32003
----------------------------------------------------------------------------------------------------*/	
void init_32KHz(uint32_t clk_src){
	
	// Calibrate OSC32K [if you don't your timer will not be very accurate]..
	uint32_t calib = (*((uint32_t*)NVMCTRL_OTP5) & NVMCAL_OSC32K_MASK) >> NVMCAL_OSC32K_POS;
				
	switch(clk_src){
		case OSC32KHz:
			// ----------------------- Configure OSC32K --------------------------------------- (Pg. 261) [1]
			OSC32KCTRL->OSC32K.reg = (	OSC32KCTRL_OSC32K_ENABLE	  |	// Enable Int Oscillator
										OSC32KCTRL_OSC32K_CALIB(calib)| // Calibrate OSC32K
										OSC32KCTRL_OSC32K_EN32K		  | // Enable 32KHz Output
										OSC32KCTRL_OSC32K_STARTUP(6));	// 64 CC Startup Delay  (Pg. 232) [1]
										while(OSC32KCTRL->STATUS.bit.OSC32KRDY == 0); //		(Pg. 256) [1]
			break;
		case XOSC32KHz:	
			// ----------------------- Configure XOSC32K -------------------------------------- (Pg. 259) [1]
			OSC32KCTRL->XOSC32K.reg =(	OSC32KCTRL_XOSC32K_ENABLE	|	// Enable Ext Oscillator
										OSC32KCTRL_XOSC32K_XTALEN	|	// Enable Ext Input
										OSC32KCTRL_XOSC32K_EN32K	|	// Enable 32K Output
										OSC32KCTRL_XOSC32K_STARTUP(1));	// 0.13 Startup Delay
										while(OSC32KCTRL->STATUS.bit.XOSC32KRDY == 0); //		(Pg. 256) [1]
			break;
		default:
			// ----------------------- Configure OSCULP32K ------------------------------------ (Pg. 263) [1]
			//  Enabled at startup by default.
			break;
	}	
}


/*****************************************************************************************************
										Stop 32KHz Clock
******************************************************************************************************
Description:
	This function stops different 32KHz clocks.
	Can't stop OSCULP32K. Starts by default at startup.
Use:
	Input		| Value
	OSC32KHz	| 32002
	XOSC32KHz	| 32003
----------------------------------------------------------------------------------------------------*/	

void stop_32KHz(uint32_t clk_src){
	switch(clk_src){
		case OSC32KHz:
			OSC32KCTRL->OSC32K.bit.ENABLE = 0;
			break;
		case XOSC32KHz:
			OSC32KCTRL->XOSC32K.bit.ENABLE = 0;
			break;
		default:
			break;
	}
}