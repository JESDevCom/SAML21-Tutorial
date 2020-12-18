/*
 * init_timer.c
 *
 * Created: 11/30/2020 6:49:03 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Initializes timer 0 for a 4 [ms] interrupt
 *
 *
 * Clock Architecture:
 *		Int OSC --> GCLK0 (IN) --> |GCLK0_DIV: 1| --> GCLK0 (OUT) --> MCLK (IN) --> |CPU_DIV: 1| --> MCLK (OUT) --> CPU
 *													     |
 *      -------------------------------------------------|
 *		|---> TC0 (IN) --> |DIV: 1024| --> TC0 (OUT)
 *
 * Formulas:
 *		
 *		Data sheet Formula for Match Frequency Generation (MFRQ)
 *
 *									.----------------------------------------------.
 *									|	F_pwm_ss = F_gclk_tc / (N * (TOP + 1))     |     (Pg. 631) [1]
 *									'----------------------------------------------'
 *
 *
 *		My Output Frequency Formula: **CPU and GCLK0 Dividers are usually: 1 **
 *
 *					Freq = CPU_freq / ([CPU_DIV] * [GCLK0_DIV] * [TC_Prescalar] * [CC0_TOP + 1])
 *
 *		Output Frequency Formula [to PORT PIN]:
 *	
 *			Freq = CPU_freq / ([PORT_DELAY] * [CPU_DIV] * [GCLK0_DIV] * [TC_Prescalar] * [CC0_TOP + 1])
 
 
 * Calculation Example:
 *
 *
 *			[internal]:  We want a 4 [ms] interrupt (250Hz = 4ms)
 *
 *				solve( 250Hz = 4MHz / ([1024] * [CC0_TOP+1]), CC0_TOP) --> CC0_TOP = 14.625
 *											
 *												OR
 *											
 *				CC0_TOP = (CPU_freq / ([TC_Prescalar] * [FREQ_DELAY])) - 1 
 *				
 *
 *			[at Pin]: We should see approx half our internal interrupt frequency due to the 2x delay at I/O
 *
 *				Freq = 4MHz / ([2] *[1024] * [round(14.625)]) = 122.1 [Hz]   <--- Measured at Pin: 122.83 [Hz]
 *
 *
 * Notes:
 *		[1] Rounding a value without math.h:
 *			uint8_t rounded = (uint8_t)(CC_float + (CC_float > 0.0f ? 0.5f : -0.5f));
 *
 * Resources:
 *		[1] SAML21 Family Data Sheet
 *
 */ 

#include "init_timer.h"

void init_timer(uint32_t delay_freq){
	
	float CC0_f = ((float)F_CPU / (1024 * (float)delay_freq)) - 1;
	uint8_t CC0 = (uint8_t)(CC0_f + (CC0_f > 0.0f ? 0.5f : -0.5f));
	
	/* Enable TC0 in Interrupt Vector */
	NVIC_EnableIRQ( TC0_IRQn );
	
	/* Peripheral Channel Control (Pg. 137) */
	REG_GCLK_PCHCTRL27 = GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_GEN_GCLK0;	// Enable Channel | Connect TC0 to GCLK0
	while(GCLK->PCHCTRL[GCLK_PCHCTRL_TC0].bit.CHEN == 0);
	
	/* Timer 0 CTRLA Settings */
	// (Pg. 641) Clock Divisions: 1024, Counter Size: 8-bits
	REG_TC0_CTRLA |= (TC_CTRLA_PRESCALER_DIV1024 | TC_CTRLA_MODE_COUNT8 | TC_CTRLA_RUNSTDBY);
	while( TC0->COUNT8.CTRLA.bit.PRESCSYNC == 1);
	
	/* Timer0 CTRLB Settings */
	TC0->COUNT8.CTRLBSET.bit.DIR = 1;					// (Pg. 645) Count Up
	
	/* Timer 0 Waveform Settings */
	TC0->COUNT8.WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;		// (Pg. 651) Mode: Freq Match, Top Value = CC0, Toggle Output.
	
	/* Timer 0 Compare Value */
	TC0->COUNT8.CC[0].reg = CC0;						// ADJUST COMPARE VALUE HERE
	while( TC0->COUNT8.SYNCBUSY.bit.CC0 == 1 );			// Wait for CC0 to be set

	// Enable Interrupt Vectors
	REG_TC0_INTENSET |= (TC_INTENSET_ERR | TC_INTENSET_OVF | TC_INTENSET_MC0);
	
	// Start Timer 0
	TC0->COUNT8.CTRLA.bit.ENABLE = 1;					//REG_TC0_CTRLA |= TC_CTRLA_ENABLE;
	while( TC0->COUNT8.SYNCBUSY.bit.ENABLE == 1);		// Wait for TC0 to be enabled		
}


