/*
 * init_timer.h
 *
 * Created: 11/30/2020 6:48:49 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef INIT_TIMER_H_
#define INIT_TIMER_H_

/****************************************************************
						   Header Files
****************************************************************/
//#include <stdio.h>
//#include <math.h> // need for round function

#include "sam.h"
#include "sys_def.h"
#include "init_GCLK.h"
#include "ISR_TC0.h"

/****************************************************************
						   Definitions
****************************************************************/
#define PORT_DELAY	2


/****************************************************************
						   Definitions
****************************************************************/
extern volatile uint32_t F_CPU;

/****************************************************************
						Function Prototype
****************************************************************/
void init_timer(uint32_t delay_freq);


#endif /* INIT_TIMER_H_ */