/*
 * ISR_SERCOM3.c
 *
 * Created: 12/6/2020 6:21:13 PM
 * Author : John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		This function contains the interrupt vector for SERCOM3 USART.
 *		This vector is enabled in init_uart.c
 *		Individual flags that call this vector are enabled in init_uart.c
 * 
 * Function:
 *		Entering any key within the terminal 
 *
 * Resources:
 *		[1] SAM L21 Family Data Sheet
 */ 

#include "ISR_SERCOM3.h"

/***************************************************************** (Pg. 542)
						SERCOM3 Interrupt Vector
**************************************************************************/
void SERCOM3_Handler(){
	
	/*
	// -------------------------------------------------------- ERROR FLAG
	// This bit is set when any error is detected.
	if(SERCOM3->USART.INTFLAG.bit.ERROR == 1){
		SERCOM3->USART.INTFLAG.bit.ERROR = 1; // Write 1 to clear
	}
	
	// -------------------------------------------------------- RXBRK FLAG
	// This bit is set when auto-baud is enabled and a break 
	// character is received. Occurs when data line is held at
	// logic 0 for too long. 
	if(SERCOM3->USART.INTFLAG.bit.ERROR == 1){
		SERCOM3->USART.INTFLAG.bit.ERROR = 1; // Write 1 to clear
	}
	
	// -------------------------------------------------------- CTSIC FLAG
	// This flag is set when a change is detected on the CTS
	// pin.
	if(SERCOM3->USART.INTFLAG.bit.CTSIC == 1){
		SERCOM3->USART.INTFLAG.bit.CTSIC = 1; // Write 1 to clear
	}
	
	// -------------------------------------------------------- RXS FLAG
	// This flag is set when a start condition is detected on 
	// the RxD line and a start of frame detection is enabled.
	if(SERCOM3->USART.INTFLAG.bit.RXS == 1){
		SERCOM3->USART.INTFLAG.bit.RXS = 1; // Write 1 to clear
	}
	*/
	
	// -------------------------------------------------------- RXC FLAG
	// This flag is set when there are unread data in DATA reg.
	// Flag cleared by reading data from DATA.
	if(SERCOM3->USART.INTFLAG.bit.RXC == 1){
		RX_char();
	}
	
	/*
	// -------------------------------------------------------- TXC FLAG
	// This flag is set when the entire frame has been sent and
	// there is no new data in DATA.
	if(SERCOM3->USART.INTFLAG.bit.TXC == 1){
		SERCOM3->USART.INTFLAG.bit.TXC = 1; // Write  1 to Clear
	}
	
	// -------------------------------------------------------- DRE FLAG
	// This flag is set when DATA is empty and ready to be
	// written.
	// Flag cleared by writing new data to DATA.
	if(SERCOM3->USART.INTFLAG.bit.DRE == 1){
	}
	*/
}