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
						   Definitions
****************************************************************/
#define led				PORT_PB10
#define porta			0
#define portb			1

/****************************************************************
						Function Prototype
****************************************************************/
// Do not define Handler_TC0 here because its already defined 
// in the sam.h header file.

#endif /* ISR_TC0_H_ */