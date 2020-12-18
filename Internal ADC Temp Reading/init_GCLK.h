/*
 * init_GCLK.h
 *
 * Created: 12/10/2020 1:21:46 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef INIT_GCLK_H_
#define INIT_GCLK_H_

/****************************************************************
						  Header Files
****************************************************************/
#include "sam.h"
#include "sys_def.h"


/****************************************************************
						  Definitions
****************************************************************/


/****************************************************************
				      Function Prototypes
****************************************************************/
void init_GCLK(uint32_t gen_num, uint32_t clk_src, uint16_t divider);
void stop_GCLK(uint32_t gen_num);
void switch_cpu_source(uint32_t clk_src);


#endif /* INIT_GCLK_H_ */