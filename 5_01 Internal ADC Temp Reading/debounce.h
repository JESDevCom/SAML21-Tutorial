/*
 * Debounce.h
 *
 * Created: 11/30/2020 7:03:49 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_

/***********************************************
				Header Files
***********************************************/
#include "sam.h"

/**********************************************
		        Definitions
**********************************************/
#define true				1
#define false				0
#define key_read			REG_PORT_IN0
#define button				PORT_PA02

/**********************************************
				Function Prototypes
**********************************************/
uint32_t Debounce(void);				


#endif /* DEBOUNCE_H_ */