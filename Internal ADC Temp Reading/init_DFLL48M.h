/*
 * init_DFLL48M.h
 *
 * Created: 12/10/2020 2:59:08 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef INIT_DFLL48M_H_
#define INIT_DFLL48M_H_

/****************************************************************
						  Header Files
****************************************************************/
#include "sam.h"
#include "sys_def.h"
#include "init_GCLK.h"
#include "init_32KHz.h"

/****************************************************************
						  Definitions
****************************************************************/


/****************************************************************
				      Function Prototypes
****************************************************************/
void init_DFLL48M(uint32_t ref_clk);



#endif /* INIT_DFLL48M_H_ */