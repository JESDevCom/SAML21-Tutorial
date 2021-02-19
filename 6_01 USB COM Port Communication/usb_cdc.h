/*
 * usb_cdc.h
 *
 * Created: 1/3/2021 1:44:29 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Header file for CDC protocol functions.
 *
 */ 


#ifndef USB_CDC_H_
#define USB_CDC_H_

/****************************************************************
						  Header Files
****************************************************************/
#include <stdio.h>
#include <stdalign.h>				// Access demand library
#include <string.h>					// Access string library

#include "saml21j18b.h"	
#include "sys_usb_cdc_def.h"		// Access CDC definitions
#include "usb_descriptors_all.h"    // Access USB_CDC_LINECODING_TYPE
#include "usb_descriptors_ep.h"		// Access USB_EP_mem

#include "usb_std.h"
#include "usb_utils.h"				// Access USB send/received Functions
#include "usb_uart.h"               // Access Uart TX/RX Functions

/****************************************************************
						   Definitions
****************************************************************/
#define SET													0x01
#define CLR													0x00
#define CLEAR												0x00

// NOTE: Set both rising and falling edge times to 0 to disable edge detection.
#define STATUS_INACTIVE_STATE  0 // 0 - Low, 1 - High, 2 - Hi-Z
#define STATUS_ACTIVE_STATE    1 // 0 - Low, 1 - High, 2 - Hi-Z
#define STATUS_RISING_EDGE     0 // ms
#define STATUS_FALLING_EDGE    0 // ms
#define USART0_TIMEOUT		  10 // ms					
/****************************************************************
						   Structures
****************************************************************/
/* Structure used to format the send & receive buffer to handle
   the data from the communication endpoints */
typedef struct {
	
	uint32_t indx;       /* Index or current position */		 
	uint32_t size;       /* Size of the buffer        */
		
	struct{
		uint8_t FREE:1;  /*!< bit:      0  Boolean Buffer FREE      */
		uint8_t BUSY:1;      /*!< bit:      1  Boolean Buffer Busy      */
		uint8_t ZLP:1;		 /*!< bit:      2  Boolean Empty            */
		uint8_t :7;         /*!< bit: 3 .. 7  Reserved                 */
	}status;

	uint8_t buffer[SERCOM0_BUF_LEN]; /* Buffer of data */
}cdc_data_t;

/****************************************************************
						    Globals
****************************************************************/
//                                         ++ Imported Globals ++
extern volatile uint32_t F_CPU;					  // from main.c
extern volatile uart_buf_type sercom0_rx_buffer;  // from usb_uart.c
extern volatile uart_buf_type sercom0_tx_buffer;  // from usb_uart.c

extern const USB_CDC_LINECODING_TYPE reg_line_coding; // from usb_descriptors_all.c
extern volatile cdc_data_t reg_cdc_data_OUT; // EP_OUT
extern volatile cdc_data_t reg_cdc_data_IN;  // EP_IN


/****************************************************************
						 Function Prototypes
****************************************************************/
void usb_cdc_init(void);
void usb_cdc_send(uint8_t *data, int size);
void usb_cdc_recv(uint8_t *data, int size);
void USB_CDC_SERIALSTATE_(uint8_t setclear, uint8_t lineStatus);

void send_cdc_notification(void);
void init_CDC_CommEndpoint(uint32_t size);
void init_CDC_DataINEndpoint(uint32_t size);
void init_CDC_DataOUTEndpoint(uint32_t size);

void sys_time_init(void);
void sys_time_task(void);
uint32_t get_system_time(void);
void set_status_state(uint16_t lineStatus);
void usb_set_vcom_linestate(uint16_t wValue);
void status_task(void);
void usb_cdc_send_callback(void);
void buffer_send(uint8_t endpoint_num);
void usb_cdc_recv_callback(uint32_t size);
void usb_configuration_callback(void);
void SendDataViaUART_RecvFromHost(void);
void SendToHost_DataReceivedViaUart(void);
void uart_timer_task(void);
void SendStringToHost(char *string);
void usb_cdc_control_line_state_update(uint32_t linestate);

#endif /* USB_CDC_H_ */