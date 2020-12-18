/*
 * sys_def.h
 *
 * Created: 12/10/2020 1:31:54 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Definitions:
 *		This header file contains the commonly used definition declarations across this project
 */ 


#ifndef SYS_DEF_H_
#define SYS_DEF_H_


/****************************************************************
						   Definitions
****************************************************************/
// -------------------------- Clocks ---------------------------
#define CPUDIV				1

// -------------------------- Buffer ---------------------------
#define BUF_UART_RX_LEN		5
#define BUF_TEMP_LEN		9

// -------------------------- PORTS ----------------------------
#define led					PORT_PB10
#define button				PORT_PA02
#define VCOM_TX				PORT_PA22
#define VCOM_RX				PORT_PA23

#define readporta			REG_PORT_IN0
#define readportb			REG_PORT_IN1
#define porta				0
#define portb				1

#define ledportset()		PORT->Group[portb].OUTSET.reg = led;
#define ledportclr()		PORT->Group[portb].OUTCLR.reg = led;
#define ledporttgl()		PORT->Group[portb].OUTTGL.reg = led;
#define leddirset()			PORT->Group[portb].DIRSET.reg = led;
#define leddirclr()			PORT->Group[portb].DIRCLR.reg = led;

// ------------------------ PORT MUX ----------------------------
#define PMUX_A				0
#define PMUX_B				1
#define PMUX_C				2
#define PMUX_D				3
#define PMUX_E				4
#define PMUX_F				5
#define PMUX_G				6
#define PMUX_H				7
#define PMUX_I				8

// --------------------- CPU Freq Select -----------------------
#define OSC4MHz				(uint32_t)4000000
#define OSC8MHz				(uint32_t)8000000
#define OSC12MHz			(uint32_t)12000000
#define OSC16MHz			(uint32_t)16000000
#define OSCULP32KHz			(uint32_t)32001
#define OSC32KHz			(uint32_t)32002
#define XOSC32KHz			(uint32_t)32003
#define OSC48MHz			(uint32_t)48000000

// --------------------- UART Baud Rates -----------------------
#define uart_bps_2400		2400
#define uart_bps_4800		4800
#define uart_bps_9600		9600
#define uart_bps_19200		19200
#define uart_bps_38400		38400
#define uart_bps_57600		57600
#define uart_bps_115200		115200

//--------------- Generic Clock Peripheral Channel --------------
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



#endif /* SYS_DEF_H_ */