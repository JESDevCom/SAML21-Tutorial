/*
 * Blinky_Timer.c
 *
 * Created: 10/16/2020 4:01:41 PM
 * Author : John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Blink an LED using a timer with frequency control.
 *
 *
 * Resources:
 *		SAML21 Datasheet
 *
 *		Output Frequency Formula:
 *			Freq = CPU_freq / ([#bit Timer] * [CPU_DIV] * [GCLK0_DIV] * [CC0])
 *
 *			Example: (**Count Up**)
 *			Freq = 16MHz / ([8] * [1] * [1024] * [180]) = 10.85 [Hz]   <--- Checked Calc with Fluke 87V
 *
 * Clock Architecture:
 *		16MHz CPU --> MCLK (IN) --> |DIV: 1| --> MCLK (OUT) --> GCLK0 (IN) --> |DIV: 1| --> GCLK0 (OUT) --> TC0 (IN) --> |DIV: 1024|
 *
 * Timer Architecture:
 *		For the purposes of this problem, we will be using a 32-bit timer in 8-bit mode.
 *		We will be implementing compare match. Other words: an accurate frequency generator.
 *		The output at the pin will be a 50% Duty Cycle.
 */ 


#include "main.h"



int main(void)
{
    /* Initialize the SAM system */
    SystemInit();

	/* Initialize Port */
	ledclr();
	REG_PORT_DIRSET1 = led; //Set as Output
	delay();
	//REG_PORT_OUTCLR1 = led; //LED ON
	//delay();
	REG_PORT_OUTSET1 = led; //LED OFF
	//delay();


	/* Enable TC0 in Interrupt Vector */
	NVIC_EnableIRQ( TC0_IRQn );

	
	/* Enable Timer0 Access to Master Clock (MCLK) Peripheral Bus (Pg. 146) */
	MCLK->APBAMASK.bit.GCLK_   = 1; // (Pg. 156) General CLK should already be enabled by default, but set anyways
	MCLK->APBAMASK.bit.MCLK_   = 1; // (Pg. 156) Master  CLK should already be enabled by default, but set anyway
	MCLK->APBCMASK.bit.TCC0_   = 1; // (Pg. 159) Timer Counter 0
	MCLK->APBCMASK.bit.TC0_    = 1; // (Pg. 159) Timer 0
	MCLK->APBDMASK.bit.EVSYS_  = 0; // (Pg. 161) Event System; Enable for External Triggering of Counter 									

	
	/* Enable General Clock 0 with OSC16M*/
	REG_GCLK_GENCTRL0 |= (GCLK_GENCTRL_DIV(1) | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSC16M);	// Enable Gen 0 (Pg. 135) | Gen0 Source: OSC 16M (Pg. 135)
	while(GCLK->SYNCBUSY.bit.GENCTRL0 == 1){}							// Wait for synchronization	(Pg. 133)
	
	/* Peripheral Channel Control (Pg. 137) */
	REG_GCLK_PCHCTRL27 |=  (GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_GEN_GCLK0);	// Enable Channel | Connect TC0 to GCLK0
							 

	/* Timer 0 CTRLA Settings */
	// (Pg. 641) Clock Divisions: 1024, Counter Size: 16-bits
	REG_TC0_CTRLA |= (TC_CTRLA_PRESCALER_DIV1024 | TC_CTRLA_MODE_COUNT8 | TC_CTRLA_RUNSTDBY); 
	while( TC0->COUNT8.CTRLA.bit.PRESCSYNC == 1){}
		
	/* Timer0 CTRLB Settings */
	TC0->COUNT8.CTRLBSET.bit.DIR = 1;					// (Pg. 645) Count Up
	
	/* Timer 0 Waveform Settings */
	TC0->COUNT8.WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;		// (Pg. 651) Mode: Freq Match, Top Value = CC0, Toggle Output.				
					
	/* Timer 0 Compare Value */
	TC0->COUNT8.CC[0].reg = 180;						// ADJUST HERE
	while( TC0->COUNT8.SYNCBUSY.bit.CC0 == 1 ){}		// Wait for CC0 to be set

	// Enable Interrupt Vectors
	REG_TC0_INTENSET |= (TC_INTENSET_ERR | TC_INTENSET_OVF | TC_INTENSET_MC0); 
	
	// Start Timer 0
	TC0->COUNT8.CTRLA.bit.ENABLE = 1;					//REG_TC0_CTRLA |= TC_CTRLA_ENABLE;
	while( TC0->COUNT8.SYNCBUSY.bit.ENABLE == 1){}		// Wait for TC0 to be enabled



    while (1) {
		
    }
}


// =====================================================================
//                     Interrupt Service: Timer 0
// =====================================================================
void TC0_Handler(void){

	/* =========== Service Error Flag =========== */
	if ( TC0->COUNT8.INTFLAG.bit.OVF == 1 ){
		__asm("nop");
	}
	
	/* =========== Service Error Flag =========== */
	if ( TC0->COUNT8.INTFLAG.bit.ERR == 1 ){
		__asm("nop");
	}
	
	/* ========= Service Compare Flag =========== */
	if ( TC0->COUNT8.INTFLAG.bit.MC0 == 1 ){
		TC0->COUNT8.INTFLAG.reg |= TC_INTFLAG_MC0; // Clear TC0 MC0 Interrupt Flag Register
		REG_PORT_OUTTGL1 = led;
	}
	
}
