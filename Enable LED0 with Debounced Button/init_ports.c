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
 *		LED on PB10
 *		Button Press on PA02
 *
 * Resources:
 *		[1] Atmel SAM L21 Xplained Pro User Guide
 *
 */ 


#include "init_ports.h"

void init_ports(void){
	
	/* Initialize LED 0 */
	// The LED can be activated by driving the connected I/O line to GND [1].
	PORT->Group[portb].OUTSET.reg = led; // PB10 High, which turns the LED off
	PORT->Group[portb].DIRSET.reg = led; // PB10 Output
	
	/* Initialize Button 0 */
	// There is no pull-up resistor connected to the generic user button [1].
	// Therefore, enable the internal pull-up in the SAML21 to use the built-in button [1].
	PORT->Group[porta].PINCFG[2].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN; // PA02 Input Enabled | Pull-Up/Down Enabled
	
	// Give a small micro-second delay for hardware to initiate pull-up. May not need to, but I'd rather be safe.
	delay(1000);
	PORT->Group[porta].OUTSET.reg = button; // 0 = Pull-Down, 1 = Pull-Up
	PORT->Group[porta].DIRCLR.reg = button; // Input	
	delay(1000);
}