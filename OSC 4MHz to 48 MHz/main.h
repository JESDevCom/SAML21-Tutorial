/*
 * File: main.h
 * Project: Changing CPU Clock Frequency
 * Created: 10/19/2020 1:42:16 PM
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
#include "init_timer0.h"
#include "ISR_TC0.h"
#include "init_MCLK.h"
#include "cpu_freq_sel.h"

/****************************************************************
						   Definitions
****************************************************************/
#define led				PORT_PB10
#define porta			0
#define portb			1
#define ledset()		PORT->Group[portb].OUTSET.reg = led;
#define ledclr()		PORT->Group[portb].OUTCLR.reg = led;
#define ledtgl()		PORT->Group[portb].OUTTGL.reg = led;
#define ledon()			PORT->Group[portb].DIRSET.reg = led;
#define ledoff()		PORT->Group[portb].DIRCLR.reg = led;

#endif /* MAIN_H_ */