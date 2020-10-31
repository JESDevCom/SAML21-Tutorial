/*
 * main.c
 *
 * Created: 10/24/2020 5:49:28 PM
 * Author : John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Blink an LED using a timer with frequency control based on XOSC32K Clock Source
 *
 *
 * Resources:
 *		SAML21 Datasheet
 *
 *		Output Frequency Formula:
 *			Freq = CPU_freq / ([Port Delay] * [CPU_DIV] * [GCLK0_DIV] * [CC0 + 1])
 *
 *			Example: (**Count Up**)
 *			Freq = 32768kHz / ([2] * [1] * [64] * [51]) = 5.0196 [Hz]   <--- Checked Calc with Fluke 87V, Measured: 5.02 Hz
 *
 * Clock Architecture:
 *		32KHz OSC --> MCLK (IN) --> |DIV: 1| --> MCLK (OUT) --> GCLK0 (IN) --> |DIV: 1| --> GCLK0 (OUT) --> TC0 (IN) --> |DIV: 64|
 *
 * Timer Architecture:
 *		For the purposes of this problem, we will be using a 32-bit timer in 8-bit mode.
 *		We will be implementing compare match. Other words: an accurate frequency generator.
 *		The output at the pin will be a 50% Duty Cycle.
 *
 * Usage:
 *		OSCULP32K	- Enabled by default at startup, no code has to be written to enable the ULP 32K OSC.
 *					  Just link the OSCULP32K to a General Clock Gen. (Pg. 251). 32K ULP was accurate in
 *					  my calculations. Port delay is 2.0.
 *
 *		OSC32K		- Default enabled in the Master Clock under OSC32KCTRL. Must be enabled in its own OSC32KCTRL reg.
 *					  Since my port delay was 1.44 and not 2.0, I think the internal OSC32K needs calibration or
 *					  1.44 comes from some calculation regarding 16 MHz (F_cpu) that I'm not figuring out.
 *
 *		XOSC32K		- Default enabled in the Master Clock under OSC32KCTRL. Must be enabled in its own OSC32KCTRL reg.
 *					  XOSC32 was accurate in my calculations. Port delay is 2.0.
 */


 #include "main.h"


#include "sam.h"


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
	MCLK->APBAMASK.bit.OSC32KCTRL_ = 1; // (Pg. 156) OSC32 Controller should already be enabled by default, but set anyways
	MCLK->APBAMASK.bit.GCLK_   = 1; // (Pg. 156) General CLK should already be enabled by default, but set anyways
	MCLK->APBAMASK.bit.MCLK_   = 1; // (Pg. 156) Master  CLK should already be enabled by default, but set anyways
	MCLK->APBCMASK.bit.TCC0_   = 1; // (Pg. 159) Timer Counter 0
	MCLK->APBCMASK.bit.TC0_    = 1; // (Pg. 159) Timer 0
	MCLK->APBDMASK.bit.EVSYS_  = 0; // (Pg. 161) Event System; Enable for External Triggering of Counter
		
	// ----------------------- Configure OSC32K -----------------------------
	// Enable Oscillator | Enable 32KHz Output | 34 CC Startup Delay
	/*
	OSC32KCTRL->OSC32K.reg = (	OSC32KCTRL_OSC32K_ENABLE |
								OSC32KCTRL_OSC32K_EN32K  |
								OSC32KCTRL_OSC32K_STARTUP(4));
	while(OSC32KCTRL->STATUS.bit.OSC32KRDY == 0){} // (Pg. 256)
		*/
	
		
	// ----------------------- Configure XOSC32K ---------------------------- (Pg. 259)
	// Enable Ext Oscillator | Crystal Oscillator Enable | Enable 32 KHz Output | 4096 CC Startup Delay or 0.13[s]
	OSC32KCTRL->XOSC32K.reg = ( OSC32KCTRL_XOSC32K_ENABLE |
								OSC32KCTRL_XOSC32K_XTALEN |
								OSC32KCTRL_XOSC32K_EN32K  |
								OSC32KCTRL_XOSC32K_STARTUP(1));
	while(OSC32KCTRL->STATUS.bit.XOSC32KRDY == 0){} // (Pg. 256)
	

	
	/* Enable General Clock 2 with ULP32KHz */
	REG_GCLK_GENCTRL2 |= (GCLK_GENCTRL_DIV(1) | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_XOSC32K);	// Clock Divide: 1 | Enable Gen 2 (Pg. 135) | Gen0 Source: XOSC32K (Pg. 135)
	while(GCLK->SYNCBUSY.bit.GENCTRL2 == 1){}							// Wait for synchronization	(Pg. 133)
	
	/* Peripheral Channel Control (Pg. 137) */
	REG_GCLK_PCHCTRL27 |=  (GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_GEN_GCLK2);	// Enable Channel | Connect TC0 to GCLK2
	

	/* Timer 0 CTRLA Settings */
	// (Pg. 641) Clock Divisions: 64, Counter Size: 8-bits, Still Run in Standby Mode (Not needed)
	REG_TC0_CTRLA |= (TC_CTRLA_PRESCALER_DIV64 | TC_CTRLA_MODE_COUNT8 | TC_CTRLA_RUNSTDBY);
	while( TC0->COUNT8.CTRLA.bit.PRESCSYNC == 1){}
	
	/* Timer0 CTRLB Settings */
	TC0->COUNT8.CTRLBSET.bit.DIR = 1;					// (Pg. 645) Count Up
	
	/* Timer 0 Waveform Settings */
	TC0->COUNT8.WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;		// (Pg. 651) Mode: Freq Match, Top Value = CC0, Toggle Output.
	
	/* Timer 0 Compare Value */
	TC0->COUNT8.CC[0].reg = 50;							// ADJUST HERE
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
