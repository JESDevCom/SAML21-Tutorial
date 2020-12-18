/*
 * init_uart.c
 *
 * Created: 12/5/2020 3:57:04 PM
 * Author : John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		This function initializes the SAML21 to communicate with the 
 *		virtual COM port. The embedded debugger on the Xplained board
 *		acts as a Virtual Com Port gateway by using one of the ATSAML21J18B
 *		UARTs. This allows you to send or receive message over this COM ports
 *		to a terminal on the host PC over the USB connection used to
 *		program the microcontroller.
 *
 * Hardware Settings:
 *		(1) CPU Frequency = 4MHz
 *
 * UART Settings:
 *		(1) Baud Rate: 9600
 *		(2) Asynchronous Communication
 *		(3) One stop bit
 *		(4) No parity
 *		(5) Data Packet Width: 8-bits
 *
 * Target Terminal:
 *		(1) You can use either the terminal in Atmel Studio:
 *			Tools -> Data Visualizer -> Configuration -> External Connection -> Serial Port
 *		(2) Or you can use a 3rd party terminal like Putty.
 *			Both worked for me.
 *
 * Resources
 *		[1] SAM L21 Family Data Sheet
 *		[2] Atmel SAM L21 Xplained Pro User Guide
 */ 

#include "init_uart.h"


void init_uart(uint64_t baud){
	
	// Calculate Baud Rate
	uint64_t baud_rate = (uint64_t)65536 * (F_CPU - 16 * baud) / F_CPU;
	
	// Enable SERCOM3 Access To Master Clock (Pg. 146) [1]
	MCLK->APBCMASK.bit.SERCOM3_ = 1;
	
	// Enable SERCOM3 Access to Peripheral Channel Control (Pg. 137) [1]
	GCLK->PCHCTRL[SERCOM3_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN;	// Generator 0 Selected | Channel Enable
	while((GCLK->PCHCTRL[SERCOM3_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN) == 0);				// Wait till complete
	

	// -------------------- Settings for SERCOM3 ---------------------- (Pg. 531) [1]
	// Mode: Asynchronous, One Stop bit, No parity
	SERCOM3->USART.CTRLA.reg = (SERCOM_USART_CTRLA_DORD		|	/* LSB is transmitted first			*/
								SERCOM_USART_CTRLA_MODE(1)	|	/* USART with internal clock		*/		
								SERCOM_USART_CTRLA_RXPO(1)	|	/* Receive  Data on SERCOM Pad 0	*/
								SERCOM_USART_CTRLA_TXPO(0)	);	/* Transmit Data on SERCOM Pad 1	*/
								
	SERCOM3->USART.CTRLB.reg = (SERCOM_USART_CTRLB_RXEN		|
								SERCOM_USART_CTRLB_TXEN		|
								SERCOM_USART_CTRLB_CHSIZE(0));	/* Char Size Represented by 8 bits	*/
								
	while(SERCOM0->USART.SYNCBUSY.bit.CTRLB == 1);	// wait till ready
								
	SERCOM3->USART.BAUD.reg = (uint16_t)baud_rate;	// Set Baud Rate
	
	SERCOM3->USART.CTRLA.bit.ENABLE = 1;			// Enable SERCOM3 UART
	while(SERCOM3->USART.SYNCBUSY.bit.ENABLE == 1); // wait till ready
	
	// ------------------- Interrupts for SERCOM3 ---------------------- (Pg. 540) [1]
	NVIC_EnableIRQ(SERCOM3_IRQn);			// Enable SERCOM3 Interrupt Vector
	SERCOM3->USART.INTENSET.bit.RXC = 1;	// Enable Interrupt for Receive Complete
}








