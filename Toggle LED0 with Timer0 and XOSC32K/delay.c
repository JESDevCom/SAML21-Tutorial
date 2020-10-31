/*
 * delay.c
 *
 * Created: 10/24/2020 5:50:13 PM
 *  Author: John
 */ 
#include "sam.h"
#include "delay.h"

void delay(void) {
	
	for (volatile uint16_t x=0; x<65534; x++){
		__asm("nop");
	}
}