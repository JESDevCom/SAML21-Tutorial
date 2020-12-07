/*
 * init_ports.c
 *
 * Created: 11/30/2020 6:36:28 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		This file initializes ports on the SAML21J18B Xplained Pro Development board.
 *		PB10 | LED0
 *		PA22 | SERCOM3 PAD[0] UART TXD (SAM L21 TX Line)
 *		PA23 | SERCOM3 PAD[0] UART RXD (SAM L21 TX Line)
 *
 * Resources:
 *		[1] Atmel SAM L21 Xplained Pro User Guide
 *		[2]	SAM L21 Family Data Sheet
 */ 


#include "init_ports.h"

void init_ports(void){
	
	/* Initialize LED 0 */
	// The LED can be activated by driving the connected I/O line to GND [1].
	PORT->Group[portb].OUTSET.reg = led; // PB10 High, which turns the LED off
	PORT->Group[portb].DIRSET.reg = led; // PB10 Output
	
	/* Initialize USART 0 TX */
	PORT->Group[porta].DIRSET.reg = VCOM_TX;		// TX as Output
	//PORT->Group[porta].OUTSET.reg = VCOM_TX;		// Set Idle State High
	PORT->Group[porta].PINCFG[22].bit.INEN = 1;		// Enable Input (Required for UART)
	PORT->Group[porta].PINCFG[22].bit.PMUXEN = 1;	// Enable Peripheral MUX
	PORT->Group[porta].PMUX[22>>1].bit.PMUXE = 2;	// Set the PMUX bit (if pin is even, use PMUXE) (Pg. 486, 28) [2]
	
	/* Initialize USART 1 RX */
	PORT->Group[porta].DIRCLR.reg = VCOM_RX;		// RX as Input
	PORT->Group[porta].PINCFG[23].bit.INEN = 1;		// Enable Input
	PORT->Group[porta].PINCFG[23].bit.PULLEN = 0;	// Disable Pull-Up/Down
	PORT->Group[porta].PINCFG[23].bit.PMUXEN = 1;	// Enable Peripheral MUX
	PORT->Group[porta].PMUX[23>>1].bit.PMUXO = 2;	// Set the PMUX bit (if pin is odd, use PMUXO) (Pg. 486, 28) [2]
	
	
	delay(1000);
}