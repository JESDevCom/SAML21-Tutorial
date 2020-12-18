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
#include <string.h>
#include "sam.h"
#include "delay.h"
#include "init_ports.h"
#include "init_uart.h"
#include "uart_rx.h"
#include "uart_tx.h"
#include "ISR_SERCOM3.h"


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

#define NULL_CHAR			0x00
#define o_CHAR				0x6F

/****************************************************************
					External Variables Access
****************************************************************/
extern volatile uint8_t		buff_index;
extern volatile char		BUFFER[5];
extern volatile char		RX_CHAR;


#endif /* MAIN_H_ */