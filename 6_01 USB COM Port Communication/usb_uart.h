/*
 * usb_uart.h
 *
 * Created: 1/1/2021 2:41:32 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *	
 */ 


#ifndef INIT_USB_UART_H_
#define INIT_USB_UART_H_

/**************************************************************
						  Header Files 
**************************************************************/
#include "saml21j18b.h"	
#include "sys_def.h"				// Access Port Definitions
#include "sys_usb_cdc_def.h"        // Access definitions
#include "usb_descriptors_cdc.h"	// Access USB_CDC_LINECODING_TYPE struct


/**************************************************************
						   Definitions
**************************************************************/
#define SERCOM_BUFFER_TXC							(1<<0)
#define SERCOM_BUFFER_RXC							(1<<1)
#define SERCOM_BUFFER_BUSY				            (1<<2)

/**************************************************************
						   Structures
**************************************************************/
typedef struct{
	uint32_t	write;
	uint32_t	read;
	
	struct{
		uint8_t TXC:1;  /*!< bit:      0  Boolean Transmit Complete     */
		uint8_t RXC:1;  /*!< bit:	   1  Boolean Receival Complete     */
		uint8_t BUSY:1; /*!< bit:	   2  Boolean Read/Write in Process */
		uint8_t :5;		/*!< bit:  3.. 7  Reserved                      */
	}status;
	
	uint8_t data[SERCOM0_BUF_LEN];
}uart_buf_type;

/**************************************************************
						    Globals
**************************************************************/
//                                       ++ Exported Globals ++
extern volatile uart_buf_type sercom0_rx_buffer;
extern volatile uart_buf_type sercom0_tx_buffer;

//                                       ++ Imported Globals ++
extern volatile uint32_t F_CPU;


/**************************************************************
						Function Prototypes
**************************************************************/
void usb_setup_vcom(const USB_CDC_LINECODING_TYPE *linecoding);
uint8_t read_from_uart_rx_buffer(uint8_t *byte);
uint8_t write_to_uart_tx_buffer(uint8_t byte);

#endif /* INIT_USB_UART_H_ */