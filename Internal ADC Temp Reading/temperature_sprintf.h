/*
 * temperature_sprintf.h
 *
 * Created: 12/17/2020 4:40:33 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef TEMPERATURE_SPRINTF_H_
#define TEMPERATURE_SPRINTF_H_

/****************************************************************
						   Header Files
****************************************************************/
#include "sam.h"
#include "sys_def.h"
#include "sys_ascii.h"


/****************************************************************
						  Definitions
****************************************************************/



/****************************************************************
					   Global Declaration
****************************************************************/
extern volatile char BUFFER_TEMPERATURE[BUF_TEMP_LEN];

/****************************************************************
				       Function Prototypes
****************************************************************/
void floattostring(float val);
uint32_t countIntLength(uint32_t integer);
uint32_t sign_bit(float val);
char signbit2char(uint32_t val);
char integer2char (uint32_t integer);
uint32_t seperator(uint32_t integer, uint32_t divisor);


#endif /* TEMPERATURE_SPRINTF_H_ */