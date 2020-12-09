/*
 * init_clock.c
 *
 * Created: 12/6/2020 7:24:23 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Notes:
 *		[1] "When changing to a higher performance level, the bus frequency can be increased only after
 *		the Performance Level Ready flag in the Interrupt Flag Status and Clear (INTFLAG.PLRDY) bit
 *		set to '1', indicating that the performance level transition is complete." [1]
 *
 *		[2] Maximum Peripheral Clock Frequency are limited by the CPU frequency, which is limited by
 *		the Performance Level Value. (See Table 46-6, Pg. 1041) for more info. Also, PL0 can run the
 *		internal OSC at 4MHz, 8MHz, and 12 MHz. You must go PL2 for OSC16M to run at the full 16MHz.
 *		
 */ 

#include "cpu_freq_sel.h"

void cpu_freq_sel(uint8_t clk_sel){


	PD_sel(clk_sel);	/* Set Performance Level */	
	

	if (clk_sel == OSC48MHz){
		
		init_32KHz(OSC32KHz);		/* Start 32KHz Clock */
		init_GCLK(1, OSC32KHz, 1);	/* Init Clock Gen 1  */
		

		
		// ----------------------- Peripheral CH 0 Control -------------------------------- (Pg. 138) [1]
		GCLK->PCHCTRL[GCLK_PCHCTRL_DFLL48M_REF].reg = (
								GCLK_PCHCTRL_GEN_GCLK1	|	// Clock Gen In: Clock Gen 1	(Pg. 137) [1]
								GCLK_PCHCTRL_CHEN);			// Enable Peripheral Controller (Pg. 137) [1]
								
		//------------------------ Configure Flash Mem ------------------------------------
		NVMCTRL->CTRLB.reg = (NVMCTRL_CTRLB_RWS(2)		|	// 2 Wait State, (Table 46-39)  (Pg. 452) [1]
							  NVMCTRL_CTRLB_CACHEDIS);		// Disable NVM Cache 			(Pg. 451) [1]
							  
		
		// ----------------------- Configure DFLL48M -------------------------------------- (Pg. 235) [1]
		OSCCTRL->DFLLCTRL.reg = 0;							// Clear DFLL Control Reg		(Pg. 236) [1]
		while(OSCCTRL->INTFLAG.bit.DFLLRDY == 0);			// Wait till DFLL is stable &	(Pg. 231) [1]
															// ready for Read/write access
		
		/* Notes: [1] NVMCTRL_OTP5 is the address in Mem for NVM Software Calibration Area Mapping. (Pg. 45) [1]
						- The bits positions where the calibration data is in bit positions 31:26 
						- The equation seems based on equation found on Pg. 215 
				  [2] No fine value is available from Atmel, so we just guess a large 10-bit value 
					  This value gets reset anyways when we write to OSCCTRL_DFLLMUL_MUL. The value
					  will be reset to its midpoint (Pg. 238).                                   */
		
		const uint32_t course = (*((uint32_t *)NVMCTRL_OTP5 + (DFLL48M_COURSE_CAL_POS / 32)) & DFLL48M_COURSE_CAL_MASK) >> DFLL48M_COURSE_CAL_POS;
		const uint16_t fine   = 0x1FF; // See Note [2]
		
		OSCCTRL->DFLLVAL.reg = (
								OSCCTRL_DFLLVAL_COARSE(course)  |
								OSCCTRL_DFLLVAL_FINE(fine));
								while(OSCCTRL->INTFLAG.bit.DFLLRDY == 0);


		/* Notes: [1] A small step value will take longer to lock, but will be more accurate
				  [2] CSTEP and FSTEP should not be higher than 50% of the max value of 
					  COURSE and FINE */
		OSCCTRL->DFLLMUL.reg = (OSCCTRL_DFLLMUL_CSTEP(0x04)	   |// Course Maximum Step Size	(Pg. 238) [1]
							    OSCCTRL_DFLLMUL_FSTEP(1)	   |// Fine Maximum Step Size
								OSCCTRL_DFLLMUL_MUL(48000));	// 48,000,000 MHz
								while(OSCCTRL->STATUS.bit.DFLLRDY == 0);
				

		
		OSCCTRL->DFLLCTRL.reg = (
								OSCCTRL_DFLLCTRL_RUNSTDBY	|	// Run in Standby			(Pg. 235) [1]
								OSCCTRL_DFLLCTRL_CCDIS		|	// Chill Cycle Disabled		(Pg. 235) [1]
								OSCCTRL_DFLLCTRL_MODE		|	// DFLL set to Closed Loop  (Pg. 236) [1]
								OSCCTRL_DFLLCTRL_USBCRM);		// Enable Calib over USB	(Pg. 236) [1]	
								while(OSCCTRL->STATUS.bit.DFLLRDY == 0);
		
		OSCCTRL->DFLLCTRL.bit.ENABLE   = 1;					// Enable DFLL Oscillator		(Pg. 236) [1]
		while(OSCCTRL->STATUS.bit.DFLLRDY == 0);
		

		// ----------------------- Switch Clock Source ------------------------------------ (Pg. 134) [1]
		GCLK->GENCTRL[0].reg = (GCLK_GENCTRL_SRC_DFLL48M	|	// Clock Source: DFLL48M
								GCLK_GENCTRL_DIV(1)			|	// Clock Divider: 1
								GCLK_GENCTRL_RUNSTDBY		|	// Run even in standby
								GCLK_GENCTRL_GENEN);			// Enable Gen 0
								while(GCLK->SYNCBUSY.bit.GENCTRL0 == 1); //					(Pg. 133) [1]
								
		// ---------------------- Disable Generic Clock Controller 0 ----------------------
		stop_GCLK(1);
	}
	else{
		if (clk_sel < OSC48MHz){
			// Oscillator Frequency Selection (Pg. 234) [1]
			OSCCTRL->OSC16MCTRL.reg = (OSCCTRL_OSC16MCTRL_ENABLE | clk_sel);
			while(OSCCTRL->STATUS.bit.OSC16MRDY == 0);
		}
	}		
}



/*****************************************************************************************************
									 Performance Domain Select
******************************************************************************************************
Description:
	This function selects the Performance Power Mode based on the CPUs Frequency requirement.

Use:
	Input		| Value
	OSC4MHz		| 0
	OSC8MHz		| 1
	OSC12MHz	| 2
	OSC16MHz	| 3
	OSCULP32KHz | 4
	OSC32KHz	| 5
	XOSC32KHz	| 6
	OSC48MHz	| 255

Notes: 
	[1] Enable Highest Performance Power Mode (Pg. 201) [1] 
	[2] Maximum Peripheral Clock Frequency are limited by the CPU frequency, which is limited by
		the Performance Level Value. (See Table 46-6, Pg. 1041) for more info. Also, PL0 can run the
		internal OSC at 4MHz, 8MHz, and 12 MHz. You must go PL2 for OSC16M to run at the full 16MHz. 
----------------------------------------------------------------------------------------------------*/							
	
void PD_sel(uint8_t clk_sel){
	
	switch(clk_sel){
		
		case OSC16MHz: case OSC48MHz:
			PM->INTFLAG.bit.PLRDY = 1;	// Writing a '1' to this bit clears the Performance Ready Interrupt flag (Pg. 204 [1])
			PM->PLCFG.bit.PLSEL = 2;	// Enable Performance Level 2 (Pg. 201 [1])
			while(PM->INTFLAG.bit.PLRDY == 0); // Wait till Ready
			break;
		
		default:
			/*
			PM->INTFLAG.bit.PLRDY = 1;	// Writing a '1' to this bit clears the Performance Ready Interrupt flag (Pg. 204 [1])
			PM->PLCFG.bit.PLSEL = 0;	// Enable Performance Level 0 (Pg. 201 [1])
			while(PM->INTFLAG.bit.PLRDY == 0); // Wait till Ready
			*/
			break;			
		}
}


/*****************************************************************************************************
								Initialize Generic Clock Controller
******************************************************************************************************
Description:
	This function sets up a Generic Clock.
----------------------------------------------------------------------------------------------------*/
void init_GCLK(uint32_t gen_num, uint8_t clk_src, uint32_t divider){
	
	// Initialize Generic Clock Divider
	GCLK->GENCTRL[gen_num].reg |= GCLK_GENCTRL_DIV(divider);
	
	// Initialize Generic Clock Source
	switch(clk_src){
		case OSCULP32KHz:	GCLK->GENCTRL[gen_num].reg |= GCLK_GENCTRL_SRC_OSCULP32K;	break;
		case OSC32KHz:		GCLK->GENCTRL[gen_num].reg |= GCLK_GENCTRL_SRC_OSC32K;		break;
		case XOSC32KHz:		GCLK->GENCTRL[gen_num].reg |= GCLK_GENCTRL_SRC_XOSC32K;		break;
		case OSC16MHz:		GCLK->GENCTRL[gen_num].reg |= GCLK_GENCTRL_SRC_OSC16M;		break;
		case OSC48MHz:		GCLK->GENCTRL[gen_num].reg |= GCLK_GENCTRL_SRC_DFLL48M;		break;
		default:			GCLK->GENCTRL[gen_num].reg |= GCLK_GENCTRL_SRC_OSC32K;		break;
	}
	
	// Enable Generic Clock
	GCLK->GENCTRL[gen_num].bit.GENEN = 1;
	
	// Wait till GCLK is stable and running
	while(GCLK->SYNCBUSY.bit.GENCTRL1 == 1);
}


void stop_GCLK(uint32_t gen_num){
	
	// Stop Generic Clock
	GCLK->GENCTRL[gen_num].bit.GENEN = 0;
}
