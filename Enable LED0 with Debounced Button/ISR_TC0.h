/*
 * ISR_TC0.h
 *
 * Created: 12/2/2020 7:34:37 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef ISR_TC0_H_
#define ISR_TC0_H_


/****************************************************************
						   Header Files
****************************************************************/
#include "sam.h"

/****************************************************************
					External Variables Access
****************************************************************/
/* Timer Schedule:
	d7	d6	d5	d4	d3	d2	d1	d0
	512	256	128	64	32	16	8	4 [ms]    
*/
extern volatile uint8_t int_timer; // interrupt timer

/****************************************************************
						Function Prototype
****************************************************************/
// Do not define Handler_TC0 here because its already defined 
// in the sam.h header file.

#endif /* ISR_TC0_H_ */