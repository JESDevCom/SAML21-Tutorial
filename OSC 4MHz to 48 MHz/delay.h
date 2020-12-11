/*
 * delay.h
 *
 * Created: 11/30/2020 6:35:03 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef DELAY_H_
#define DELAY_H_

/****************************************************************
						   Header Files
****************************************************************/
#include "sam.h"


/****************************************************************
				       Function Prototypes
****************************************************************/
void delayus_16bit(uint16_t CPU_Hz, uint16_t delay_us);
void delayus_32bit(uint32_t CPU_Hz, uint32_t delay_us);
void delayms_16bit(uint16_t CPU_Hz, uint16_t delay_ms);
void delayms_32bit(uint32_t CPU_Hz, uint32_t delay_ms);
void cycle_delay(uint32_t number_of_cycles);

#endif /* DELAY_H_ */