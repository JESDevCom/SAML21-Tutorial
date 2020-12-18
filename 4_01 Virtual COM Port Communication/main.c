/*
 * Project: Print to Console via Virtual COM Port
 * Created: 12/3/2020 4:23:27 PM
 * Author : John
 * Completion Time: Approx. 16 hours
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Send and Receive a message over the virtual COM Port (UART)
 *		to the console in Atmel Studio or PuTTy. Sending a lowercase
 *		letter 'o' will toggle the LED on the Xplained board.
 *
 * Lesson Notes:
 *		Requires knowledge about using:
 *			USART - (Universal Synchronous and Asynchronous Receiver and Transmitter)
 *			Interrupts
 *			COM Ports & Terminals
 *			ASCII
 *			Clock Frequencies
 *
 * Hardware Notes:
 *		(1) Internal 4MHz Clock used as the main clock source for CPU and timer.
 *		(2) UART SETTINGS: 9600 baud, no parity, 1 stop bit.
 *		(3) "Debug USB" port on Xplained Pro used. COM3 for me.
 */ 

/***************************************************
					Header Files
***************************************************/
#include "main.h"

volatile uint8_t buff_index = 0;
volatile char BUFFER[5] = {'\0'};
volatile char RX_CHAR = NULL_CHAR;

char message[] = "Press Lowercase 'o' On Your Keyboard to Toggle the LED";


int main(void)
{	
    SystemInit();						/* Initialize the SAM system */
	init_ports();						/* Initialize ports on board */
	init_uart(uart_bps_9600);			/* Initialize UART0 on board */
	TX_string(message);					/* Print Message to Console  */
				

    while (1) 
    {

		if(RX_CHAR == o_CHAR){ // if the letter lowercase 'o' is entered, toggle led
			PORT->Group[1].OUTTGL.reg = PORT_PB10;
			TX_string("LED Toggled");	// send status back
			RX_CHAR = NULL_CHAR;			// clear RX_CHAR variable
		}
    }
}
