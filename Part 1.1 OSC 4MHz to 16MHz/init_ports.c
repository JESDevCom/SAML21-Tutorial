/*
 * init_ports.c
 *
 * Created: 11/30/2020 6:36:28 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		This file initializes ports on the SAML21J18B Xplained Pro Development board.
 *		PB10 | LED0
 *
 * Resources:
 *		[1] Atmel SAM L21 Xplained Pro User Guide
 *		[2]	SAM L21 Family Data Sheet
 *
 * Notes:
 *		With an oscilloscope test if the clock frequency can be seen at PORT_PB14 on the board
 */ 


#include "init_ports.h"

void init_ports(void){
	
	/* Initialize LED 0 */
	// The LED can be activated by driving the connected I/O line to GND [1].
	PORT->Group[portb].OUTSET.reg = led; // PB10 High, which turns the LED off
	PORT->Group[portb].DIRSET.reg = led; // PB10 Output
	
	/* Initialize PB14 To Output GCLK0 */
	//PORT->Group[portb].DIRSET.reg = PORT_PB14;			// Output
	//PORT->Group[portb].PINCFG[14].bit.PMUXEN = 1;			// Enable Peripheral MUX
	//PORT->Group[portb].PMUX[14>>1].bit.PMUXE = PMUX_H;	// Set the PMUX (Pg. 28) [2]
	
	/* Note: The following two lines are the same
	PORT->Group[portb].PMUX[14>>1].reg |= PORT_PMUX_PMUXE(MUX_PA14H_GCLK_IO0);
	PORT->Group[portb].PMUX[14>>1].bit.PMUXE = PMUX_H;
	*/
	
	/* Initialize PB13 To Output TC0 Signal */
	PORT->Group[portb].DIRSET.reg = PORT_PB13;			// Output
	PORT->Group[portb].PINCFG[13].bit.PMUXEN = 1;		// Enable Peripheral MUX
	PORT->Group[portb].PMUX[13>>1].bit.PMUXO = PMUX_E;	// Set the PMUX (Pg. 28) [2]
	
	
	delay(1000); // delay for ports to setup
}