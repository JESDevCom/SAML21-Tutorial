/*
 * delay.c
 *
 * Created: 11/30/2020 6:34:19 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 *
 * Description:
 *		Software based delay, tuned for a 16 MHz processor. Receives a 16 unsigned value.
 * 
 * 
 */ 

#include "delay.h"

void delay(uint16_t value) {
	
	__asm("nop");
	
	for (volatile uint16_t x=0; x < value; x++){
		__asm("nop");
	}
	
	__asm("nop");
}