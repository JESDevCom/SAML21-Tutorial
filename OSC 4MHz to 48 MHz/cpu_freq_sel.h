/*
 * init_clock.h
 *
 * Created: 12/6/2020 7:24:41 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef INIT_CLOCK_H_
#define INIT_CLOCK_H_

/****************************************************************
						   Header Files
****************************************************************/
#include "sam.h"
#include "init_32KHz.h"


/****************************************************************
						   Definitions
****************************************************************/
#define OSC4MHz		0xFF & OSCCTRL_OSC16MCTRL_FSEL_4	// Val = 0
#define OSC8MHz		0xFF & OSCCTRL_OSC16MCTRL_FSEL_8	// Val = 1
#define OSC12MHz	0xFF & OSCCTRL_OSC16MCTRL_FSEL_12	// Val = 2
#define OSC16MHz	0xFF & OSCCTRL_OSC16MCTRL_FSEL_16   // Val = 3
#define OSCULP32KHz	4
#define OSC32KHz	5
#define XOSC32KHz	6
#define OSC48MHz	255


// Table 17-9 PCHCTRL Mapping (Pg 138)
#define GCLK_PCHCTRL_DFLL48M_REF	0
#define GCLK_PCHCTRL_DPLL			1 // FPPLL96M input clock source for reference
#define GCLK_PCHCTRL_DPLL_32K		2 // FDPLL96M 32kHz clock for FDPLL96M internal lock timer
#define GCLK_PCHCTRL_EIC			3
#define GCLK_PCHCTRL_USB			4
#define GCLK_PCHCTRL_EVSYS_CH0		5
#define GCLK_PCHCTRL_EVSYS_CH1		6
#define GCLK_PCHCTRL_EVSYS_CH2		7
#define GCLK_PCHCTRL_EVSYS_CH3		8
#define GCLK_PCHCTRL_EVSYS_CH4		9
#define GCLK_PCHCTRL_EVSYS_CH5		10
#define GCLK_PCHCTRL_EVSYS_CH6		11
#define GCLK_PCHCTRL_EVSYS_CH7		12
#define GCLK_PCHCTRL_EVSYS_CH8		13
#define GCLK_PCHCTRL_EVSYS_CH9		14
#define GCLK_PCHCTRL_EVSYS_CH10		15
#define GCLK_PCHCTRL_EVSYS_CH11		16
#define GCLK_PCHCTRL_SERCOM0_CORE	18
#define GCLK_PCHCTRL_SERCOM1_CORE	19
#define GCLK_PCHCTRL_SERCOM2_CORE	20
#define GCLK_PCHCTRL_SERCOM3_CORE	21
#define GCLK_PCHCTRL_SERCOM4_CORE	22
#define GCLK_PCHCTRL_SERCOM5_SLOW	23
#define GCLK_PCHCTRL_SERCOM5_CORE	24
#define GCLK_PCHCTRL_TCC0			25
#define GCLK_PCHCTRL_TCC1			25
#define GCLK_PCHCTRL_TCC2			26
#define GCLK_PCHCTRL_TC0			27
#define GCLK_PCHCTRL_TC1			27
#define GCLK_PCHCTRL_TC2			28
#define GCLK_PCHCTRL_TC3			28
#define GCLK_PCHCTRL_TC4			29
#define GCLK_PCHCTRL_ADC			30
#define GCLK_PCHCTRL_AC				31
#define GCLK_PCHCTRL_DAC			32
#define GCLK_PCHCTRL_PTC			33
#define GCLK_PCHCTRL_CCL			34

// Table 11-5 NVM Software Calibration Area Mapping (Pg. 45)
#define DFLL48M_COURSE_CAL_MASK		0xFC000000
#define DFLL48M_COURSE_CAL_POS		26
#define OSC32KCAL_MASK				0x00001FC0
#define OSC32KCAL_POS				6
#define USB_TRANSN_MASK				0x0003E000
#define USB_TRANSN_POS				13


/****************************************************************
				       Function Prototypes
****************************************************************/
void cpu_freq_sel(uint8_t clk_sel);
void PD_sel(uint8_t clk_sel);
void init_GCLK(uint32_t gen_num, uint8_t clk_src, uint32_t divider);
void stop_GCLK(uint32_t gen_num);

#endif /* INIT_CLOCK_H_ */