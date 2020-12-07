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
 */ 


#include "init_ports.h"

void init_ports(void){
	
	/* Initialize LED 0 */
	// The LED can be activated by driving the connected I/O line to GND [1].
	PORT->Group[portb].OUTSET.reg = led; // PB10 High, which turns the LED off
	PORT->Group[portb].DIRSET.reg = led; // PB10 Output
	
	
	delay(1000);
}