/*
 * usb_descriptors_cdc.h
 *
 * Created: 12/27/2020 11:40:09 AM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Contains all the Functional Descriptors required for CDC ACM 
 *		communication over USB2.0
 *
 * Notes:
 *
 */ 


#ifndef USB_CDC_DESCRIPTORS_H_
#define USB_CDC_DESCRIPTORS_H_

/****************************************************************
						    Header Files
****************************************************************/ 
#include "saml21j18b.h"	
#include "usb_descriptors_std.h"  // Access to USB_REQUEST_TYPE

/****************************************************************
						    Definitions
****************************************************************/ 

/****************************************************************
						    Structures
****************************************************************/ 
// Communication Class Header Functional Descriptor (Pg. 21) [5]
typedef struct __attribute__ ((packed)){
	uint8_t		bFunctionLength;
	uint8_t		bDescriptorType;
	uint8_t		bDescriptorSubtype;
	uint16_t	bcdCDC;
}USB_CDC_HEADER_FUNCTIONAL_DESCRIPTOR_TYPE;



//  Communication Class Union Functional Descriptor (Pg. 22) [5]
typedef struct __attribute__ ((packed)){
	uint8_t		bFunctionLength;
	uint8_t		bDescriptorType;
	uint8_t		bDescriptorSubtype;
	uint8_t		bControlInterface;
	uint8_t		bSubordinateInterface0;
}USB_CDC_UNION_FUNCTIONAL_DESCRIPTOR_TYPE;



// Call Management Functional Descriptor (Pg. 22) [5]
typedef struct __attribute__ ((packed)){
	uint8_t		bFunctionLength;
	uint8_t		bDescriptorType;
	uint8_t		bDescriptorSubtype;
	uint8_t		bmCapabilities;
	uint8_t		bDataInterface;
}USB_CDC_CALL_MANAGEMENT_FUNCTIONAL_DESCRIPTOR_TYPE;



//  ACM Management Functional Descriptor (pg. 21) [5]
typedef struct __attribute__ ((packed)){
	uint8_t		bFunctinLength;
	uint8_t		bDescriptorType;
	uint8_t		bDescriptorSubtype;
	uint8_t		bmCapabilities;
}USB_CDC_ACM_FUNCTIONAL_DESCRIPTOR_TYPE;



// Line Coding Sets Up: Baud/StopBit/Parity/Length (Pg. 2) [7]
typedef struct __attribute__ ((packed)){
	uint32_t	deDTERate;
	uint8_t		bCharFormat;
	uint8_t		bParityType;
	uint8_t		bDataBits;
}USB_CDC_LINECODING_TYPE;



typedef struct __attribute__ ((packed)){
	USB_REQUEST_TYPE	request;
	uint16_t			data;
}USB_CDC_REQUEST_TYPE;


/****************************************************************
						    Globals
****************************************************************/ 
/****************************************************************
						Function Prototypes
****************************************************************/ 

#endif /* USB_CDC_DESCRIPTORS_H_ */