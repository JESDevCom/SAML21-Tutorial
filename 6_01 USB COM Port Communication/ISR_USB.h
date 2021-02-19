/*
 * ISR_USB.h
 *
 * Created: 12/21/2020 3:32:57 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Header file for USB interrupt vector.
 */ 


#ifndef ISR_USB_H_
#define ISR_USB_H_

/****************************************************************
						   Header Files
****************************************************************/
#include "saml21j18b.h"					
#include "sys_usb_std_def.h"		// Access Standard Descriptor Definitions
#include "sys_usb_cdc_def.h"		// Access CDC Descriptor Definitions
#include "usb_descriptors_ep.h"		// Access USB_EP_mem
#include "usb_endpoints.h"			// Access Endpoint Functions
#include "usb_std.h"	            // Access usb_requests(), Set_Line_Coding_Flag
#include "usb_utils.h"				// Access usb_control_send_zero_length_packet()
#include "usb_cdc.h"				// Access reg_cdc_data_OUT & reg_cdc_data_IN

/****************************************************************
						   Definitions
****************************************************************/


/****************************************************************
						Global Declarations
****************************************************************/
//extern volatile mem_DEVICE_DESC_BANK_type USB_EP_mem[8];
extern volatile uint8_t USB_CTRL_I[64];
extern volatile uint8_t USB_CTRL_O[64];
extern volatile uint8_t	currentConfigurationDescriptor;
extern volatile uint8_t	ConfigState;

extern volatile cdc_data_t reg_cdc_data_OUT; 
extern volatile cdc_data_t reg_cdc_data_IN;  

/****************************************************************
						   Function Prototypes
****************************************************************/
void usb_flag_handler(void);
//void usb_control_recv(void (*callback)(uint8_t *data, uint32_t size));




#endif /* ISR_USB_H_ */