/*
 * usb_descriptors.c
 *
 * Created: 12/27/2020 11:33:21 AM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Initializes all USB descriptors for this project into RAM.
 * 
 */ 

#include <stdalign.h>
#include "usb_descriptors_all.h"


const alignas(4) USB_DEVICE_DESCRIPTORS_TYPE reg_device = {
	.bLength				= sizeof(USB_DEVICE_DESCRIPTORS_TYPE),
	.bDescriptorType		= USB_DESCRIPTOR_DEVICE_bDescriptorType,
	.bcdUSB					= USB_DESCRIPTOR_DEVICE_bcdUSB_1dot1,
	.bDeviceClass			= USB_DESCRIPTOR_DEVICE_bDeviceClass_CDC,
	.bDeviceSubClass		= USB_DESCRIPTOR_DEVICE_bDeviceSubClass_CDC,
	.bDeviceProtocol		= USB_DESCRIPTOR_DEVICE_bDeviceProtocol_CDC,
	.bMaxPacketSize0		= USB_DESCRIPTOR_DEVICE_bMaxPacketSize_64,
	.idVendor				= 0x6666,	// Specify we are a prototype device, unless you want to pay for a vendor ID :)
	.idProduct				= 0x8888,	// Can be anything since we are a prototype device.
	.bcdDevice				= USB_DESCRIPTOR_DEVICE_bcdDevice_Ver_1_0,
	.iManufacturer			= 0x01,		// Can be anything
	.iProduct				= 0x02,		// Can be anything
	.iSerialNumber			= 0x03,		// Can be anything
	.bNumConfigurations		= 1,		// Only doing CDC so 1 config.
};

const alignas(4) CDCDescriptor_t reg_cdc = {
	
	// ----------------------- Configuration
	.reg_cdc_config = {
		.bLength			= sizeof(USB_CONFIG_DESCRIPTORS_TYPE),
		.bDescriptorType	= USB_DESCRIPTOR_CONFIG_bDescriptorType,
		.wTotalLength		= sizeof(CDCDescriptor_t),
		.bNumInterfaces		= 2,
		.bConfigurationValue= 1,
		.iConfiguration		= 0,
		.bmAttributes		= USB_DESCRIPTOR_CONFIG_bmAttributes_BusPower,
		.bMaxPower			= USB_DESCRIPTOR_CONFIG_bMaxPower_100mA,
	},
	/*
	//------------------------ Interface Association
	.reg_cdc_iad = {
		.bLength			= sizeof(USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE),
		.bDescriptorType	= 0x0B,
		.bFirstInterface	= 0x00,
		.bInterfaceCount	= 0x02,
		.bFunctionClass		= 0x02,
		.bFunctionSubClass	= 0x02,
		.bFunctionProtocol  = USB_DESCRIPTOR_IAD_bFunctionProtocol_AsyncSerial,
		.iFunction			= 0x00,
	},
	*/
	//------------------------ Interface #1: Communication
	.reg_cdc_comm = {
		.bLength			= sizeof(USB_INTERFACE_DESCRIPTORS_TYPE),
		.bDescriptorType	= USB_DESCRIPTOR_INTERFACE_bDescriptorType,
		.bInterfaceNumber	= 0,
		.bAlternateSetting	= 0,
		.bNumEndpoints		= 1,
		.bInterfaceClass	= USB_DESCRIPTOR_INTERFACE_bInterfaceClass_CDC_COMM,
		.bInterfaceSubClass	= USB_DESCRIPTOR_INTERFACE_bInterfaceSubClass_CDC_AbstractControlModel,
		.bInterfaceProtocol = USB_DESCRIPTOR_IAD_bFunctionProtocol_NoneSpecified,
		.iInterface			= 0,
	},
	
	//------------------------ Communication Functional Descriptors
	.reg_cdc_header = {
		.bFunctionLength	= sizeof(USB_CDC_HEADER_FUNCTIONAL_DESCRIPTOR_TYPE),
		.bDescriptorType	= USB_DESCRIPTOR_HEADER_bDescriptorType_Interface,
		.bDescriptorSubtype	= USB_DESCRIPTOR_HEADER_bDescriptorSubtype_Header,
		.bcdCDC				= USB_DESCRIPTOR_HEADER_bcdCDC,
	},
	.reg_cdc_acm = {
		.bFunctinLength		= sizeof(USB_CDC_ACM_FUNCTIONAL_DESCRIPTOR_TYPE),
		.bDescriptorType	= USB_DESCRIPTOR_ACM_bDescriptorType_Interface,
		.bDescriptorSubtype = USB_DESCRIPTOR_ACM_bDescriptorSubtype_ACM,
		.bmCapabilities		= USB_DESCRIPTOR_ACM_bmCapabilities_LineRequestOnly,
	},
	.reg_cdc_mgmt = {
		.bFunctionLength	= sizeof(USB_CDC_CALL_MANAGEMENT_FUNCTIONAL_DESCRIPTOR_TYPE),
		.bDescriptorType	= USB_DESCRIPTOR_CALL_MAN_bDescriptorType_Interface,
		.bDescriptorSubtype = USB_DESCRIPTOR_CALL_MAN_bDescriptorSubtype_Call_Management,
		.bmCapabilities		= USB_DESCRIPTOR_CALL_MAN_bmCapabilities_Call_Management_Via_DCI, // Device handles call management itself, Call management info is sent/received over the communication class interface (not the data interface). 
		.bDataInterface		= 1,
	},
	.reg_cdc_union = {
		.bFunctionLength	= sizeof(USB_CDC_UNION_FUNCTIONAL_DESCRIPTOR_TYPE),
		.bDescriptorType	= USB_DESCRIPTOR_UNION_bDescriptorType_Interface,
		.bDescriptorSubtype = USB_DESCRIPTOR_UNION_bDescriptorSubtype_Union,
		.bControlInterface	= 0,
		.bSubordinateInterface0 = 1,
	},
	//------------------------ Communication Endpoint Descriptor
	.reg_ep_comm = {
		.bLength			= sizeof(USB_ENDPOINT_DESCRIPTORS_TYPE),
		.bDescriptorType	= USB_DESCRIPTOR_ENDPOINT_bDescriptorType,
		.bEndpointAddress	= (USB_EP_3_CDC_COMM | USB_DESCRIPTOR_ENDPOINT_bEndPointAddress_IN),
		.bmAttributes		= USB_DESCRIPTOR_ENDPOINT_bmAtrributes_TransferType_Interrupt,
		.wMaxPacketSize		= USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_MaxPacketSize_64,
		.bInterval			= 1,
	},
	
	
	//------------------------ Interface #2: Data
	.reg_cdc_data = {
		.bLength			= sizeof(USB_INTERFACE_DESCRIPTORS_TYPE),
		.bDescriptorType	= USB_DESCRIPTOR_INTERFACE_bDescriptorType,
		.bInterfaceNumber	= 1,
		.bAlternateSetting	= 0,
		.bNumEndpoints		= 2,
		.bInterfaceClass	= USB_DESCRIPTOR_INTERFACE_bInterfaceClass_CDC_DATA,
		.bInterfaceSubClass = 0,
		.bInterfaceProtocol	= 0,
		.iInterface			= 0,
	},
	//------------------------ Data Endpoint IN
	.reg_cdc_data_in = {
		.bLength			= sizeof(USB_ENDPOINT_DESCRIPTORS_TYPE),
		.bDescriptorType	= USB_DESCRIPTOR_ENDPOINT_bDescriptorType,
		.bEndpointAddress	= (USB_EP_1_CDC_DATA | USB_DESCRIPTOR_ENDPOINT_bEndPointAddress_IN),
		.bmAttributes		= USB_DESCRIPTOR_ENDPOINT_bmAttributes_TransferType_Bulk,
		.wMaxPacketSize		= USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_MaxPacketSize_64,
		.bInterval			= USB_DESCRIPTOR_ENDPOINT_bInterval_NoPolling,
	},
	//------------------------ Data Endpoint OUT
	.reg_cdc_data_out = {
		.bLength			= sizeof(USB_ENDPOINT_DESCRIPTORS_TYPE),
		.bDescriptorType	= USB_DESCRIPTOR_ENDPOINT_bDescriptorType,
		.bEndpointAddress	= (USB_EP_2_CDC_DATA | USB_DESCRIPTOR_ENDPOINT_bEndPointAddress_OUT),
		.bmAttributes		= USB_DESCRIPTOR_ENDPOINT_bmAttributes_TransferType_Bulk,
		.wMaxPacketSize		= USB_DESCRIPTOR_ENDPOINT_wMaxPacketSize_MaxPacketSize_64,
		.bInterval			= USB_DESCRIPTOR_ENDPOINT_bInterval_NoPolling,
	}
};

const alignas(4) USB_STRING_DESCRIPTORS_UNICODE_TYPE reg_string_uni = {
	.bLength				= sizeof(USB_STRING_DESCRIPTORS_UNICODE_TYPE),
	.bDescriptorType		= USB_DESCRIPTOR_STRING_bDescriptorType,
	.wLANGID				= USB_DESCRIPTOR_TYPE_STRING_LANGID_ENG_USA,
};

const alignas(4) USB_CDC_LINECODING_TYPE reg_line_coding = {
	.deDTERate		= USB_CDC_LINECOODING_deDTERate_115200,
	.bCharFormat	= USB_CDC_LINECODING_bCharFormat_1StopBit,
	.bParityType	= USB_CDC_LINECODING_bParityType_None,
	.bDataBits		= USB_CDC_LINECODING_bDataBits_Width_8,
};


const char *usb_strings[] = {
	[0] = L'\0',
	[1] = "John",
	[2] = "Virtual COM-Port",
	[3] = USB_SERIAL_NUMBER,
};






