/*
 * usb_utils.h
 *
 * Created: 12/27/2020 11:25:29 AM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Header file for usb_utils.c which contains simple USB RX and TX 
 *		functions.
 */ 


#ifndef USB_UTILS_H_
#define USB_UTILS_H_

/**************************************************************
						  Header Files 
**************************************************************/
#include "saml21j18b.h"	
#include "sys_usb_macros.h"			// Access USB specific Macros
#include "usb_descriptors_all.h"	// Access reg_device Struct
#include "usb_descriptors_ep.h"		// Access USB_EP_mem

/**************************************************************
						   Definitions
**************************************************************/
/**************************************************************
						   Structures
**************************************************************/
/**************************************************************
						    Globals
**************************************************************/
//extern volatile mem_DEVICE_DESC_BANK_type USB_EP_mem[8];
extern volatile uint8_t USB_CTRL_I[64];
extern volatile uint8_t USB_CTRL_O[64];

/**************************************************************
						Function Prototypes
**************************************************************/
void usb_control_send(uint8_t *packet, uint32_t length);

void usb_control_stall(void);

void usb_control_send_zero_length_packet(void);

void EndpointIN_DeviceSend(uint8_t ep_num, volatile uint8_t *data, uint32_t length);

void EndpointOUT_DeviceRecv(uint8_t ep_num, volatile uint8_t *data, uint32_t length);

void usb_set_address(uint8_t address);


#endif /* USB_UTILS_H_ */