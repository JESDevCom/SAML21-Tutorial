/*
 * ButtonPressWithDebounce.c
 *
 * Created: 11/30/2020 6:31:41 PM
 * Author : John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		This program using a button debounce algorithm to debounce
 *		SW0 on PA_02. The debounce algorithm uses Timer0 to create a
 *		4 [ms] interrupt so the algorithm can be serviced accurately..
 *		
 *		Ultimately, the user can toggle LED0 and PB_10 using the button
 *		on PA_02
 *
 * Lesson Notes:
 *		Requires knowledge about using:
 *			System timers and clocks,
 *			Reading from I/O, 
 *			Writing to I/O, and
 *			Algorithm practice for software based debouncing.
 *
 * Hardware Notes:
 *		(1) Internal 16MHz Clock used as the main clock source for CPU and timer.
 */ 

/***************************************************
					Header Files
***************************************************/
#include "main.h"

/***************************************************
					Global Variables
***************************************************/
volatile uint8_t int_timer = 0;



int main(void)
{
	/* Variable Declaration */
	static uint8_t time_srv = 0; // timer service flags for scheduling
	uint8_t scheduler = 0;
	uint32_t key = 0;
	uint32_t prev_key = 0;
	
   
    SystemInit();   /* Initialize the SAM system */
	init_ports();	/* Initialize ports on board */
	init_timer();   /* Initialize & setup timer0 */


    /**************************** MAIN WHILE LOOP *****************************/
    while (1) 
    {
		scheduler = time_srv^int_timer;
		
		// -------------------------------------------- serviced every: 4.6 [ms] as measured by Fluke 87V
		if (scheduler & 0x01){ 
			time_srv = time_srv ^ 0x01;
			//REG_PORT_OUTTGL1 = led; //toggle LED
			
			key = Debounce(); // Debounce Pin
			
			if(key != prev_key){ // stop duplicated entries when no key has changed
				if((key & button) == button){
					REG_PORT_OUTTGL1 = led;	// toggle LED0
				}
				prev_key = key;
			}
		}
		
		// --------------------------------------------- serviced every: 9.09 [ms] as measured by Fluke 87V
		if (scheduler & 0x02){ 
			time_srv = time_srv ^ 0x02;
			//REG_PORT_OUTTGL1 = led; //toggle LED
		}
    }// while
}// END MAIN
