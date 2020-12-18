/*
 * CFile1.c
 *
 * Created: 10/19/2020 1:29:51 PM
 *  Author: John
 */

#include "sam.h"
#include "delay.h"

void delay(void) {
	
	for (volatile uint16_t x=0; x<65534; x++){
		__asm("nop");
	}
}