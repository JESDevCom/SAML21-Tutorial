/*
 * sys_usb_std_def.h
 *
 * Created: 12/21/2020 3:04:47 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Contains definitions for configuring USB 2.0 Standard Descriptor Bit-Fields
 * 
 * Resources:
 *		[1] SAM L21 Family Data Sheet
 *		[2] SAM L21 XPlained Pro User Guide
 *		[3] USB 2.0 Manual: http://sdphca.ucsd.edu/lab_equip_manuals/usb_20.pdf
 *		[4] USB-IF: https://www.usb.org/defined-class-codes
 */ 


#ifndef SYS_USB_H_
#define SYS_USB_H_

/****************************************************************
						   Header Files
****************************************************************/
#include "saml21j18b.h"	

/****************************************************************
						    Definitions
****************************************************************/
#define NUM_ENDPOINTS						8

// -------------------------------------------------------------- request states [3]
#define default_state						0
#define address_state						1
#define configured_state					2

// -------------------------------------------------------------- Device Endpoint Config Register (Pg. 852) [1]
#define USB_DEVICE_EPCFG_EPTYPE_DISABLED	0
#define USB_DEVICE_EPCFG_EPTYPE_CONTROL		1
#define USB_DEVICE_EPCFG_EPTYPE_ISOCHRONOUS	2
#define USB_DEVICE_EPCFG_EPTYPE_BULK		3
#define USB_DEVICE_EPCFG_EPTYPE_INTERRUPT	4
#define USB_DEVICE_EPCFG_EPTYPE_DUALBANK	5

// -------------------------------------------------------------- [1]
#define USB_DEVICE_PCKSIZE_SIZE_8			0
#define USB_DEVICE_PCKSIZE_SIZE_16			1
#define USB_DEVICE_PCKSIZE_SIZE_32			2
#define USB_DEVICE_PCKSIZE_SIZE_64			3
#define USB_DEVICE_PCKSIZE_SIZE_128			4
#define USB_DEVICE_PCKSIZE_SIZE_256			5
#define USB_DEVICE_PCKSIZE_SIZE_512			6
#define USB_DEVICE_PCKSIZE_SIZE_1023		7

// -------------------------------------------------------------- table 9-4 Standard Request Codes (Pg. 251) [3]
#define USB_STD_REQUEST_CODE_GET_STATUS					0
#define USB_STD_REQUEST_CODE_CLEAR_FEATURE				1
#define USB_STD_REQUEST_CODE_SET_FEATURE				3
#define USB_STD_REQUEST_CODE_SET_ADDRESS				5
#define USB_STD_REQUEST_CODE_GET_DESCRIPTOR				6
#define USB_STD_REQUEST_CODE_SET_DESCRIPTOR				7
#define USB_STD_REQUEST_CODE_GET_CONFIGURATION			8
#define USB_STD_REQUEST_CODE_SET_CONFIGURATION			9
#define USB_STD_REQUEST_CODE_GET_INTERFACE				10
#define USB_STD_REQUEST_CODE_SET_INTERFACE				11
#define USB_STD_REQUEST_CODE_SYNCH_FRAME				12

// -------------------------------------------------------------- Table 9-5. Descriptor Types (Pg. 251) [3]
#define USB_DESCRIPTOR_TYPE_DEVICE						1
#define USB_DESCRIPTOR_TYPE_CONFIGURATION				2
#define USB_DESCRIPTOR_TYPE_STRING						3
#define USB_DESCRIPTOR_TYPE_INTERFACE					4
#define USB_DESCRIPTOR_TYPE_ENDPOINT					5
#define USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER			6
#define USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION	7
#define USB_DESCRIPTOR_TYPE_INTERFACE_POWER				8

// -------------------------------------------------------------- Table 9-6. Standard Feature Selectors (Pg. 252) [3]
#define USB_STD_FEATURE_DEVICE_REMOTE_WAKEUP			1
#define USB_STD_FEATURE_ENDPOINT_HALT					0
#define USB_STD_FEATURE_TEST_MODE						2

// -------------------------------------------------------------- Class Codes [4]
#define	USB_CLASS_CODE_MSC								0x08
#define USB_CLASS_CODE_CDC								0x02

/*=============================================================*/
/* +++++++++++++++++++ DESCRIPTOR: DEVICE ++++++++++++++++++++ */
/*=============================================================*/
// -------------------------------------------------------------- bLength
#define USB_DESCRIPTOR_DEVICE_bLength_Off					0
#define USB_DESCRIPTOR_DEVICE_bLength						0x12
// -------------------------------------------------------------- bDescriptorType
#define USB_DESCRIPTOR_DEVICE_bDescriptorType_Off			1
#define USB_DESCRIPTOR_DEVICE_bDescriptorType				0x01
// -------------------------------------------------------------- bcdUSB
#define USB_DESCRIPTOR_DEVICE_bcdUSB_Off					2
#define USB_DESCRIPTOR_DEVICE_bcdUSB_1dot0					0x0100
#define USB_DESCRIPTOR_DEVICE_bcdUSB_1dot1					0x0110
#define USB_DESCRIPTOR_DEVICE_bcdUSB_2dot0					0x0200
#define USB_DESCRIPTOR_DEVICE_bcdUSB_2dot1					0x0210
// -------------------------------------------------------------- bDeviceClass
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_Off				4
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_Audio			0x01
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_CDC				0x02
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_HID				0x03
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_Physical			0x05
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_Image			0x06
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_Printer			0x07
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_MassStorage		0x08
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_Hub_val			0x09
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_CDC_data			0x0A
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_SmartCard		0x0B
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_ContentSecurity	0x0D
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_Video			0x0E
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_PersonalHealth	0x0F
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_AudioVideo		0x10
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_Billboard		0x11
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_USBCBridge		0x12
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_DiagnostiC		0xDC
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_WirelessCNTLR	0xE0
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_MISC				0xEF
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_AppSpecific		0xFE
#define USB_DESCRIPTOR_DEVICE_bDeviceClass_VendorSpecific	0xFF
// -------------------------------------------------------------- bDeviceSubClass
#define USB_DESCRIPTOR_DEVICE_bDeviceSubClass_MSC_Off		5
#define USB_DESCRIPTOR_DEVICE_bDeviceSubClass_MSC			0x00
#define USB_DESCRIPTOR_DEVICE_bDeviceSubClass_CDC			0x00
#define USB_DESCRIPTOR_DEVICE_bDeviceSubClass_Image			0x01
#define USB_DESCRIPTOR_DEVICE_bDeviceSubClass_MISC			0x02
// -------------------------------------------------------------- bDeviceProtocol
#define USB_DESCRIPTOR_DEVICE_bDeviceProtocol_MSC_Off		6
#define USB_DESCRIPTOR_DEVICE_bDeviceProtocol_CDC			0x00
#define USB_DESCRIPTOR_DEVICE_bDeviceProtocol_MSC			0x00
#define USB_DESCRIPTOR_DEVICE_bDeviceProtocol_Image			0x01
#define USB_DESCRIPTOR_DEVICE_bDeviceProtocol_MISC			0x01
// -------------------------------------------------------------- bMaxPacketSize
#define USB_DESCRIPTOR_DEVICE_bMaxPacketSize_Off			7
#define USB_DESCRIPTOR_DEVICE_bMaxPacketSize_64				0x40
#define USB_DESCRIPTOR_DEVICE_bMaxPacketSize_32				0x20	
#define USB_DESCRIPTOR_DEVICE_bMaxPacketSize_16				0x10	
#define USB_DESCRIPTOR_DEVICE_bMaxPacketSize_8				0x08
// -------------------------------------------------------------- idVendor
#define USB_DESCRIPTOR_DEVICE_idVendor_Off					8
#define USB_DESCRIPTOR_DEVICE_idVendor_Atmel				0x03EB
#define USB_DESCRIPTOR_DEVICE_idVendor_Microchip			0x04D8
#define USB_DESCRIPTOR_DEVICE_idVendor_Prototype			0x6666	
// -------------------------------------------------------------- idProduct
#define USB_DESCRIPTOR_DEVICE_idProduct_Off					10
#define USB_DESCRIPTOR_DEVICE_idProduct_Atmel				0x2169	
#define USB_DESCRIPTOR_DEVICE_idProduct_Microchip_USB_SERIAL			0x000A
#define USB_DESCRIPTOR_DEVICE_idProduct_Microchip_CDC_RS323_Emulation	0x000A
#define USB_DESCRIPTOR_DEVICE_idProduct_Prototype			0x8888
// -------------------------------------------------------------- bcdDevice
#define USB_DESCRIPTOR_DEVICE_bcdDevice_Off					12
#define USB_DESCRIPTOR_DEVICE_bcdDevice_Ver_1_0				0x0100
// -------------------------------------------------------------- iManufacturer
#define USB_DESCRIPTOR_DEVICE_iManufacturer_Off				14
// -------------------------------------------------------------- iProduct
#define USB_DESCRIPTOR_DEVICE_iProduct_Off					15
// -------------------------------------------------------------- iSerialNumber
#define USB_DESCRIPTOR_DEVICE_iSerialNumber_Off				16
// -------------------------------------------------------------- bNumConfigurations
#define USB_DESCRIPTOR_DEVICE_bNumConfigurations_Off		17


/*=============================================================*/
/* +++++++++++++++++++ DESCRIPTOR: CONFIG ++++++++++++++++++++ */
/*=============================================================*/
// -------------------------------------------------------------- bLength
#define USB_DESCRIPTOR_CONFIG_bLength_Off					0			
#define USB_DESCRIPTOR_CONFIG_bLength		/*bytes*/		9	
// -------------------------------------------------------------- bDescriptorType
#define USB_DESCRIPTOR_CONFIG_bDescriptorType_Off			1
#define USB_DESCRIPTOR_CONFIG_bDescriptorType				2
// -------------------------------------------------------------- wTotalLength
#define USB_DESCRIPTOR_CONFIG_wTotalLength_Off				2
// -------------------------------------------------------------- bNumInterfaces
#define USB_DESCRIPTOR_CONFIG_bNumInterfaces_Off			4
// -------------------------------------------------------------- bConfigurationValue
#define USB_DESCRIPTOR_CONFIG_bConfigurationValue_Off		5
// -------------------------------------------------------------- iConfiguration
#define USB_DESCRIPTOR_CONFIG_iConfiguration_Off			6
// -------------------------------------------------------------- bmAttributes
#define USB_DESCRIPTOR_CONFIG_bmAttributes_Off				7
#define USB_DESCRIPTOR_CONFIG_bmAttributes_BusPower			0x80
#define USB_DESCRIPTOR_CONFIG_bmAttributes_SelfPower		0x40
#define USB_DESCRIPTOR_CONFIG_bmAttributes_RemoteWake		0x20
// -------------------------------------------------------------- bMaxPower
#define USB_DESCRIPTOR_CONFIG_bMaxPower_Off					8
#define USB_DESCRIPTOR_CONFIG_bMaxPower_500mA				250	
#define USB_DESCRIPTOR_CONFIG_bMaxPower_100mA				50		


/*=============================================================*/
/* +++++++++++++++++++ DESCRIPTOR: INTERFACE +++++++++++++++++ */
/*=============================================================*/
// -------------------------------------------------------------- bLength
#define USB_DESCRIPTOR_INTERFACE_bLength_Off				0
#define USB_DESCRIPTOR_INTERFACE_bLenght	/*bytes*/		9
// -------------------------------------------------------------- bDescriptorType
#define USB_DESCRIPTOR_INTERFACE_bDescriptorType_Off		1
#define USB_DESCRIPTOR_INTERFACE_bDescriptorType			0x04
// -------------------------------------------------------------- bInterfaceNumber
#define USB_DESCRIPTOR_INTERFACE_bInterfaceNumber_Off		2
// -------------------------------------------------------------- bAlternateSetting
#define USB_DESCRIPTOR_INTERFACE_bAlternateSetting_Off		3
// -------------------------------------------------------------- bNumEndpoints
#define USB_DESCRIPTOR_INTERFACE_bNumEndpoints_Off			4
// -------------------------------------------------------------- bInterfaceClass
#define USB_DESCRIPTOR_INTERFACE_bInterfaceClass_Off		5
#define USB_DESCRIPTOR_INTERFACE_bInterfaceClass_CDC_COMM	2
#define USB_DESCRIPTOR_INTERFACE_bInterfaceClass_CDC_DATA	10
// -------------------------------------------------------------- bInterfaceSubClass
#define USB_DESCRIPTOR_INTERFACE_bInterfaceSubClass_Off		6
#define USB_DESCRIPTOR_INTERFACE_bInterfaceSubClass_CDC_AbstractControlModel	2


/*=============================================================*/
/* +++++++++++++++++++ DESCRIPTOR: STRING ++++++++++++++++++++ */
/*=============================================================*/
// -------------------------------------------------------------- bLength
#define USB_DESCRIPTOR_STRING_bLength_Off					0
#define USB_DESCRIPTOR_STRING_bLength	/* bytes */			4
// -------------------------------------------------------------- bDescriptorType
#define USB_DESCRIPTOR_STRING_bDescriptorType_Off			1
#define USB_DESCRIPTOR_STRING_bDescriptorType				0x03
// -------------------------------------------------------------- wLANGID
#define USB_DESCRIPTOR_STRING_wLANGID_Off					2
#define USB_DESCRIPTOR_TYPE_STRING_LANGID_ENG_USA			0X0409
#define USB_DESCRIPTOR_TYPE_STRING_LANGID_ENG_AUS			0x0c09
#define USB_DESCRIPTOR_TYPE_STRING_LANGID_GER_GER			0x0407


/*=============================================================*/
/* +++++++++++++++++++ DESCRIPTOR: ENDPOINT ++++++++++++++++++ */
/*=============================================================*/
// -------------------------------------------------------------- bLength
#define USB_DESCRIPTOR_ENDPOINT_bLength_Off					0
#define USB_DESCRIPTOR_ENDPOINT_bLength			/*bytes*/	7
// -------------------------------------------------------------- bDescriptorType
#define USB_DESCRIPTOR_ENDPOINT_bDescriptorType_Off			1
#define USB_DESCRIPTOR_ENDPOINT_bDescriptorType				5
// -------------------------------------------------------------- bEndpointAddress
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_off					2
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Pos					0
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Msk					(_U_(0xF) << USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Pos)
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress(value)					(USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Msk & ((value) << USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Pos))
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Endpoint0_val			_U_(0x0)
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Endpoint1_val			_U_(0x1)	
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Endpoint2_val			_U_(0x2)
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Endpoint3_val			_U_(0x3)
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Endpoint4_val			_U_(0x4)
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Endpoint5_val			_U_(0x5)
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Endpoint6_val			_U_(0x6)
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Endpoint7_val			_U_(0x7)
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Endpoint0				(_U_(0x0) << USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Pos)
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Endpoint1				(_U_(0x1) << USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Pos)
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Endpoint2				(_U_(0x2) << USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Pos)
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Endpoint3				(_U_(0x3) << USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Pos)
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Endpoint4				(_U_(0x4) << USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Pos)
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Endpoint5				(_U_(0x5) << USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Pos)
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Endpoint6				(_U_(0x6) << USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Pos)
#define USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Endpoint7				(_U_(0x7) << USB_DESCRIPTOR_ENDPOINT_bEndpointAddress_Pos)
#define USB_DESCRIPTOR_ENDPOINT_bEndPointAddress_IN					0x80
#define USB_DESCRIPTOR_ENDPOINT_bEndPointAddress_OUT				0x00
// -------------------------------------------------------------- bmAttributes
#define USB_DESCRIPTOR_ENDPOINT_bmAttributes_Off						3
#define USB_DESCRIPTOR_ENDPOINT_bmAttributes_TransferType_Pos			0
#define USB_DESCRIPTOR_ENDPOINT_bmAttributes_TransferType_Msk			(_U_(0x3) << USB_DESCRIPTOR_ENDPOINT_bmAttributes_TransferType_Pos)
#define USB_DESCRIPTOR_ENDPOINT_bmAttributes_TransferType(value)		(USB_DESCRIPTOR_ENDPOINT_bmAttributes_TransferType_Msk & ((value) << USB_DESCRIPTOR_ENDPOINT_bmAttributes_TransferType_Pos))
#define USB_DESCRIPTOR_ENDPOINT_bmAttributes_TransferType_Control		0
#define USB_DESCRIPTOR_ENDPOINT_bmAttributes_TransferType_Isochronous	1
#define USB_DESCRIPTOR_ENDPOINT_bmAttributes_TransferType_Bulk			2
#define USB_DESCRIPTOR_ENDPOINT_bmAtrributes_TransferType_Interrupt		3
// -------------------------------------------------------------- wMaxPacketSize
#define USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_Off							4
#define USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_TransPerMicroframe_Pos		11
#define USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_1TransPerMicroframe_val		_U_(0x0)
#define USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_2TransPerMicroframe_val		_U_(0x1)
#define USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_3TransPerMicroframe_val		_U_(0x2)
#define USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_1TransPerMicroframe			_U_(0x0) << USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_TransPerMicroframe_Pos
#define USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_2TransPerMicroframe			_U_(0x1) << USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_TransPerMicroframe_Pos
#define USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_3TransPerMicroframe			_U_(0x2) << USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_TransPerMicroframe_Pos					
#define USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_MaxPacketSize_64	/*64bytes*/	0x0040
#define USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_MaxPacketSize_32	/*32bytes*/	0x0020
#define USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_MaxPacketSize_16	/*16bytes*/	0x0010
#define USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_MaxPacketSize_8	/*8bytes*/	0x0008
// --------------------------------------------------------------  bInterval
#define USB_DESCRIPTOR_ENDPOINT_bInterval_Off				6
#define USB_DESCRIPTOR_ENDPOINT_bInterval_NoPolling			0

/*=============================================================*/
/* +++++++++++++++++++++++++ REQUEST +++++++++++++++++++++++++ */ //(Pg. 248) [4]
/*=============================================================*/
// -------------------------------------------------------------- bLength
#define USB_REQUEST_bLength		/* 8 bytes */				8
// -------------------------------------------------------------- bmRequestType
#define bmRequestType_Off						0
#define bmRequestType_DataTransferDir_Pos		7
#define bmRequestType_DataTransferDir_Msk		0x80
#define bmRequestType_DataTransferDir(value)	(bmRequestType_DataTransferDir_Msk & ((value) << bmRequestType_DataTransferDir_Pos)))
#define bmRequestType_HostToDevice_val			_U_(0x0)
#define bmRequestType_DeviceToHost_val			_U_(0x1)
#define bmRequestType_HostToDevice				(0 << 7)
#define bmRequestType_DeviceToHost				(1 << 7)
#define bmRequestType_Type_Pos					5
#define bmRequestType_Type_MSK					0x60
#define bmRequestType_Type_Standard				(0 << 5)
#define bmRequestType_Type_Class				(1 << 5)
#define bmRequestType_Type_Vendor				(2 << 5)
#define bmRequestType_Type_Reserved				(3 << 5)
#define bmRequestType_Recipient_Pos				0
#define bmRequestType_Recipient_Mask			0x1F
#define bmRequestType_Recipient_Device			0
#define bmRequestType_Recipient_Interface		1
#define bmRequestType_Recipient_Endpoint		2
#define bmRequestType_Recipient_Other			3
// -------------------------------------------------------------- bRequest
#define bRequest_Off							1
#define bRequest_Get_Status						0
#define bRequest_Clear_Feature					1
#define bRequest_Set_Feature					3
#define bRequest_Set_Address					5
#define bRequest_Get_Descriptor					6
#define bRequest_Set_Descriptor					7
#define bRequest_Get_Configuration				8
#define bRequest_Set_Configuration				9
#define bRequest_Get_Interface					10
#define bRequest_Set_Interface					11
#define bRequest_SYNCH_FRAME					12

// Shortened bRequest
#define Get_Status						0
#define Clear_Feature					1
#define Set_Feature						3
#define Set_Address						5
#define Get_Descriptor					6
#define Set_Descriptor					7
#define Get_Configuration				8
#define Set_Configuration				9
#define Get_Interface					10
#define Set_Interface					11
#define SYNCH_Frame						12
// -------------------------------------------------------------- wValue
#define USB_REQUEST_wValue_Off								2
// -------------------------------------------------------------- wIndex
#define USB_REQUEST_wIndex_Off								4
// -------------------------------------------------------------- wLength
#define USB_REQUEST_wLength_Off								6
#define USB_REQUEST_wLength_NoDataStage						0x00


/*=============================================================*/
/* +++++++++++++ Interface Association Descriptor ++++++++++++ */ 
/*=============================================================*/
// -------------------------------------------------------------- bLength
#define USB_DESCRIPTOR_IAD_bLength	/* bytes */				0x08
// -------------------------------------------------------------- bDescriptorType
#define USB_DESCRIPTOR_IAD_bDescriptorType					0x0B
// -------------------------------------------------------------- bFirstInterface
#define	USB_DESCRIPTOR_IAD_bFirstInterface					0x00
// -------------------------------------------------------------- bInterfaceCount
#define USB_DESCRIPTOR_IAD_bInterfaceCount					0x02
// -------------------------------------------------------------- bFunctionClass
#define USB_DESCRIPTOR_IAD_bFunctionClass					0x02
// -------------------------------------------------------------- bFunctionSubClass
#define USB_DESCRIPTOR_IAD_bFunctionSubclass				0x02
// -------------------------------------------------------------- bFunctionProtcol
#define USB_DESCRIPTOR_IAD_bFunctionProtocol_NoneSpecified	0x00
#define USB_DESCRIPTOR_IAD_bFunctionProtocol_AsyncSerial	0x01
// -------------------------------------------------------------- iFunction
#define USB_DESCRIPTOR_IAD_iFunction						0x00 

/****************************************************************
						     Macros
****************************************************************/ 

/****************************************************************
						   Structures
****************************************************************/ 



/****************************************************************
						    Globals
****************************************************************/ 


/****************************************************************
						Function Prototypes
****************************************************************/ 



#endif /* SYS_USB_H_ */