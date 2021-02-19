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
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // need for string support

#include "saml21j18b.h"	
#include "sys_def.h"
#include "sys_ascii.h"

#include "init_ports.h"
#include "timers/init_timer.h"
#include "uart/init_uart.h"


/* _________ INTERRUPTS __________ */
#include "interrupts/ISR_SERCOM3.h"
#include "interrupts/ISR_TC0.h"



/* ___________ CLOCKS ____________ */
#include "delays/delay.h"
#include "clocks/cpu_freq_sel.h"
#include "uart/uart_rx.h"
#include "uart/uart_tx.h"

/* _____________ USB _____________ */
#include "ISR_SERCOM0.h"
#include "ISR_USB.h"
#include "init_USB_hw.h"
#include "usb_cdc.h"
#include "usb_std.h"


/****************************************************************
						   Definitions
****************************************************************/
#define PERIOD_4ms			250 //Hz

/****************************************************************
						    Structures
****************************************************************/

/****************************************************************
						Global Declarations
****************************************************************/
//                                               Exported Globals
extern volatile uint32_t	F_CPU;			
extern volatile uint8_t		buff_index;
extern volatile char		RX_BUFFER[BUF_UART_RX_LEN];
extern volatile char		RX_CHAR;
extern volatile uint32_t	int_timer;

//                                               Imported Globals
extern volatile uint8_t currentConfigurationDescriptor;


/****************************************************************
						   Function Prototypes
****************************************************************/



#endif /* MAIN_H_ */