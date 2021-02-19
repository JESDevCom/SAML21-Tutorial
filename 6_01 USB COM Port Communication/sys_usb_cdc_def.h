/*
 * sys_usb_cdc.h
 *
 * Created: 12/23/2020 3:26:47 PM
 *  Author: John
 * 
 * Description:
 *		Definitions and structures associated with Communication Class/Abstract Control Model (ACM) interface
 *		with a notification endpoint. We use this communication protocol to communicate to a Serial
 *		terminal like PuTTy on the Host side. Definitions and structs were created abiding by
 *		USB-IF WMC110_errate manual.
 *
 * Resources:
 *		[1]	SAM L21 Family Data Sheet
 *		[2] Atmel L21 Xplained Pro User Guide
 *		[5] WMC110_errata.pdf
 *		[6] CDC120_errata.pdf
 *		[7] https://www.silabs.com/documents/public/application-notes/AN758.pdf
 */ 


#ifndef CDC_DESCRIPTORS_H_
#define CDC_DESCRIPTORS_H_

#include "saml21j18b.h"	
#include "usb_descriptors_std.h"

/****************************************************************
						   Header Files
****************************************************************/

/****************************************************************
						    Definitions
****************************************************************/
#define USB_CLASS_CODE_CDC									0x02 // (pg. 12) [6]
#define USB_CLASS_CODE_COM_CDC	/* Comm Class Interface */	0x02 // (pg. 12) [6]
#define USB_CLASS_CODE_DATA_CDC	/* Data Class Interface */	0x0A // (pg. 13) [6]

// Single Composite CDC Device
#define USB_CONFIGURATION_CTRL								0
#define USB_CONFIGURATION_CDC								1

#define USB_INTERFACE_CTRL									0x00
#define USB_INTERFACE_CDC_COMM								0x00
#define USB_INTERFACE_CDC_DATA								0x01
#define USB_DEVICE_MAX_INTERFACE							3	 // Total Number of Interfaces Used

#define USB_EP_DIR_IN										0x80
#define USB_EP_DIR_OUT										0x00

#define USB_EP_3_CDC_COMM_IN								0x83 // Notification Endpoint
#define USB_EP_3_CDC_COMM								    0x03 // Notification Endpoint
#define USB_EP_1_CDC_DATA									0x01 // Host's Receival Endpoint
#define USB_EP_1_CDC_DATA_IN								0x81 
#define USB_EP_2_CDC_DATA									0x02 // Host's Send Endpoint
#define USB_EP_2_CDC_DATA_OUT								0x02 

#define USB_EP_0_CTRL										0x00 // Control Endpoint
#define USB_EP_0_CTRL_IN									0x80 // Control Endpoint w/ Direction
#define USB_EP_0_CTRL_OUT									0x00 // Control Endpoint w/ Direction
#define USB_DEVICE_MAX_EP									5    // Total Number of EP's Used

// Communication Interface Class (CIC) Management Request Codes (Pg. 21) [6]
// Class Request use standard request packet.
#define USB_CDC_SEND_ENCAPSULATED_COMMAND					0x00 /* Required for CDC ACM */
#define USB_CDC_GET_ENCAPSULATED_RESPONSE					0x01 /* Required for CDC ACM */
#define USB_CDC_SET_COMM_FEATURE							0x02 /* Required for CDC ACM */
#define USB_CDC_GET_COMM_FEATURE							0x03 /* Required for CDC ACM */
#define USB_CDC_CLEAR_COMM_FEATURE							0x04 /* Required for CDC ACM */
#define USB_CDC_RESET_FUNCTION								0x05

#define USB_CDC_SET_AUX_LINE_STATE							0x10
#define USB_CDC_SET_HOOK_STATE								0x11
#define USB_CDC_PULSE_SETUP									0x12
#define USB_CDC_SEND_PULSE									0x13
#define USB_CDC_SET_PULSE_TIME								0x14
#define USB_CDC_RING_AUX_JACK								0x15

#define USB_CDC_SET_LINE_CODING								0x20 /* Required for CDC ACM */
#define USB_CDC_GET_LINE_CODING								0x21 /* Required for CDC ACM */
#define USB_CDC_SET_LINE_STATE								0x22 /* Required for CDC ACM */
#define USB_CDC_SEND_BREAK									0x23 /* Required for CDC ACM */

#define USB_CDC_SET_RINGER_PARAMS							0x30
#define USB_CDC_GET_RINGER_PARAMS							0x31
#define USB_CDC_SET_OPERATION_PARAMS						0x32
#define USB_CDC_GET_OPERATION_PARAMS						0x33
#define USB_CDC_SET_LINE_PARAMS								0x34
#define USB_CDC_GET_LINE_PARAMS								0x35
#define USB_CDC_DIAL_DIGITS									0x36
#define USB_CDC_SET_UNIT_PARAMETER							0x37
#define USB_CDC_GET_UNIT_PARAMETER							0x38
#define USB_CDC_CLEAR_UNIT_PARAMETER						0x39
#define USB_CDC_GET_PROFILE									0x3A

#define USB_CDC_SET_ATM_DATA_FORMAT							0x50
#define USB_CDC_GET_ATM_DEVICE_STATISTICS					0x51
#define USB_CDC_SET_ATM_DEFAULT_VC							0x52
#define USB_CDC_GET_ATM_VC_STATISTICS						0x53

// Communication Interface Class (CIC) Notifications Codes (Pg. 23) [6]
#define USB_CDC_CIC_NETWORK_CONNECTION						0x00 /* Required for CDC ACM */
#define USB_CDC_CIC_RESPONSE_AVAILABLE						0x01 /* Required for CDC ACM */
#define USB_CDC_CIC_AUX_JACK_HOOK_STATE						0x08
#define USB_CDC_CIC_RING_DETECT								0x09
#define USB_CDC_CIC_SERIAL_STATE							0x20 /* Required for CDC ACM */
#define USB_CDC_CIC_CALL_STATE_CHANGE						0x28
#define	USB_CDC_CIC_LINE_STATE_CHANGE						0x29
#define USB_CDC_CIC_CONNECTION_SPEED_CHANGE					0x2A



/* +++++++++++++++++++ DESCRIPTOR: CDC Header ++++++++++++++++ */
// -------------------------------------------------------------- bFunctionLength
#define USB_DESCRIPTOR_HEADER_bFunctionLength	/* bytes */	0x05
// -------------------------------------------------------------- bDescriptorType
#define USB_DESCRIPTOR_HEADER_bDescriptorType_Interface		36
#define USB_DESCRIPTOR_HEADER_bDescriptorType_Endpoint		37
// -------------------------------------------------------------- bDescriptorSubtype
#define USB_DESCRIPTOR_HEADER_bDescriptorSubtype_Header		0
// -------------------------------------------------------------- bcdCDC
#define USB_DESCRIPTOR_HEADER_bcdCDC						0x0110



/* +++++++++++++++++++ DESCRIPTOR: CDC Union +++++++++++++++++ */
// -------------------------------------------------------------- bFunctionLength
#define USB_DESCRIPTOR_UNION_bFunctionLength				0x05
// -------------------------------------------------------------- bDescriptorType
#define USB_DESCRIPTOR_UNION_bDescriptorType_Interface		0x24 // 36
#define USB_DESCRIPTOR_UNION_bDescriptorType_Endpoint		0x25 // 37
// -------------------------------------------------------------- bDescriptorSubtype
#define USB_DESCRIPTOR_UNION_bDescriptorSubtype_Union		6
// -------------------------------------------------------------- bControlInterface
#define USB_DESCRIPTOR_UNION_bControlInterface_Class_Data	0x02
#define USB_DESCRIPTOR_UNION_bControlInterface_Class_Comm	0x0A
// -------------------------------------------------------------- bSubordinateInterface(n)
#define USB_DESCRIPTOR_UNION_bSubordinateInterface			0x01



/* +++++++++++++++ DESCRIPTOR: CDC Call Management +++++++++++ */
// -------------------------------------------------------------- bFunctionLength
#define USB_DESCRIPTOR_CALL_MAN_bFunctionLength				0x05
// -------------------------------------------------------------- bDescriptorType
#define USB_DESCRIPTOR_CALL_MAN_bDescriptorType_Interface	0x24  // 36
#define USB_DESCRIPTOR_CALL_MAN_bDescriptorType_Endpoint	0x25  // 37
// -------------------------------------------------------------- bDescriptorSubtype
#define USB_DESCRIPTOR_CALL_MAN_bDescriptorSubtype_Call_Management 1
// -------------------------------------------------------------- bmCapabilities
#define USB_DESCRIPTOR_CALL_MAN_bmCapabilities_Call_Management_Not_Supported	0x00 // Device sends/receives call management information over the Communication Class Interface. (Pg. 22) [5]
#define USB_DESCRIPTOR_CALL_MAN_bmCapabilities_Call_Management_Supported		0x01 // Device can send/receive call management information over a Data Class Interface. (Pg. 22) [5]
#define USB_DESCRIPTOR_CALL_MAN_bmCapabilities_Call_Management_Via_CCI			0x00 // Function sends/receives call management information only over this Communication Class Interface (CCI) (Pg. 22) [5]
#define USB_DESCRIPTOR_CALL_MAN_bmCapabilities_Call_Management_Via_DCI			0x02 // Function can send/receive call management info over the Data Class Interface (DCI)
// -------------------------------------------------------------- bDataInterface
#define USB_DESCRIPTOR_CALL_MAN_bDataInterface				0x01			



/* +++++++++++++++ DESCRIPTOR: CDC ACM Management +++++++++++++ */
// -------------------------------------------------------------- bFunctionLength
#define USB_DESCRIPTOR_ACM_bFunctionLength				0x04
// -------------------------------------------------------------- bDescriptorType
#define USB_DESCRIPTOR_ACM_bDescriptorType_Interface	0x24	  // 36			
// -------------------------------------------------------------- bDescriptorSubtype
#define USB_DESCRIPTOR_ACM_bDescriptorSubtype_ACM		2
// -------------------------------------------------------------- bmCapabilities
#define USB_DESCRIPTOR_ACM_bmCapabilities_All_Supported	0x0F //	  (Pg. 21, 20) [5, 6]
#define USB_DESCRIPTOR_ACM_bmCapabilities_Standard		0x06
#define USB_DESCRIPTOR_ACM_bmCapabilities_LineRequestOnly 0x02
// -------------------------------------------------------------- bDataInterface
#define USB_DESCRIPTOR_ACM_bDataInterface(value)		0xFF & (value)


/* ++++++++++++++++++ DESCRIPTOR: Line Coding ++++++++++++++++  */  //(Pg. 3) [7]
// -------------------------------------------------------------- dwDTERate
#define USB_CDC_LINECOODING_deDTERate_115200 /*baud*/	115200
// -------------------------------------------------------------- bCharFormat
#define USB_CDC_LINECODING_bCharFormat_1StopBit			0x00
#define USB_CDC_LINECODING_bCharFormat_2StopBit			0x03
// -------------------------------------------------------------- bParityType
#define USB_CDC_LINECODING_bParityType_None				0x00
#define USB_CDC_LINECODING_bParityType_Odd				0x01
#define USB_CDC_LINECODING_bParityType_Even				0x02
#define USB_CDC_LINECODING_bParityType_Mark				0x03
#define USB_CDC_LINECODING_bParityType_Space			0x04
// --------------------------------------------------------------- bDataBits
#define USB_CDC_LINECODING_bDataBits_Width_16			0x10
#define USB_CDC_LINECODING_bDataBits_Width_8			0x08
#define USB_CDC_LINECODING_bDataBits_Width_7			0x07
#define USB_CDC_LINECODING_bDataBits_Width_6			0x06
#define USB_CDC_LINECODING_bDataBits_Width_5			0x05



/* +++++++++++++++++ Descriptor: Set_Line_Coding +++++++++++++++ */
// ---------------------------------------------------------------- bmRequestType
#define USB_CDC_SETLINECODING_bmRequestType				0x21
// ---------------------------------------------------------------- bRequest
#define USB_CDC_SETLINECODING_bRequest					USB_CDC_SET_LINE_CODING
// ---------------------------------------------------------------- wValue
#define USB_CDC_SETLINECODING_wValue					0x00
// ---------------------------------------------------------------- wIndex
// Interface number
// ---------------------------------------------------------------- wLength
// Size of Structure being returned
// ---------------------------------------------------------------- Data
// Line coding structure



/* +++++++++++++++++ Descriptor: Get_Line_Coding +++++++++++++++ */
// ---------------------------------------------------------------- bmRequestType
#define USB_CDC_GETLINECODING_bmRequestType				0xA1
// ---------------------------------------------------------------- bRequest
#define USB_CDC_GETLINECODING_bRequest					USB_CDC_GET_LINE_CODING 
// ---------------------------------------------------------------- wValue
#define USB_CDC_GETLINECODING_wVaue						0x00
// ---------------------------------------------------------------- wIndex
// Interface number
// ---------------------------------------------------------------- wLength
// Size of Structure being returned
// ---------------------------------------------------------------- Data
// Line coding Structure



/* +++++++++++++ Descriptor: Set_Control_Line_State ++++++++++++ */
// ---------------------------------------------------------------- bmRequestType
#define USB_CDC_LINESTATE_bmRequestType					0x21
// ---------------------------------------------------------------- bRequest
#define USB_CDC_LINESTATE_bRequest						USB_CDC_CIC_LINE_STATE_CHANGE /* This request generates RS-232 style control signals. */
// ---------------------------------------------------------------- WValue
#define USB_CDC_LINESTATE_wValue_ActivateCarrier		0x0002 /* Carrier control for half duplexed modems. This signal corresponds to V.24 signal 105 and RS232 signal RTS. The device ignores the value of this bit when operating in full duplexed mode*/
#define USB_CDC_LINESTATE_wValue_DeactivateCarrier		0x0000
#define USB_CDC_LINESTATE_wValue_DTEpresent				0x0001 /* Indicates to DCE if DTE is present or not. This signal corresponds to V.24 signal 108/2 and RS232 signal DTR */
#define USB_CDC_LINESTATE_wValue_noDTEpresent			0x0000
// ---------------------------------------------------------------- wIndex
// Interface number										
// ---------------------------------------------------------------- wLength
#define USB_CDC_LINESTATE_wLength						0	
// ---------------------------------------------------------------- Data
// None



/* ++++++++==+++++++++ Descriptor: Serial_State ++++++++++++++++ */
// ---------------------------------------------------------------- bmRequestType
#define USB_CDC_SERIALSTATE_bmRequestType				0xA1
// ---------------------------------------------------------------- bRequest
#define USB_CDC_SERIALSTATE_bRequest					USB_CDC_CIC_SERIAL_STATE /* Returns the current state of the carrier detects, DSR, break, and Ring Signal */
// ---------------------------------------------------------------- wValue
#define USB_CDC_SERIALSTATE_wValue						0x00
// ---------------------------------------------------------------- wIndex
// Interface number
// ---------------------------------------------------------------- wLength
#define USB_CDC_SERIALSTATE_wLength						2	
// ---------------------------------------------------------------- Data
#define bOverRun	(1<<6) // Received data has been discarded due to overrun in the device
#define bParity		(1<<5) // A parity error occurred
#define bFraming	(1<<4) // A framing error occurred
#define bRingSignal	(1<<3) // State of ring signal detection of the device
#define bBreak		(1<<2) // State of break detection mechanism of the device
#define bTxCarrier  (1<<1) // State of transmission carrier. (RS232 DSR)
#define bRxCarrier	(1<<0) // State of Receiver carrier detection mechanism of device. (RS232 DCD)




/****************************************************************
						    Structures
****************************************************************/ 

/****************************************************************
						    Globals
****************************************************************/ 

/****************************************************************
						Function Prototypes
****************************************************************/ 




#endif /* CDC_DESCRIPTORS_H_ */