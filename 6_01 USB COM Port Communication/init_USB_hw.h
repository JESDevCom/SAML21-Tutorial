/*
 * init_USB.h
 *
 * Created: 12/19/2020 4:04:03 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 * 
 * Descriptions:
 *		Header file for init_USB_hw.c and init_USB_ports.c
 */ 


#ifndef INIT_USB_H_
#define INIT_USB_H_

/****************************************************************
						   Header Files
****************************************************************/
#include <stdio.h>
#include <stdalign.h>
#include "saml21j18b.h"	
#include "sys_def.h"				// Access PORT Name Definitions
#include "sys_calib.h"				// Access calibration values & serial number  
#include "sys_ascii.h"				// Access ZERO_CHAR, A_CHAR

#include "usb_descriptors_ep.h"		// Access USB_EP_mem
#include "usb_endpoints.h"          // Access erase_EndpointDescriptors
#include "sys_usb_std_def.h"        // Access ConfigState definitions



/****************************************************************
						   Definitions
****************************************************************/
#define USB_DN		PORT_PA24
#define USB_DP		PORT_PA25
#define USB_VBUS	PORT_PA12
#define USB_ID		PORT_PB02

// Serial number comes from four 32-bit values, so 128 bits.
// We need to know how many hex value will be required to store all
// 128-bits plus a byte for a Null character.
#define SERIAL_NUM_LENGTH				((32 * 4) / 4) + 1

/****************************************************************
						  Structures
****************************************************************/


/****************************************************************
						Global Declarations
****************************************************************/
extern volatile uint8_t USB_CTRL_I[64];
extern volatile uint8_t USB_CTRL_O[64];
extern char USB_SERIAL_NUMBER[SERIAL_NUM_LENGTH];

extern volatile uint8_t	currentConfigurationDescriptor;
extern volatile uint8_t	ConfigState;	

/****************************************************************
						   Function Prototypes
****************************************************************/
void usb_hw_init(void);
void enable_USB(void);
void disable_USB(void);
void attach_USB_to_host(void);
void detach_USB_from_host(void);




#endif /* INIT_USB_H_ */