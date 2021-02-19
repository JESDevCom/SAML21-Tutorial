/*
 * usb_descriptors_ep.h
 *
 * Created: 12/31/2020 5:10:37 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Header file used to contain the endpoint memory structure as 
 *		defined on page 860 of the data sheet [1].
 *
 * Resources
 *		[1] SAM L21 Family Data Sheet
 *
 */ 


#ifndef USB_DESCRIPTORS_EP_H_
#define USB_DESCRIPTORS_EP_H_

/****************************************************************
						   Header Files
****************************************************************/
#include "saml21.h"
/****************************************************************
						   Definitions
****************************************************************/
/****************************************************************
						  Structures
****************************************************************/
typedef union{
	UsbDeviceDescBank	bank[2];
	struct{
		UsbDeviceDescBank out;
		UsbDeviceDescBank in;
	};
}mem_DEVICE_DESC_BANK_type;

/****************************************************************
						Global Declarations
****************************************************************/
extern volatile mem_DEVICE_DESC_BANK_type USB_EP_mem[8];

/****************************************************************
						   Function Prototypes
****************************************************************/



#endif /* USB_DESCRIPTORS_EP_H_ */