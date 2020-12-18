/*
 * init_adc.c
 *
 * Created: 12/12/2020 1:11:58 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		We will be running the ADC to periodically read the temperature value.
 *		ADC will be initialized to one conversion per trigger (ie., non-continuous operation).
 *		Setup to average 16 readings before returning a complete conversion.
 *		Averaging done in hardware. Averaging does drop conversion speed.
 *
 * Hardware:
 *		F_CPU: 4 [MHz]		
 *		F_ADC: F_CPU/8 =500 [KHz]
 *		Sampling Time (no avg): (63+1)/F_ADC = 128[us]
 *		Sampling Time (with avg): 16*(63+1)/F_ADC = 2.048 [ms]
 *		Conversion Rate (Max Theoretical) = 2048 [Samples/s]
 *		Supply Voltage (VDDIN) = 3.3 [V]
 *		ADC Voltage Reference = 1.0 [V]
 *		Temperature Accuracy: +/- 5 [Celsius]
 *
 * Initialization Temp Sensor:
 *		[1] Connect ADC to a Clock Source
 *		[2] Set Voltage Reference System in SUPC for temp sensor
 *		[3] Configure ADC
 *			[3.1] Disable the ADC (CTRLA.ENABLE=0) so we can write to the registers in the following steps
 *			[3.2] Set internal voltage reference INTREF as ADC reference voltage by writing to ADC.REFCTRL.REFSEL
 *			[3.3] Set the temp sensor vs. internal GND as input by writing TEMP and GND to the positive and
 *				  negative MUX input Selection bit fields (ADC.INPUTCTRL.MUXNEG and MUXPOS, respectively)
 *			[3.4] Config Control Register B (CTRLB)
 *			[3.5] Select ADC Resolution (CTRLC.RESSEL)
 *			[3.6] Config Event Control Register (EVCTRL)
 *			[3.7] Config Calibration Register (CALIB)
 *			[3.8] Setup Conversion Ready Interrupt
 *			[3.9] Enable ADC (CTRLA.ENABLE=1)
 *		[4] Start Conversion
 *		[4] Acquire value ADC_m via Result Conversion Ready (RESRDY) interrupt
 *
 *
 * Notes:
 *		[1] ADC has a resolution of 12-bit, 10-bit, or 8-bit
 *		[2] Calibration values for bias and linearity available in NVM Software Calibration Area
 *		[3]	ADC has two conversion options: (1) Single-Ended (pos readings), (2) Differential (pos or neg readings).
 *		[4] Capture Modes: (1) Free Running (continuously sampled), and (2) Single-Shot (sampled once)
 *		[5] Data Conversion started by (SWTRIG.START=1)
 *
 * Resources:
 *		[1] SAM L21 Family Data Sheet
 *			- Device Temperature Measurement Guide starts on Pg. 947
 */ 


#include "init_adc.h"


void init_adc(void){
	
	// Enable ADC0 Access to Master Clock (Pg. 146)
	MCLK->APBDMASK.bit.ADC_ = 1;
	
	// Enable ADC PCHCTRL (Pg. 138)
	GCLK->PCHCTRL[ADC_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN;
	
	// Enable Temp Sensor Hardware
	SUPC->VREF.reg |= (SUPC_VREF_TSEN | SUPC_VREF_SEL_1V0 | SUPC_VREF_RUNSTDBY);
	
	// Disable ADC
	ADC->CTRLA.bit.ENABLE = 0;
	
	// Calibrate (Pg. 978) [1]
	ADC->CALIB.reg = ADC_CALIB_BIASCOMP(CAL_BIASCOMP) | ADC_CALIB_BIASREFBUF(CAL_BIASREFBUF);
	
	// Enable Reference
	ADC->REFCTRL.reg = ADC_REFCTRL_REFSEL_INTREF;
	
	// Connect Temp Sensor to ADC (Pg. 961-2)
	ADC->INPUTCTRL.reg = ADC_INPUTCTRL_MUXPOS_TEMP;
	while(ADC->SYNCBUSY.bit.INPUTCTRL);
	
	// Setup ADC Clock Divider
	ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV8;
		
	// Setup Sampling Method (Pg. 964)
	ADC->CTRLC.reg =   (ADC_CTRLC_WINMODE_DISABLE	| // No window mode
						ADC_CTRLC_RESSEL_12BIT		| // 12-bit resolution
						0 << ADC_CTRLC_CORREN_Pos	| // No Digital Result Correction
						0 << ADC_CTRLC_LEFTADJ_Pos	| // Result is right-adjusted in RESULT reg
						0 << ADC_CTRLC_FREERUN_Pos  | // Run Single Conversion Capture
						0 << ADC_CTRLC_DIFFMODE_Pos );// Run Single 
	while(ADC->SYNCBUSY.bit.CTRLC);
	
	// Sampling Time (Pg. 967) [1]
	ADC->SAMPCTRL.reg = ADC_SAMPCTRL_SAMPLEN(63);    // Number of ADC clock cycles a value is sampled for
	while(ADC->SYNCBUSY.bit.SAMPCTRL);
	
	// Averaging Samples (Pg. 966) [1]
	ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_16 | (ADC_AVGCTRL_ADJRES(0x4));
	while(ADC->SYNCBUSY.bit.AVGCTRL);
		
	// Setup Interrupt for Result Ready
	NVIC_EnableIRQ(ADC_IRQn);
	ADC->INTENSET.reg = ADC_INTENSET_RESRDY;
	
	// Enable ADC
	ADC->CTRLA.bit.ENABLE = 1;
	while(ADC->SYNCBUSY.bit.ENABLE);	
	
}

uint16_t adc_read(void){
	/* Result will be located in bit locations [11:0]. (Pg. 976) [1] */
	return (0x0FFF & ADC->RESULT.reg);
}

void start_adc_conversion(void){
	/* Writing a '1' to this bit will start a conversion  (Pg. 972) [1] */
	if(ADC->INTFLAG.bit.RESRDY == 0){
		ADC->SWTRIG.bit.START = 1;
	}
}
void disable_adc(void){
	ADC->CTRLA.bit.ENABLE = 0;
}


void switch_adc_src_to_temp_sensor(void){
	
	// Disable Interrupt
	NVIC_DisableIRQ(ADC_IRQn);
	
	// Disable ADC so we can write to Write-Protected Registers
	disable_adc();
	
	// Enable Reference
	ADC->REFCTRL.reg = ADC_REFCTRL_REFSEL_INTREF;
		
	// Connect Temp Sensor to ADC
	ADC->INPUTCTRL.reg = ADC_INPUTCTRL_MUXPOS_TEMP;
	while(ADC->SYNCBUSY.bit.INPUTCTRL);
		
	// Setup ADC Clock Divider
	ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV8;
		
	// Setup Sampling Method (Pg. 964)
	ADC->CTRLC.reg =   (ADC_CTRLC_WINMODE_DISABLE	| // No window mode
	ADC_CTRLC_RESSEL_12BIT		| // 12-bit resolution
	0 << ADC_CTRLC_CORREN_Pos	| // No Digital Result Correction
	0 << ADC_CTRLC_LEFTADJ_Pos	| // Result is right-adjusted in RESULT reg
	0 << ADC_CTRLC_FREERUN_Pos  | // Run Single Conversion Capture
	0 << ADC_CTRLC_DIFFMODE_Pos );// Run Single
	while(ADC->SYNCBUSY.bit.CTRLC);
		
	// Sampling Time (Pg. 967) [1]
	ADC->SAMPCTRL.reg = ADC_SAMPCTRL_SAMPLEN(63);    // Number of ADC clock cycles a value is sampled for
	while(ADC->SYNCBUSY.bit.SAMPCTRL);
		
	// Averaging Samples (Pg. 966) [1]
	ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_16 | (ADC_AVGCTRL_ADJRES(0x4));
	while(ADC->SYNCBUSY.bit.AVGCTRL);
	
	// Re-Enable Interrupt for Result Ready
	NVIC_EnableIRQ(ADC_IRQn);
	ADC->INTENSET.reg = ADC_INTENSET_RESRDY;
		
	// Enable ADC
	ADC->CTRLA.bit.ENABLE = 1;
	while(ADC->SYNCBUSY.bit.ENABLE);
}


