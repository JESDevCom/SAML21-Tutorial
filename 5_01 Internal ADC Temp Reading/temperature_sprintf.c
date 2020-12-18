/*
 * temperature_sprintf.c
 *
 * Created: 12/17/2020 4:36:00 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		A collection of function used to do the job of sprintf(TEMPERATURE_BUFFER, "%3.2f", temp_float).
 *		Not using sprintf reduces code size by 26% as ~O0 compiler optimization.
 *
 * Resources
 *		[1] SAM L21 Family Data Sheet
 * 
 */ 
#include "temperature_sprintf.h"

void floattostring(float val){
	/* This function converts a float to a char string. This is used to avoid using the sprintf function
	 which uses up about 26% of Program Memory. We can use this method since we know the exact
	 length of our char buffer (BUFFER_TEMPERATURE) and how we want to format the digits inside
	 the buffer 
	 
	 BUF_TEMP_LEN:    9
	 FORMAT Example: ' 100.91 F`' or '-100.91 F`'
	 BITS:			  0123456789
	 	 
	 FUNCTION OUTPUT SAME AS: sprintf(BUFFER_TEMPERATURE, "%.2f F", val); // will need #include <stdlib.h>
	 */
	uint32_t sign_char = signbit2char(sign_bit(val));
	uint32_t val_frac  = (val - (int)val)*100;
	uint32_t val_int   = val;
	uint32_t len_int   = countIntLength(val_int);
	
	// Fractional Digit Conversion
	uint32_t remainder = (val_frac % 10);
	BUFFER_TEMPERATURE[6] = integer2char(remainder);				// 100th place
	val_frac = val_frac - remainder;
	BUFFER_TEMPERATURE[5] = integer2char(seperator(val_frac, 10));	// 10th  place
	BUFFER_TEMPERATURE[4] = PERIOD_CHAR;							// '.' char
	
	// Three digit integer temperature conversion
	if (len_int == 3){
		remainder = seperator(val_int, 1);
		BUFFER_TEMPERATURE[3] = integer2char(remainder); // LSD
		val_int = val_int - remainder;
		
		remainder = seperator(val_int, 10);
		BUFFER_TEMPERATURE[2] = integer2char(remainder);
		val_int = val_int - remainder;
		
		remainder = seperator(val_int, 10);
		BUFFER_TEMPERATURE[1] = integer2char(remainder); // MSD
		
		BUFFER_TEMPERATURE[0] = sign_char;
	}
	
	// Two digit integer temperature conversion
	else if(len_int == 2){
		remainder = seperator(val_int, 1);
		BUFFER_TEMPERATURE[3] = integer2char(remainder); // LSD
		val_int = val_int - remainder;
		
		remainder = seperator(val_int, 10);
		BUFFER_TEMPERATURE[2] = integer2char(remainder); // MSD
				
		BUFFER_TEMPERATURE[1] = sign_char;
		
		BUFFER_TEMPERATURE[0] = SPACE_CHAR; // Space Character
	}
	
	// One digit integer temperature conversion
	else{	
		BUFFER_TEMPERATURE[3] = integer2char(val_int); // LSD & MSB
				
		BUFFER_TEMPERATURE[2] = sign_char;
				
		BUFFER_TEMPERATURE[1] = SPACE_CHAR; // Space Character
				
		BUFFER_TEMPERATURE[0] = SPACE_CHAR; // Space Character
	}
}

char signbit2char(uint32_t val){
	/* This function returns the corresponding signed character for a positive or negative sign bit */
	char signum = 0x20; // pace character
	if (val == 1){
		signum = MINUS_CHAR;	// Negative character
	}
	return signum;
}
uint32_t sign_bit(float val){
	uint32_t bit = 0;
	if (val < 0){
		bit = 1;
	}
	return bit;
}

uint32_t countIntLength(uint32_t integer){
	/* Count the number of digits in an integer. */
	uint32_t count = 0;
	while(integer != 0){
		integer = integer / 10;
		++count;
	}	
	return count;
}

char integer2char (uint32_t integer){
	/* Return the ASCII character of an integer */
	return 0x30 + (uint8_t)integer;	
}

uint32_t seperator(uint32_t integer, uint32_t divisor){
	/* This function separates an integer and stops a nan issue where the numerator is 0 */
	uint32_t val = 0;
	
	if(integer != 0){
		val = (integer / divisor) % 10;
	}
	return val;
}