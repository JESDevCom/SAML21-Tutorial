/*
 * delay.c
 *
 * Created: 11/13/2020 2:40:55 PM
 * Author: John
 *
 * Description:
 *		Software based delay, tuned for a 16 MHz processor. Receives a 16 unsigned value.
 * 
 * 
 */ 

#include "sam.h"
#include "delay.h"

void delay(uint16_t value) {
	
	__asm("nop");
	
	for (volatile uint16_t x=0; x < value; x++){
		__asm("nop");
	}
	
	__asm("nop");
}
