/*
 * temperature_calc.c
 *
 * Created: 12/17/2020 1:58:51 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 *
 * Description:
 *		Calculates the internal CPU temperature from temperature sensor inside the mcu.
 *
 * Resources
 *		[1] SAM L21 Family Data Sheet
 */ 
#include "temperature_calc.h"

float calc_temp(uint16_t ADCin){
	/* This function calculates the CPUs internal temperature and returns a temp in Fahrenheit 
	   My Calibration Values from factory were:
			room int:	0x1e = 30
			room dec:	0x00 =  0
			hot int:	0x54 = 84
			hot dec:	0x09 =  9
			room 1V:	0x00 =  0
			hot  1V:	0x01 =  1
			room ADC:  0xA73 = 2675
			hot ADC:   0xC65 = 3173
	*/
	
	float ADCm	= ADCin;

	// Equation 3 - Course Temperature (Pg. 948)
	float tempC = tempR + ( (((ADCm*1) - (ADCr*INT1Vr)) * (tempH-tempR)) / ((ADCh*INT1Vh) - (ADCr*INT1Vr)) );
	
	// Equation 4 - Precise Internal 1[V] Reference (Pg. 949)
	float INT1Vm = INT1Vr + (((INT1Vh - INT1Vr) * (tempC - tempR)) / (tempH - tempR));
	
	// Equation 5 - Accurate temperature in Celsius
	float tempF = tempR + ( (((ADCm*INT1Vm) - (ADCr*INT1Vr)) * (tempH - tempR)) / ((ADCh*INT1Vh)-(ADCr*INT1Vr)) );
	
	// Convert from C to F
	tempF = ((tempF * (1.8)) + 32);
	
	return tempF;
}

