/*
* init_DFLL48M.c
*
* Created: 12/10/2020 2:58:09 PM
* Author: John
* Board: Atmel SAML21 Xplained Pro
* MCU: Microchip SAML21J18B
* IDE: Atmel Studio v7
* ASF: No
*
* Description:
*		In this function we will setup clock to run the CPU at 48MHz using DFLL in close-loop configuration.
*
* Setup Steps:
*		[1] Initialize a reference clock
*		[2] Initialize a Generic Clock Controller with reference clock as the source
*		[3] Using table 17-8, setup the correct peripheral channel register for DFLL48M operation and connect your lock clock gen you selected in step 2 to use.
*		[4] Use table 46-39 to configure your memory controller to operate correctly at 48MHz.
*		[5] Configure the DFLL48M registers. 
*				(a) We will be using a closed-loop configuration since we are using a reference clock.
*				(b) Initialize the factory calibration values
*		[6] Enable DFLL48M
*		[7] Your CPU runs from GCLK0, so switch GCLK0 clock source to GCLK_GENCTRL_SRC_DFLL48M.
*		[8] Shutdown your Generic Clock Controller used from step 2, since it is no longer need.
*
* Notes:
*		[1] The reason we disable NVM data caching is due to an issue when using the DMA to transfer data
*			from the uCs flash memory over USB to host. Source: https://www.avrfreaks.net/forum/same70-usb-cache
*			May not be an issue on the L21/22 series, so we will disable it now for precautions.
*
* Reference:
*		[1] SAM L21 Family Data Sheet
*/ 

#include "init_DFLL48M.h"

void init_DFLL48M(uint32_t ref_clk){
	
	// --------------------- Initialize Reference Clock Source ------------------------
	/* Notes:
			[1] Start Internal 32K Clock as Reference Source 
			[2]	The external 32K or internal ULP 32K can also be used here. 
					- OSCULP32KHz
					- XOSC32KHz
					- OSC32KHz
			*/
	init_32KHz(ref_clk);
	
	
	/* Notes: 
			[1] Select GCLK: 1
			[2] Select a GCLK source: OSC32KHz
			[3]	Select a clock divider factor: 1 (we need no divider in this case) 
			*/
	init_GCLK(1, ref_clk, 1);	
		

		
	// ----------------------- Peripheral CH 0 Control -------------------------------- (Pg. 138) [1]
	GCLK->PCHCTRL[GCLK_PCHCTRL_DFLL48M_REF].reg = (
							GCLK_PCHCTRL_GEN_GCLK1	|	// Clock Gen In: Clock Gen 1	(Pg. 137) [1]
							GCLK_PCHCTRL_CHEN);			// Enable Peripheral Controller (Pg. 137) [1]
								
	//------------------------ Configure Flash Mem ------------------------------------
	NVMCTRL->CTRLB.reg = (NVMCTRL_CTRLB_RWS(2)		|	// 2 Wait State, (Table 46-39)  (Pg. 452) [1]
						  NVMCTRL_CTRLB_CACHEDIS);      // Disable NVM Cache,
														// See Note [1] in header		(Pg. 451) [1]
							  
		
	// ----------------------- Configure DFLL48M -------------------------------------- (Pg. 235) [1]
	OSCCTRL->DFLLCTRL.reg = 0;							// Clear DFLL Control Reg		(Pg. 236) [1]
	while(OSCCTRL->INTFLAG.bit.DFLLRDY == 0);			// Wait till DFLL is stable &	(Pg. 231) [1]
														// ready for Read/write access
		
	/* Notes:	[1] NVMCTRL_OTP5 is the address in Mem for NVM Software Calibration Area Mapping. (Pg. 45) [1]
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


	/* Notes:	[1] A small step value will take longer to lock, but will be more accurate
				[2] CSTEP and FSTEP should not be higher than 50% of the max value of 
					COURSE and FINE */
	OSCCTRL->DFLLMUL.reg = (OSCCTRL_DFLLMUL_CSTEP(0x04)	   |// Course Maximum Step Size		(Pg. 238) [1]
							OSCCTRL_DFLLMUL_FSTEP(1)	   |// Fine Maximum Step Size
							OSCCTRL_DFLLMUL_MUL(48000));	// 48,000,000 MHz
							while(OSCCTRL->STATUS.bit.DFLLRDY == 0);
				

	if ((ref_clk == OSC32KHz) || (ref_clk == OSCULP32KHz)){
		OSCCTRL->DFLLCTRL.reg = (
								OSCCTRL_DFLLCTRL_RUNSTDBY	|	// Run in Standby			(Pg. 235) [1]
								//OSCCTRL_DFLLCTRL_CCDIS	|	// Chill Cycle Disabled		(Pg. 235) [1]
								OSCCTRL_DFLLCTRL_MODE		|	// DFLL set to Closed Loop  (Pg. 236) [1]
								OSCCTRL_DFLLCTRL_USBCRM);		// Enable Calib over USB	(Pg. 236) [1]	
								while(OSCCTRL->STATUS.bit.DFLLRDY == 0);
	}
	else{
		OSCCTRL->DFLLCTRL.reg = (
								OSCCTRL_DFLLCTRL_RUNSTDBY	|	// Run in Standby			(Pg. 235) [1]
								OSCCTRL_DFLLCTRL_WAITLOCK	|	// Output clock when locked	(Pg. 235) [1]
								OSCCTRL_DFLLCTRL_MODE		|	// DFLL set to Closed Loop  (Pg. 236) [1]
								OSCCTRL_DFLLCTRL_QLDIS);		// Disable Quick Lock		(Pg. 236) [1]
								while(OSCCTRL->STATUS.bit.DFLLRDY == 0);		
	}
	
	OSCCTRL->DFLLCTRL.bit.ENABLE   = 1;							// Enable DFLL Oscillator	(Pg. 236) [1]
	while(OSCCTRL->STATUS.bit.DFLLRDY == 0);
		

	// ----------------------- Switch Clock Source ------------------------------------		(Pg. 134) [1]
	GCLK->GENCTRL[0].reg = (GCLK_GENCTRL_SRC_DFLL48M	|	// Clock Source: DFLL48M
							GCLK_GENCTRL_DIV(1)			|	// Clock Divider: 1
							GCLK_GENCTRL_RUNSTDBY		|	// Run even in standby
							GCLK_GENCTRL_GENEN);			// Enable Gen 0
							while(GCLK->SYNCBUSY.bit.GENCTRL0 == 1); //						(Pg. 133) [1]
								
	// ---------------------- Disable Generic Clock Controller 1 ----------------------
	stop_GCLK(1);
	
}
