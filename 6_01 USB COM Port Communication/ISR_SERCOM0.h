/*
 * ISR_UART0.h
 *
 * Created: 1/1/2021 5:14:15 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Header file for sercom0 interrupt vector.
 */ 


#ifndef ISR_UART0_H_
#define ISR_UART0_H_

/****************************************************************
						  Header Files
****************************************************************/
#include "saml21j18b.h"				// Access SERCOM0_Handler()
#include "sys_usb_cdc_def.h"		// Access CDC Definitions
#include "usb_cdc.h"                // Access USB_CDC_SERIALSTATE_()
#include "usb_uart.h"				// Access uart_buf_type
/****************************************************************
						   Definitions
****************************************************************/

/****************************************************************
						   Structures
****************************************************************/
/****************************************************************
						     Globals
****************************************************************/
extern volatile uart_buf_type sercom0_rx_buffer;
extern volatile uart_buf_type sercom0_tx_buffer;

/****************************************************************
						Function Prototypes
****************************************************************/

#endif /* ISR_UART0_H_ */