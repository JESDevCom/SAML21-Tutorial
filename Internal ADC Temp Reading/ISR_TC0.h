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
#include "sys_def.h"

/****************************************************************
					External Variables Access
****************************************************************/
/* Timer Schedule:
	d7		d6		d5		d4		d3		d2		d1		d0
	524.8	262.1	131.1	65.5	32.8	16.4	8.2		4.1 [ms]
	
	d15		d14		d13		d12		d11		d10		d9		d8
	134.3	67.2	33.6	16.8	8.4		4.2		2.1		1.05 [s]
	
	d23		d22		d21		d20		d19		d18		d17		d16
	34393	17196	8598	4299	2149	1075	537.4	268.7 [s]
	    
*/
extern volatile uint32_t int_timer; // interrupt timer

/****************************************************************
						Function Prototype
****************************************************************/
// Do not define Handler_TC0 here because its already defined 
// in the sam.h header file.

#endif /* ISR_TC0_H_ */