/*
 * ISR_TC0.c
 *
 * Created: 12/2/2020 7:36:06 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		This file contains the TC0_Handler which is the interrupt vector
 *		for Timer 0.
 *
 * Notes:
 *		The name TC0_Handler is not a function name I made up, it is
 *		pre-defined in the sam.h header file. If you were to use timer1
 *		the pre-defined function name would be TC1_Handler.
 *
 *		It is good practice to clear MC0 flag last. There could be issues
 *		if you clear MC0 flag first in cases when your code spends to much time
 *		inside the interrupt and the interrupt is to fast for your code to 
 *		complete inside the interrupt.
 */ 

#include "ISR_TC0.h"


// =====================================================================
//                     Interrupt Service: Timer 0
// =====================================================================
void TC0_Handler(void){

	/* ========== Service Overflow Flag ========= */
	/*if ( TC0->COUNT8.INTFLAG.bit.OVF == 1 ){
	}*/
	
	/* =========== Service Error Flag =========== */
	/*if ( TC0->COUNT8.INTFLAG.bit.ERR == 1 ){
	}*/
	
	/* ========= Service Compare Flag =========== */
	if ( TC0->COUNT8.INTFLAG.bit.MC0 == 1 ){
		
		//Toggle LED 0
		PORT->Group[portb].OUTTGL.reg = led;
		
		// Clear TC0 MC0 Interrupt Flag Register
		TC0->COUNT8.INTFLAG.reg |= TC_INTFLAG_MC0;
	}
}
