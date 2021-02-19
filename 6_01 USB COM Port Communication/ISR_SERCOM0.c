/*
 * ISR_UART0.c
 *
 * Created: 1/1/2021 5:13:57 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description: 
 *		Interrupt vector for SERCOM0.
 *
 * Resources:
 *		[1] SAM L21 Family Data Sheet
 *		[2] Atmel SAM L21 Xplained Pro User Guide
 *
 */ 

#include "ISR_SERCOM0.h"

void SERCOM0_Handler(void){
	
	// -------------------------------------------------------- RXC FLAG
	// This flag is set when there are unread data in DATA reg.
	// Flag cleared by reading data from DATA.
	if(SERCOM0->USART.INTFLAG.bit.RXC){
		
		uint8_t data_received = SERCOM0->USART.DATA.reg;
		uint32_t wr_index = (sercom0_rx_buffer.write + 1) % SERCOM0_BUF_LEN;
				
		// ---------------------------------------------------- BUFOVF (Buffer Overflow)
		// Flag set when a buffer overflow condition is detected.
		// A buffer overflow occurs when the RX buffer is full,
		// there is a new character waiting in the RX shift reg
		//  and a new start bit is detected.
		if(SERCOM0->USART.STATUS.bit.BUFOVF){
			USB_CDC_SERIALSTATE_(SET, bOverRun);
		}
		// ----------------------------------------------------- FERR (Frame Error)
		// Flag set if the received character had a frame error.
		// (ie when the first stop bit is zero).
		// Flag cleared by writing a '1' or disabling receiver.
		
		if(SERCOM0->USART.STATUS.bit.FERR){
			USB_CDC_SERIALSTATE_(SET, bFraming);
		}
		// ----------------------------------------------------- PERR (Parity Error)
		// Flag set if parity checking is enabled (CTRLA.FORM is
		// 0x1, 0x5) and a parity error is detected.
		// Flag cleared by writing a '1' or disabling receiver.
		if(SERCOM0->USART.STATUS.bit.PERR){
			USB_CDC_SERIALSTATE_(SET, bParity);
		}
		
		if(wr_index == sercom0_rx_buffer.read){ //Data read from port slower than received
			USB_CDC_SERIALSTATE_(SET, bOverRun);
		}
		else{
			sercom0_rx_buffer.data[sercom0_rx_buffer.write] = data_received;
			sercom0_rx_buffer.write = wr_index;
		}
	}
	// -------------------------------------------------------- DRE FLAG
	// This flag is set when the data register is empty
	// Flag cleared by writing new data to DATA
	if(SERCOM0->USART.INTFLAG.bit.DRE){
		if(sercom0_tx_buffer.read == sercom0_tx_buffer.write){
			SERCOM0->USART.INTENCLR.bit.DRE = 1; // Enable Interrupt
		}
		else{
			SERCOM0->USART.DATA.reg = sercom0_tx_buffer.data[sercom0_tx_buffer.read];
			sercom0_tx_buffer.read = (sercom0_tx_buffer.read + 1) % SERCOM0_BUF_LEN;
		}
	}
}