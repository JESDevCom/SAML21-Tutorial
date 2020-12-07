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
#define button				PORT_PA02
#define key_read			REG_PORT_IN0
#define VCOM_TX				PORT_PA22
#define VCOM_RX				PORT_PA23
#define porta				0
#define portb				1
#define ledset()			PORT->Group[portb].OUTSET.reg = led;
#define ledclr()			PORT->Group[portb].OUTCLR.reg = led;
#define ledtgl()			PORT->Group[portb].OUTTGL.reg = led;
#define ledon()				PORT->Group[portb].DIRSET.reg = led;
#define ledoff()			PORT->Group[portb].DIRCLR.reg = led;

/****************************************************************
					   Function Prototypes
****************************************************************/
void init_ports(void);


#endif /* INIT_PORTS_H_ */