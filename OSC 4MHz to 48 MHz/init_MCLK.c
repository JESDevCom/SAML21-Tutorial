/*
 * init_MCLK.c
 *
 * Created: 12/6/2020 7:55:13 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 

#include "init_MCLK.h"

void init_MCLK(void){
	
	/* Enable Timer 0 Access to Master Clock (MCLK) Peripheral Bus (Pg. 146) */
	MCLK->APBAMASK.bit.GCLK_   = 1; // (Pg. 156) General CLK should already be enabled by default, but set anyways
	MCLK->APBAMASK.bit.MCLK_   = 1; // (Pg. 156) Master  CLK should already be enabled by default, but set anyway
	MCLK->APBCMASK.bit.TCC0_   = 1; // (Pg. 159) Timer Counter 0
	MCLK->APBCMASK.bit.TC0_    = 1; // (Pg. 159) Timer 0
	MCLK->APBDMASK.bit.EVSYS_  = 0; // (Pg. 161) Event System; Enable for External Triggering of Counter
	
}
