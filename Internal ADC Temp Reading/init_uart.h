/*
 * init_uart.h
 *
 * Created: 12/5/2020 3:57:25 PM
 * Author : John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef INIT_UART_H_
#define INIT_UART_H_


/****************************************************************
						   Header Files
****************************************************************/
#include <stdlib.h> // need for uint64_t variable type
#include <stdint.h> // need for uint64_t variable type
#include "sam.h"


/****************************************************************
						   Definitions
****************************************************************/
#define VCOM_TX				PORT_PA22
#define VCOM_RX				PORT_PA23
#define porta				0
#define portb				1

#define uart_bps_2400		2400
#define uart_bps_4800		4800
#define uart_bps_9600		9600
#define uart_bps_19200		19200
#define uart_bps_38400		38400
#define uart_bps_57600		57600
#define uart_bps_115200		115200

// Table 17-9 PCHCTRL Mapping (Pg 138)
#define GCLK_DFLL48M_REF	0
#define GCLK_DPLL			1 // FPPLL96M input clock source for reference
#define GCLK_DPLL_32K		2 // FDPLL96M 32kHz clock for FDPLL96M internal lock timer
#define GCLK_EIC			3
#define GCLK_USB			4
#define GCLK_EVSYS_CH0		5
#define GCLK_EVSYS_CH1		6
#define GCLK_EVSYS_CH2		7
#define GCLK_EVSYS_CH3		8
#define GCLK_EVSYS_CH4		9
#define GCLK_EVSYS_CH5		10
#define GCLK_EVSYS_CH6		11
#define GCLK_EVSYS_CH7		12
#define GCLK_EVSYS_CH8		13
#define GCLK_EVSYS_CH9		14
#define GCLK_EVSYS_CH10		15
#define GCLK_EVSYS_CH11		16
#define GCLK_SERCOM0_CORE	18
#define GCLK_SERCOM1_CORE	19
#define GCLK_SERCOM2_CORE	20
#define GCLK_SERCOM3_CORE	21
#define GCLK_SERCOM4_CORE	22
#define GCLK_SERCOM5_SLOW	23
#define GCLK_SERCOM5_CORE	24
#define GCLK_TCC0			25
#define GCLK_TCC1			25
#define GCLK_TCC2			26
#define GCLK_TC0			27
#define GCLK_TC1			27
#define GCLK_TC2			28
#define GCLK_TC3			28
#define GCLK_TC4			29
#define GCLK_ADC			30
#define GCLK_AC				31
#define GCLK_DAC			32
#define GCLK_PTC			33
#define GCLK_CCL			34

/****************************************************************
						   Global Variables
****************************************************************/
extern volatile uint32_t F_CPU;

/****************************************************************
						 Function Prototypes
****************************************************************/
void init_uart(uint64_t baud);
void init_uart_ports(void);


#endif /* INIT_UART_H_ */