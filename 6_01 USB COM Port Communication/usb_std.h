/*
 * usb_request.h
 *
 * Created: 12/27/2020 2:10:11 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *     Header file for usb_request.c
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

/**************************************************************
						  Header Files 
**************************************************************/
#include "saml21j18b.h"	
#include "sys_usb_std_def.h"		// Access Standard Definitions
#include "sys_usb_cdc_def.h"		// Access CDC Definitions
#include "sys_usb_macros.h"			// Access USB Specific Macros

#include "usb_descriptors_all.h"	// Access Defined Global Structs
#include "usb_descriptors_ep.h"		// Access USB_EP_mem 
#include "usb_utils.h"				// Access basic packet RX & TX
#include "usb_uart.h"				// Access USB UART Functions
#include "usb_cdc.h"				// Access usb_set_vcom_linestate


/**************************************************************
						  Definitions
**************************************************************/
//------------------------------------------------------------- Request Status
#define stall											0
#define valid											1
#define undefined										2





// =================== REQUEST DEFINITIONS ====================
// ------------------------------------------------------------ GetStatus()
#define GetStatus_Device_WakeupEnabled					0x0002
#define GetStatus_Device_WakeupDisabled					0x0000
#define GetStatus_Device_SelfPowered					0x0001
#define GetStatus_Device_BusPowered						0x0000
#define GetStatus_Interface								0x0000
#define GetStatus_Endpoint_HaltEnabled					0x0001
#define GetStatus_Endpoint_HaltDisabled					0x0000

// ------------------------------------------------------------- SetFeature()
#define SetFeature_FeatureSel_EndpointHalt				0
#define SetFeature_FeatureSel_DeviceRemoteWakeup		1
#define SetFeature_FeatureSel_TestMode					2
#define SetFeature_TestSel_Reserved						0x00
#define SetFeature_TestSel_Test_J						0x01
#define SetFeature_TestSel_Test_K						0x02
#define SetFeature_TestSel_Test_TestSE0_NAK				0x03
#define SetFeature_TestSel_Test_Packet					0x04
#define SetFeature_TestSel_Test_Force_Enable			0x05

/**************************************************************
						   Structures
**************************************************************/
/**************************************************************
						    Globals
**************************************************************/
extern uint8_t  Set_Line_Coding_Flag;
//extern uint16_t Set_Line_Coding_Length;

extern volatile uint8_t	currentConfigurationDescriptor;
extern volatile uint8_t	ConfigState;

/**************************************************************
						Function Prototypes
**************************************************************/
uint8_t usb_requests(USB_REQUEST_TYPE *request_msg);
void usb_init(void);
/*
void usb_set_callback(int ep, void (*callback)(uint32_t size));
bool usb_class_handle_request(USB_REQUEST_TYPE *request);
void usb_send_callback(int ep)
void usb_recv_callback(int ep, int size);
*/


#endif /* INCFILE1_H_ */