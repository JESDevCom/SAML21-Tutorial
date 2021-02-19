/*
 * USB_Mass_Storage_Part1.c
 *
 * Created: 12/19/2020 3:55:06 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Send a message over USB using the CDC ACM protocol to a PuTTY console on the host.
 *
 * Hardware Usage:
 *		Host Computer OS: Windows 10
 *
 *
 * Project Size:
 *		Optimization ~O0 (None):
 *
 *		Optimization ~03 (Most):
 *
 * Clock Usage:
 *
 *				.-----------.
 *  OSC16M ---->|  OSCCTRL	| 
 *				`-----------'
 *					  |
 *					12 MHz
 *					  |
 *				.-----------.
 *				|	GCLK0	| ----> 12 MHz ----> CPU
 *				`-----------'	|
 *                              |        
 *								`--------------------- CLK_USB_AHB ---------------------.
 *								                  (Require: min of 8 MHz)               |
 *				.-----------.	                                                 .-------------.
 *		 .----->|   GCLK2	| ----> 48 MHz										 |  USB CTRLr  | ----> 480 MHz 
 *		 |	    `-----------'   |                                                `-------------'
 *       |                      |                                                       |   
 *       |                      `---------------------> GCLK_USB -----------------------' Ref Clk
 *		 |                                     (Require: 48MHz +/- 0.25%)
 *       |	
 *		 `------------------------------------------------------.				
 *																|
 *				.-----------.					 .----------.	|				
 *				|	GCLK1	| ----> 32.768KHz -> |	48MDLL	| ----> 48 MHz  
 *				'-----------'					 `----------'					
 *					  |
 *				   32.768 KHz
 *					  |
 *				.-----------.
 *  OSC32K ---->| OSC32KCTRL| ----> 1 KHz -----> Start of Frame (SOP)
 *				`-----------'
 *
 * Conclusion:
 *		
 *
 * Resources:
 *		[1] SAM L21 Family Data Sheet
 *		[2] Atmel SAM L21 Xplained Pro User Guide
 *		[3] USB 2.0 Manual: http://sdphca.ucsd.edu/lab_equip_manuals/usb_20.pdf
 *		[4] USB-IF: https://www.usb.org/defined-class-codes
 *		[5] WMC110_errata.pdf
 *		[6] CDC120_errata.pdf
 *		[7] https://www.silabs.com/documents/public/application-notes/AN758.pdf
 *		
 */

 #include "main.h"

 volatile uint32_t F_CPU = 4000000; // Default CPU startup frequency
 volatile uint8_t buff_index = 0;
 volatile char RX_BUFFER[BUF_UART_RX_LEN] = {'\0'};
 volatile char RX_CHAR = NULL_CHAR;
 volatile uint32_t int_timer = 0;

static void sys_init(void);
 
//static char message0[] = "DEBUG USB:  CONNECTION ESTABLISHED\r\nHello World!\n\n";
static char message1[] = "TARGET USB: CONNECTION ESTABLISHED\r\nHello World!\r\n";
char here0[] = "\r\nHERE 0\r\n";

static void sys_init(void)
{
	PM->INTFLAG.reg = PM_INTFLAG_PLRDY;
	PM->PLCFG.reg = PM_PLCFG_PLSEL_PL2;
	while (!PM->INTFLAG.reg);

	MCLK->LPDIV.reg = MCLK_LPDIV_LPDIV_DIV4;
	while (0 == (MCLK->INTFLAG.reg & MCLK_INTFLAG_CKRDY));

	MCLK->BUPDIV.reg = MCLK_LPDIV_LPDIV_DIV8;
	while (0 == (MCLK->INTFLAG.reg & MCLK_INTFLAG_CKRDY));

	OSCCTRL->OSC16MCTRL.reg = OSCCTRL_OSC16MCTRL_ENABLE | OSCCTRL_OSC16MCTRL_FSEL_16;

	NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CACHEDIS | NVMCTRL_CTRLB_RWS(2);

	SUPC->INTFLAG.reg = SUPC_INTFLAG_BOD33RDY | SUPC_INTFLAG_BOD33DET;
	OSCCTRL->INTFLAG.reg = OSCCTRL_INTFLAG_DFLLRDY | OSCCTRL_INTFLAG_OSC16MRDY;

	OSCCTRL->DFLLCTRL.reg = 0;
	while (0 == (OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY));

	OSCCTRL->DFLLMUL.reg = OSCCTRL_DFLLMUL_MUL(48000) | OSCCTRL_DFLLMUL_CSTEP(1) | OSCCTRL_DFLLMUL_FSTEP(1);
	while (0 == (OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY));

	const uint32_t course = (*((uint32_t *)NVMCTRL_OTP5 + (DFLL48M_COURSE_CAL_POS / 32)) & DFLL48M_COURSE_CAL_MASK) >> DFLL48M_COURSE_CAL_POS;
	OSCCTRL->DFLLVAL.reg = (OSCCTRL_DFLLVAL_COARSE(course)  |
							OSCCTRL_DFLLVAL_FINE(0x200));
							while(OSCCTRL->INTFLAG.bit.DFLLRDY == 0);
	
	OSCCTRL->DFLLCTRL.reg = OSCCTRL_DFLLCTRL_ENABLE | OSCCTRL_DFLLCTRL_MODE | OSCCTRL_DFLLCTRL_USBCRM;
	while (0 == (OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY));

	GCLK->GENCTRL[0].reg = GCLK_GENCTRL_SRC(GCLK_SOURCE_DFLL48M) | GCLK_GENCTRL_GENEN;
	while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL0);	
}






int main(void)
{

	init_ports();
	sys_init();
	sys_time_init();
	usb_hw_init();
	usb_cdc_init();
	set_status_state(0);
	//TX_string(here0);
	

	/* Replace with your application code */
	while (1)
	{
		/*
		scheduler = time_srv^int_timer;
		
		// -------------------------------------------- serviced every: 1.0 [s]
		if (scheduler & 0x100){
			time_srv = time_srv ^ 0x100;
			ledporttgl();
			TX_string(message0);
			//SendStringToHost(message1);
		}
		*/
		sys_time_task();
		usb_flag_handler();
		SendStringToHost(message1);
		SendDataViaUART_RecvFromHost();
		//SendToHost_DataReceivedViaUart(); 
		uart_timer_task();
		status_task();
		
	}
} // END MAIN

