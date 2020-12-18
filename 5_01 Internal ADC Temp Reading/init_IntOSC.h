/*
 * init_IntOSC.h
 *
 * Created: 12/10/2020 2:01:21 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef INIT_INTOSC_H_
#define INIT_INTOSC_H_

/****************************************************************
						   Header Files
****************************************************************/
#include "sam.h"
#include "sys_def.h"
#include "init_GCLK.h"


/****************************************************************
						   Definitions
****************************************************************/


/****************************************************************
						   Definitions
****************************************************************/
extern volatile uint32_t F_CPU;


/****************************************************************
						Function Prototype
****************************************************************/
void init_IntOSC(uint32_t clk_hz, uint16_t divider);




#endif /* INIT_INTOSC_H_ */