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
							Receive  a Character
****************************************************************************
READING RULES: 
	Reading these bits will return the contents of the RX data register.

	The register should be read only when the RX Complete Flag bit in the
	"Interrupt Flag Status and Clear" Register (INTFLAG.RXC) is set.
	The status bits in STATUS should be read before reading the DATA register
	value in order to get any corresponding error.
---------------------------------------------------------------------------*/
char RX_char(void){
	while(SERCOM3->USART.INTFLAG.bit.RXC == 0); // wait till RX Complete
	return SERCOM3->USART.DATA.reg;				// (Pg. 545) [1]
}