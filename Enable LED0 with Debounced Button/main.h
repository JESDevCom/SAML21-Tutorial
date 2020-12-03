/*
 * main.h
 *
 * Created: 11/30/2020 6:35:31 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef MAIN_H_
#define MAIN_H_

/****************************************************************
						   Header Files
****************************************************************/
#include "sam.h"
#include "delay.h"
#include "init_ports.h"
#include "init_timer.h"
#include "Debounce.h"
#include "ISR_TC0.h"

/****************************************************************
						   Definitions
****************************************************************/
#define led					PORT_PB10
#define button				PORT_PA02
#define key_read			REG_PORT_IN0
#define porta				0
#define portb				1
#define ledset()			PORT->Group[portb].OUTSET.reg = led;
#define ledclr()			PORT->Group[portb].OUTCLR.reg = led;
#define ledtgl()			PORT->Group[portb].OUTTGL.reg = led;
#define ledon()				PORT->Group[portb].DIRSET.reg = led;
#define ledoff()			PORT->Group[portb].DIRCLR.reg = led;

/****************************************************************
					External Variables Access
****************************************************************/
/* Timer Schedule:
	d7	d6	d5	d4	d3	d2	d1	d0
	512	256	128	64	32	16	8	4 [ms]    
*/
extern volatile uint8_t int_timer;


#endif /* MAIN_H_ */