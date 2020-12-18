/*
 * init_timer0.c
 *
 * Created: 12/6/2020 7:13:13 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Initializes timer 0 for a 4 [ms] interrupt
 *
 * Output Frequency Formula:
 *			Freq = CPU_freq / ([Port Delay] * [CPU_DIV] * [GCLK0_DIV] * [CC0])
 *
 *			Example: (**Count Up**)
 *			Freq = 4MHz / ([2] * [1] * [1024] * [180]) = 10.85 [Hz]   <--- Checked Calc with Fluke 87V
 *
 * Clock Architecture:
 *		4MHz CPU --> MCLK (IN) --> |DIV: 1| --> MCLK (OUT) --> GCLK0 (IN) --> |DIV: 1| --> GCLK0 (OUT) --> TC0 (IN) --> |DIV: 1024|
 *
 * Timer Architecture:
 *		For the purposes of this problem, we will be using a 32-bit timer in 8-bit mode.
 *		We will be implementing compare match. Other words: an accurate frequency generator.
 *		The output at the pin will be a 50% Duty Cycle.
 *  
 * Resources:
 *		[1] SAM L21 Family Data Sheet
 */ 

#include "init_timer0.h"


void init_timer0(void){
	
		/* Enable TC0 in Interrupt */
		NVIC_EnableIRQ( TC0_IRQn );
		
		/* Enable General Clock 0 with OSC16M*/
		REG_GCLK_GENCTRL0 |= (GCLK_GENCTRL_DIV(1) | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSC16M);	// Enable Gen 0 (Pg. 135) | Gen0 Source: OSC 16M (Pg. 135)
		while(GCLK->SYNCBUSY.bit.GENCTRL0 == 1){}								// Wait for synchronization	(Pg. 133)
		
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

		// Enable TC0 Interrupt TriggersS
		REG_TC0_INTENSET |= (TC_INTENSET_ERR | TC_INTENSET_OVF | TC_INTENSET_MC0);
		
		// Start Timer 0
		TC0->COUNT8.CTRLA.bit.ENABLE = 1;					//Enable Timer 0
		while( TC0->COUNT8.SYNCBUSY.bit.ENABLE == 1){}		// Wait for TC0 to be enabled (Pg. 654) [1]
			
}

void stop_timer0(void){
	TC0->COUNT8.CTRLA.bit.ENABLE = 0;				// Disable Clock
	while(TC0->COUNT8.STATUS.bit.STOP == 0);		// Wait till Counter Stopped
}

void start_timer0(void){
	TC0->COUNT8.CTRLA.bit.ENABLE = 1;				// Enable Timer 0
	while( TC0->COUNT8.SYNCBUSY.bit.ENABLE == 1){}	// Wait for TC0 to be enabled (Pg. 654) [1]
}
