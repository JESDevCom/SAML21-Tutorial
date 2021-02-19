/*
 * usb_endpoints.h
 *
 * Created: 12/31/2020 1:43:34 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Header file for usb_endpoints.h
 */ 


#ifndef USB_ENDPOINTS_H_
#define USB_ENDPOINTS_H_

/**************************************************************
						  Header Files 
**************************************************************/
#include "saml21j18b.h"	
#include "sys_usb_std_def.h"		// Access Standard Definitions
#include "sys_usb_cdc_def.h"		// Access CDC Definitions
#include "usb_descriptors_std.h"	// Access Standard Descriptor Structs
#include "usb_descriptors_cdc.h"	// Access CDC Descriptor Structs
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

/**************************************************************
						Function Prototypes
**************************************************************/
void usb_initialize_endpoints(USB_ENDPOINT_DESCRIPTORS_TYPE *endpoint);
void erase_EndpointDescriptors(void);
void set_endpoint_type(uint8_t n_ep, uint8_t eptype0, uint8_t eptype1);
void set_all_endpoint_types(uint8_t OUT_eptype_Bank0, uint8_t IN_eptype_Bank1);
uint8_t set_endpoint_PCKSIZE_SIZE(uint16_t byte_len, uint8_t TransferType);
uint8_t set_endpoint_direction(uint8_t TransferType);
uint8_t get_endpoint_type(uint8_t endpoint_num, uint8_t endpoint_dir);
uint8_t is_endpoint_enabled(uint8_t endpoint_num, uint8_t endpoint_dir);
uint8_t is_endpoint_stalled(uint8_t endpoint_num, uint8_t endpoint_dir);
void endpoint_stall(uint8_t endpoint_num, uint8_t endpoint_dir);

#endif /* USB_ENDPOINTS_H_ */