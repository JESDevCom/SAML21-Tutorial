/*
 * uart_rx.c
 *
 * Created: 12/6/2020 6:13:12 PM
 * Author : John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 

#include "uart_rx.h"

/***************************************************************************
							Receive a Character
****************************************************************************
READING RULES: 
	Reading these bits will return the contents of the RX data register.

	The register should be read only when the RX Complete Flag bit in the
	"Interrupt Flag Status and Clear" Register (INTFLAG.RXC) is set.
	The status bits in STATUS should be read before reading the DATA register
	value in order to get any corresponding error.
---------------------------------------------------------------------------*/
char RX_char(void){
	
	if(SERCOM3->USART.INTFLAG.bit.RXC == 1){			// If Receivable Complete
		
		const char c = SERCOM3->USART.DATA.reg;				// Read RX Register (Pg. 545) [1]
		RX_string(c);
		RX_CHAR = c;
	}	
}


/***************************************************************************
							Receive a String
****************************************************************************
We add each character to BUFFER with a set length (BUF_LEN)
--------------------------------------------------------------------------*/
void RX_string(char c){
	
	// Reset buffer index to begin overwriting in a Circular Buffer manner.
	// We will overwrite all elements except the last element (ie., the Null character '\0')
	if (buff_index == (BUF_LEN-1)){
		buff_index = 0;
	}		 
	BUFFER[buff_index++] = RX_CHAR;
}
