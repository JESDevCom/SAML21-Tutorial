/*
 * init_adc.h
 *
 * Created: 12/12/2020 1:12:14 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef INIT_ADC_H_
#define INIT_ADC_H_

/******************************************************
				   Header Files
******************************************************/
#include "sam.h"
#include "sys_def.h"


/******************************************************
				   Definitions
******************************************************/
#define NVMCAL_BIASREFBUF_MASK			0x00000007
#define NVMCAL_BIASREFBUF_POS			0
#define NVMCAL_BIASCOMP_MASK			0x00000038
#define NVMCAL_BIASCOMP_POS				3

#define CAL_BIASREFBUF	(*((uint32_t*)NVMCTRL_OTP5) & NVMCAL_BIASREFBUF_MASK) >> NVMCAL_BIASREFBUF_POS
#define CAL_BIASCOMP	(*((uint32_t*)NVMCTRL_OTP5) & NVMCAL_BIASCOMP_MASK)   >> NVMCAL_BIASCOMP_POS


/******************************************************
				 Global Variables
******************************************************/



/******************************************************
				Function Prototypes
******************************************************/
void init_adc(void);
uint16_t adc_read(void);
void start_adc_conversion(void);
void disable_adc(void);

#endif /* INIT_ADC_H_ */