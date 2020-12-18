/*
 * ISR_ADC.c
 *
 * Created: 12/12/2020 6:48:53 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Resources
 *		[1] SAM L21 Family Data Sheet
 */ 

#include "ISR_ADC.h"

void ADC_Handler(void){
	if(ADC->INTFLAG.bit.RESRDY == 1){
		ADCx = adc_read();
	}
}
