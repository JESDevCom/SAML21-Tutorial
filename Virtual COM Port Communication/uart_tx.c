/*
 * uart_tx.c
 *
 * Created: 12/6/2020 6:12:54 PM
 * Author : John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 

#include "uart_tx.h"

/**************************************************************************
							Print a Character
***************************************************************************
WRITING RULES:
Writing these bits will write the Transmit Data Register.

This register should be written when the Data Register Empty "Interrupt
Flag Status and Clear" bit (INTFLAG.DRE) is set.
--------------------------------------------------------------------------*/
void TX_char(char c){
	while( SERCOM3->USART.INTFLAG.bit.DRE == 0); // wait till Data Register is empty and ready to be written too
	SERCOM3->USART.DATA.reg = c;
	while(SERCOM3->USART.INTFLAG.bit.TXC == 0);  // wait till Data Register is completely transmitted.
}


/***************************************************************************
							Print a String to Console
***************************************************************************/
void TX_string(char *string){
	while(*string){
		TX_char(*string++);
	}
	TX_char('\r');
	TX_char('\n');
}
