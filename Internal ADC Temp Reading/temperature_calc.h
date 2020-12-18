/*
 * temperature_calc.h
 *
 * Created: 12/17/2020 2:02:15 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 *
 * Description:
 *		Calculates the internal CPU temperature from temperature sensor inside the mcu.
 */ 


#ifndef TEMPERATURE_CALC_H_
#define TEMPERATURE_CALC_H_

/****************************************************************
						   Header Files
****************************************************************/
#include "sam.h"
#include "sys_calib.h"

/****************************************************************
						   Definitions
****************************************************************/


/****************************************************************
						Global Declarations
****************************************************************/


/****************************************************************
						   Function Prototypes
****************************************************************/
float calc_temp(uint16_t ADCin);




#endif /* TEMPERATURE_CALC_H_ */