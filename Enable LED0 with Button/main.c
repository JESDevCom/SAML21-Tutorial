/*
 * GccApplication1.c
 *
 * Created: 11/13/2020 2:36:15 PM
 * Author : John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Toggle an LED on PB10 with a button press on PA02.
 *
 * Resources:
 *		[1] Atmel SAM L21 Xplained Pro User Guide.
 *
 * Notes:
 *		16MHz Internal OSC is the clock source.
 *		Internal pull-up must be used for built-in button PA02. -_- come on Atmel, a 10k resister is cheap. 
 */ 


#include "main.h"


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
	/* Initialize LED 0 */
	// The LED can be activated by driving the connected I/O line to GND [1].
	PORT->Group[portb].OUTSET.reg = led; // PB10 High, which turns the LED off.
	PORT->Group[portb].DIRSET.reg = led; // PB10 Output
	
	/* Initialize Button 0 */
	// There is no pull-up resistor connected to the generic user button [1].
	// Therefore, enable the internal pull-up in the SAML21 to use the built-in button [1].
	PORT->Group[porta].PINCFG[2].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN; //PA02 Input Enabled | Pull-Up/Down Enabled
	
	// Give a small micro-second delay for hardware to initiate pull-up. May not need to, but I'd rather be safe.
	delay(2000);
	PORT->Group[porta].OUTSET.reg = button; // 0 = Pull-Down, 1 = Pull-Up
	PORT->Group[porta].DIRCLR.reg = button; // Input
	


    while (1) 
    {
		// if button pressed
		if((REG_PORT_IN0 & button) != 0){
			PORT->Group[portb].OUTSET.reg = led; // LED ON
		}
		// else button not pressed
		else{
			PORT->Group[portb].OUTCLR.reg = led; // LED OFF
		}
    }
}
