/*
 * init_32KHz.h
 *
 * Created: 12/8/2020 8:34:28 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef INIT_32KHZ_H_
#define INIT_32KHZ_H_

/****************************************************************
						   Header Files
****************************************************************/
#include "sam.h"
#include "sys_def.h"
#include "sys_calib.h"


/****************************************************************
						  Definitions
****************************************************************/


/****************************************************************
					   Function Prototypes 
****************************************************************/
void init_32KHz(uint32_t clk_src);
void stop_32KHz(uint32_t clk_src);



#endif /* INIT_32KHZ_H_ */