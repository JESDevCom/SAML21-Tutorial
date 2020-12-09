/*
 * init_timer0.h
 *
 * Created: 12/6/2020 7:12:55 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef INIT_TIMER0_H_
#define INIT_TIMER0_H_

/****************************************************************
						   Header Files
****************************************************************/
#include "sam.h"

/****************************************************************
						   Function Prototype
****************************************************************/
void init_timer0(void);
void stop_timer0(void);
void start_timer0(void);

#endif /* INIT_TIMER0_H_ */