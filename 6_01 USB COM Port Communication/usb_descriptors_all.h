/*
 * usb_descriptors_all.h
 *
 * Created: 12/27/2020 1:18:33 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Header files used to group the descriptors for this project that we have set values for.
 */ 


#ifndef USB_DESCRIPTORS_ALL_H_
#define USB_DESCRIPTORS_ALL_H_

/**************************************************************
						  Header Files 
**************************************************************/
#include "saml21j18b.h"	
#include "sys_usb_std_def.h"		// Access Standard Definitions
#include "sys_usb_cdc_def.h"		// Access CDC Definitions
#include "usb_descriptors_std.h"	// Access STD Descriptor Structs
#include "usb_descriptors_cdc.h"	// Access CDC Descriptor Structs
#include "init_USB_hw.h"			// Access global variable SERIAL NUMBER

/**************************************************************
						   Definitions
**************************************************************/
	
/**************************************************************
						   Structures
**************************************************************/
/***** CDC Abstract Control Model (ACM) Class Descriptor *******/
typedef struct __attribute__ ((packed)){	
	
/*  Memory Layout in accordance to USB2.0 Chapter 9
	Configuration Descriptor =>
		Communication Interface =>
			Communication Endpoint (Interrupt)
			Functional Descriptors 
		Data Interface =>
			Data In 
			Data Out */
	
	USB_CONFIG_DESCRIPTORS_TYPE							reg_cdc_config;

	//USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE			reg_cdc_iad;		/* Interface Association Descriptor */

	USB_INTERFACE_DESCRIPTORS_TYPE						reg_cdc_comm;		/* Communication Class Interface Descriptor Requirement */
	USB_CDC_HEADER_FUNCTIONAL_DESCRIPTOR_TYPE			reg_cdc_header;		/* Header Functional Descriptor */
	USB_CDC_ACM_FUNCTIONAL_DESCRIPTOR_TYPE				reg_cdc_acm;		/* ACM Functional Descriptor */
	USB_CDC_CALL_MANAGEMENT_FUNCTIONAL_DESCRIPTOR_TYPE	reg_cdc_mgmt;		/* Call Management Functional Descriptor */
	USB_CDC_UNION_FUNCTIONAL_DESCRIPTOR_TYPE			reg_cdc_union;		/* Union Functional Descriptor */
	USB_ENDPOINT_DESCRIPTORS_TYPE						reg_ep_comm;		/* Endpoint Descriptor */
	
	USB_INTERFACE_DESCRIPTORS_TYPE						reg_cdc_data;		/* Data Class Interface Requirement */
	USB_ENDPOINT_DESCRIPTORS_TYPE						reg_cdc_data_in;	/* Endpoint Descriptor */
	USB_ENDPOINT_DESCRIPTORS_TYPE						reg_cdc_data_out;	/* Endpoint Descriptor */
}CDCDescriptor_t;

/**************************************************************
						    Globals
**************************************************************/
extern const USB_DEVICE_DESCRIPTORS_TYPE			reg_device;
extern const CDCDescriptor_t						reg_cdc;
extern const USB_STRING_DESCRIPTORS_UNICODE_TYPE	reg_string_uni;
extern const USB_CDC_LINECODING_TYPE				reg_line_coding;
extern const char *usb_strings[];

/**************************************************************
						Function Prototypes
**************************************************************/




#endif /* USB_DESCRIPTORS_ALL_H_ */