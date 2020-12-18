/*
 * File: main.h
 * Project: ADC Read Temp.c
 * Created: 12/11/2020 4:52:50 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef MAIN_H_
#define MAIN_H_

/****************************************************************
						   Header Files
****************************************************************/
#include <sam.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // need for string support

#include "sam.h"
#include "sys_def.h"
#include "sys_ascii.h"

#include "init_ports.h"
#include "init_timer.h"
#include "init_uart.h"
#include "init_adc.h"

#include "ISR_SERCOM3.h"
#include "ISR_TC0.h"
#include "ISR_ADC.h"

#include "delay.h"
#include "cpu_freq_sel.h"
#include "debounce.h"
#include "uart_rx.h"
#include "uart_tx.h"
#include "temperature_calc.h"
#include "temperature_sprintf.h"



/****************************************************************
						   Definitions
****************************************************************/
#define NULL_CHAR			0x00
#define o_CHAR				0x6F
#define TEMP_BUFF_LEN		9

/****************************************************************
						Global Declarations
****************************************************************/
extern volatile uint32_t	F_CPU;
extern volatile uint8_t		buff_index;
extern volatile char		RX_BUFFER[BUF_UART_RX_LEN];
extern volatile char		RX_CHAR;
extern volatile uint32_t	int_timer;
extern volatile uint16_t	ADCx;
extern volatile char		BUFFER_TEMPERATURE[BUF_TEMP_LEN];

/****************************************************************
						   Function Prototypes
****************************************************************/



#endif /* MAIN_H_ */