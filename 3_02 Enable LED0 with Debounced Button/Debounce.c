/*
* Debounce.c
*
* Created: 11/30/2020 7:00:05 PM
* Author: John
* Board: Atmel SAML21 Xplained Pro
* MCU: Microchip SAML21J18B
* IDE: Atmel Studio v7
* ASF: No
*
* Description:
*		Implements a software debounce for all pins on Port A.
*
*       This debounce algorithm uses a 3 node state machine.
*			(0) Is the button pressed?
*			(1) Is the button still pressed after a short period of time?
*			(2) Has the button been released?
*
*		In this application we are using Debounce.c to debounce Pin PA_02 which is connected to push button 0.
*
* Notes:
*		A pressed button will be read as 0 on the microcontroller pin
*		A unpressed button will be read as 1 on the microcontroller pin 
*/

#include "Debounce.h"

uint32_t Debounce(void){
	
	// Variable Declaration
	uint8_t unpressed_key = false;		 // flag
	uint8_t still_pressed_key = false;   // flag
	uint8_t released_key = false;		 // flag
	uint32_t temp = (key_read & button); // read all port A and filter out only our button. button = 0x00000004
	uint32_t key = 0;
	uint8_t pinvalue[32];
	
	// Store these variables in Memory to hold values between function calls
	static uint8_t pinstate[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static uint8_t prev_pinvalue[32];
	
	// ============================================================= If button has been pressed? 
	// Here we are converting each bit from a 32 bit value
	// into its own array element.
	for (uint8_t i = 0; i<32; i++){
		
		if ((temp & (1<<i)) > 0) {
			pinvalue[i] = 1; // <------------------ set unpressed pin val to 1
		}
		else{
			pinvalue[i] = 0; // <------------------ set pressed pin val to 0
		}
	}// end for
	
	
	// ============================================================== Is the button still pressed?
	// Here we are setting flags depending on the pin value and 
	// its subsequent pin state.
	for(uint8_t i = 0; i < 32; i++){
		
		if (pinvalue[i] == 0){ // <----------------- Yes, button still pressed.
			switch(pinstate[i]){ //					 So we have two possibilities:
				case 1:
					still_pressed_key = true; //	(1) the button could still be pressed, or
					break;
				case 0:
					unpressed_key = true; //		(2) the button could have become unpressed (i.e., due to bounce)
					break;
				default:
					// default isn't necessary here, but
					// it's a good coding practice to have 
					// a default. Stops a crash :)
					break;
			}
		}
		else{
			if(pinstate[i] == 1){ // <-------------- No, button still unpressed
				still_pressed_key = true; 
				released_key = true;      
			}
		}
	}// end for 
	
	// =============================================================== Has the button been released?
	for(uint8_t i = 0; i < 32; i++){
		
		// ------------------------------------------ Save Current Pin Value
		if(unpressed_key == true){
			if(pinvalue[i] == 0){
				for(uint8_t j = 0; j<32; j++){
					prev_pinvalue[j] = pinvalue[j];
				}
				pinstate[i] = 1; // elevate the pin state for pressed button
			}
		}
		
		// ------------------------------------------ Check for Pin Value Change
		if(still_pressed_key == true){
			if(pinvalue[i] == 0){ //					Button hasn't been released, 
				pinstate[i] = 1; //						(1) so Keep pin state the same.
			}
			else{//										Button has been released, 
				if(prev_pinvalue[i] != pinvalue[i]){ // Then if it is different than the previous pin value:
					pinstate[i] = 2; //					(1) elevate the pin state for the released button, and
					released_key = true; //				(2) allow a positive button press to be returned.
				}
			}
		}
		
		// ------------------------------------------ Update pin status in variable:'key'
		if(released_key == true){
			switch(pinstate[i]){
				case 2:
					key |= (1<<i);		// Return Logic '1' for positively debounced pin
					pinstate[i] = 0;	// reset pin state
					break;
				default:
					key &= ~(1<<i);		// Otherwise, Logic '0'
					break;
			}
			prev_pinvalue[i] = pinvalue[i];
		}
	}// end for
	
	return key;
	
}// end Debounce


