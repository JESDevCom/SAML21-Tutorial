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
 */ 

#include "init_32KHz.h"

/*****************************************************************************************************
										Start 32KHz Clock
******************************************************************************************************
Description:
	This function enables different 32KHz
Use:
	Input		| Value
	OSCULP32KHz	| 4
	OSC32KHz	| 5
	XOSC32KHz	| 6
----------------------------------------------------------------------------------------------------*/	
void init_32KHz(uint8_t clk_sel){
	
	// Calibrate OSC32K [if you don't your timer will not be very accurate]..
	uint32_t calib = (*((uint32_t*)NVMCTRL_OTP5) & OSC32KCAL_MASK) >> OSC32KCAL_POS;
				
	switch(clk_sel){
		case 5:
			// ----------------------- Configure OSC32K --------------------------------------- (Pg. 261) [1]
			OSC32KCTRL->OSC32K.reg = (	OSC32KCTRL_OSC32K_ENABLE	  |	// Enable Int Oscillator
										OSC32KCTRL_OSC32K_CALIB(calib)| // Calibrate OSC32K
										OSC32KCTRL_OSC32K_EN32K		  | // Enable 32KHz Output
										OSC32KCTRL_OSC32K_STARTUP(6));	// 64 CC Startup Delay  (Pg. 232) [1]
										while(OSC32KCTRL->STATUS.bit.OSC32KRDY == 0); //		(Pg. 256) [1]
			break;
		case 6:	
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
	This function stops different 32KHz clocks
Use:
	Input		| Value
	OSC32KHz	| 5
	XOSC32KHz	| 6
----------------------------------------------------------------------------------------------------*/	

void stop_32KHz(uint8_t clk_sel){
	switch(clk_sel){
		case 5:
			OSC32KCTRL->OSC32K.bit.ENABLE = 0;
			break;
		case 6:
			OSC32KCTRL->XOSC32K.bit.ENABLE = 0;
			break;
		default:
			break;
	}
}