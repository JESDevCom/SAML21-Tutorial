/*
 * init_clock.h
 *
 * Created: 12/6/2020 7:24:41 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef INIT_CLOCK_H_
#define INIT_CLOCK_H_

/****************************************************************
						   Header Files
****************************************************************/
#include "sam.h"
#include "sys_def.h"
#include "init_32KHz.h"
#include "init_GCLK.h"
#include "init_IntOSC.h"
#include "init_DFLL48M.h"
#include "delay.h"


/****************************************************************
						   Definitions
****************************************************************/


/****************************************************************
				       Function Prototypes
****************************************************************/
void cpu_freq_sel(uint32_t clk_hz);
void PD_sel(uint32_t clk_src);


#endif /* INIT_CLOCK_H_ */