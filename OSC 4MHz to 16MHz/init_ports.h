/*
 * init_ports.h
 *
 * Created: 11/30/2020 6:38:11 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef INIT_PORTS_H_
#define INIT_PORTS_H_

/****************************************************************
						   Header Files
****************************************************************/
#include "sam.h"
#include "delay.h"

/****************************************************************
						   Definitions
****************************************************************/
#define led					PORT_PB10
#define porta				0
#define portb				1
#define ledset()			PORT->Group[portb].OUTSET.reg = led;
#define ledclr()			PORT->Group[portb].OUTCLR.reg = led;
#define ledtgl()			PORT->Group[portb].OUTTGL.reg = led;
#define ledon()				PORT->Group[portb].DIRSET.reg = led;
#define ledoff()			PORT->Group[portb].DIRCLR.reg = led;

#define PMUX_A				0
#define PMUX_B				1
#define PMUX_C				2
#define PMUX_D				3
#define PMUX_E				4
#define PMUX_F				5
#define PMUX_G				6
#define PMUX_H				7
#define PMUX_I				8

/****************************************************************
					   Function Prototypes
****************************************************************/
void init_ports(void);


#endif /* INIT_PORTS_H_ */