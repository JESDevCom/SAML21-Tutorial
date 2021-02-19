/*
 * usb_descriptors_std.h
 *
 * Created: 12/23/2020 7:26:38 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Contains all the Standard Descriptors required for communication
 *		over USB2.0. An IAD is not a typical standard but may be required for 
 *		grouping interfaces.
 *
 * Notes:
 *		[1] Interface Association Descriptor (IAD) [Source: Microsoft]
 *			-	IAD allows the device to group interfaces that belong
 *				to a function.
 *			-	To alert an OS that a device has an IAD, set the following
 *				device descriptor fields:
 *					bDeviceClass	0xEF	// for misc. device
 *					bDeviceSubclass	0x02
 *					bDeviceProtocol	0x01
 *				otherwise, the OS will not detect the device's IAD or group
 *				of device interfaces properly.
 *			-   IAD must be located immediately before the interfaces
 *				in the interface group that the IAD describes.
 *			- Read More at: https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/usb-interface-association-descriptor
 *
 * Resources
 *		[1] SAM L21 Family Data Sheet
 *		[2] SAM L21 XPlained Pro User Guide
 *		[3] USB 2.0 Manual: http://sdphca.ucsd.edu/lab_equip_manuals/usb_20.pdf
 *		[4] USB-IF: https://www.usb.org/defined-class-codes
 */ 


#ifndef SYS_USB_STD_H_
#define SYS_USB_STD_H_

/****************************************************************
						   Header Files
****************************************************************/ 
#include "saml21j18b.h"	
/****************************************************************
						   Definitions
****************************************************************/ 

/****************************************************************
						   Structures
****************************************************************/ 

/********************** Device Descriptors *********************/	
typedef struct __attribute__ ((packed)){
	uint8_t		bLength;		/* Size of the descriptor is bytes */
	uint8_t		bDescriptorType;/* Device Descriptor Type = 1*/
	uint16_t	bcdUSB;			/* USB specification number (USB 2.10 is 0x210) */
	uint8_t		bDeviceClass;	/*	Class Code: (assigned by USB-IF)
									If this field is:
									* reset to zero, each interface within a config specifies 
									  its own class information and the various interfaces operate independently.
									* set a value between 0x01 and 0xFE, the device supports different class 
									  specs on different interfaces and the interfaces may not operate independently. 
									  This value identifies the class definition used for the aggregate interfaces.
									* set to 0xFF, the device class is vendor specific. */
	uint8_t		bDeviceSubClass; /* Subclass Code: (assigned by USB-IF)
									If bDeviceClass is:
									* reset to zero, this field must also be reset to zero.
									* not set to 0xFF, all values are reserved for assignment by USB-IF.
									*/
	uint8_t		bDeviceProtocol; /* Protocol code (assigned by USB-IF)
									These codes are qualified by the value of bDeviceClass and bDeviceClass fields.
									If a device supports class-specific protocols on a device basis as opposed to an
									interface basis, this code identifies the protocols that the device use as defined
									by the specification of the device class.
									This field is:
									* reset to zero, the device does not use class specific protocols on a device basis.
									  However, it may use class specific protocols on an interface basis.
									* set to 0xFF, the device uses a vendor specific protocol on a device basis. */
	uint8_t		bMaxPacketSize0;  /* Maximum packet side for Endpoint[0] (only, 8, 16, 32, 64) are valid */
	uint16_t	idVendor;		  /* Vendor ID (assigned by the USB-IF) */
	uint16_t	idProduct;		  /* Product ID (assigned by the manufacturer) */	
	uint16_t	bcdDevice;		  /* Device release number in binary-coded decimal */
	uint8_t		iManufacturer;	  /* Index of string descriptor describing the manufacturer */
	uint8_t		iProduct;         /* Index of string describing the product */
	uint8_t		iSerialNumber;	  /* Index of string descriptor describing the device's serial number */
	uint8_t		bNumConfigurations; /* number of possible configurations */	
	}USB_DEVICE_DESCRIPTORS_TYPE;
	
	
	
/****************** Configuration Descriptors ******************/	
typedef struct __attribute__ ((packed)) {
	uint8_t		bLength;			/* Size of this descriptor in bytes */
	uint8_t		bDescriptorType;	/* Configuration Descriptor Type = 2 */
	uint16_t	wTotalLength;		/* Total length of data returned for this configuration. */
	uint8_t		bNumInterfaces;		/* Number of interfaces supported by this configuration.*/
	uint8_t		bConfigurationValue;/* Value to select this configuration with SetConfiguration() */
	uint8_t		iConfiguration;		/* Index of string descriptor describing this configuration */
	uint8_t		bmAttributes;		/* Bit7: must be 1, 
									   Bit6: set for 'self-powered', 
									   Bit5: set for 'remote-wake', and
									   Bit[4:0] :Reserved (reset to zero) */
	uint8_t		bMaxPower;			/* Maximum power consumption of the USB device from the bus  in this 
									   specific configuration when the device is fully operational. 
									   Expressed in 2[mA] units (50=100[mA]) */
}USB_CONFIG_DESCRIPTORS_TYPE;



/******************** Interface Descriptors ********************/	
typedef struct __attribute__ ((packed)) {
	uint8_t		bLength;			/* Size of this descriptor in bytes */
	uint8_t		bDescriptorType;	/* Interface Descriptor Type = 4 */
	uint8_t		bInterfaceNumber;	/* The number of this interface */
	uint8_t		bAlternateSetting;	/* Value used to select an alt setting for the interface identified 
									   in the prior field.*/
	uint8_t		bNumEndpoints;		/* Number of endpoints used by this interface (excluding 0 endpoint) */
	uint8_t		bInterfaceClass;	/* Code Class (assigned by USB-IF)
								         * a value of zero is reserved for future standardization 
									     * if this field is set to 0xFF, the interface class is vendor specific
									     * all other values are reserved for assignment by the USB-IF */
	uint8_t		bInterfaceSubClass;	/* Subclass code (assigned by USB-IF)
		                               If bInterfaceClass:
									      * is reset to zero, this field must also be reset.
										  * is not set to 0xFF, all values are reserved for assignment by USB-IF */
	uint8_t		bInterfaceProtocol; /* Protocol Code (assigned by USB-IF)
				                       If an interface supports class-specific requests, this code identifies 
									   the protocols that the device uses as defined in the device class. 
									   If this field:
									       * is reset to zero, the device does not use a class-specific protocol 
										     on this interface.
										   * is set to 0xFF, the device uses a vendor specific protocol for this 
										     interface. */
	uint8_t		iInterface;			/* Index of string descriptor describing this interface */
	}USB_INTERFACE_DESCRIPTORS_TYPE;

/******************** Endpoint Descriptors ********************/
typedef struct __attribute__ ((packed)) {
	uint8_t		bLength;
	uint8_t		bDescriptorType;
	uint8_t		bEndpointAddress;
	uint8_t		bmAttributes;
	uint16_t	wMaxPacketSize;
	uint8_t		bInterval;
	}USB_ENDPOINT_DESCRIPTORS_TYPE;

/********************** String Descriptors *********************/	
typedef struct __attribute__ ((packed)) {
	uint8_t		bLength;
	uint8_t		bDescriptorType;
	uint16_t	wLANGID;			
	}USB_STRING_DESCRIPTORS_UNICODE_TYPE;
	
typedef struct __attribute__ ((packed)) {
	uint8_t		bLength;
	uint8_t		bDescriptorType;
	uint16_t	bString;			
	} USB_STRING_DESCRIPTORS_TYPE;
	
/****************** Device Qualifier Descriptors ***************/
typedef struct {
	uint8_t		bLength;
	uint8_t		bDescriptorType;
	uint16_t	bcdUSB;
	uint8_t		bDeviceClass;
	uint8_t		bDeviceSubClass;
	uint8_t		bDeviceProtocol;
	uint8_t		bMaxPacketSize0;
	uint8_t		bNumConfigurations;
	uint8_t		bReserved;
	}USB_QUALIFIER_DESCRIPTORS_TYPE;
	
/********************* Device Setup Packet *********************/
typedef struct __attribute__ ((packed)) {
	uint8_t		bmRequestType;		/* Characteristic of Request (see definitions: USB_REQUEST_bmRequestType_XX) */
	uint8_t		bRequest;			/* Specific Request (see definitions: USB_STD_REQUEST_CODE_XX) */
	uint16_t	wValue;				/* Value according to request */
	uint16_t	wIndex;				/* Index or Offset according to request */
	uint16_t	wLength;			/* Number of bytes to transfer if there is a data state */
	}USB_REQUEST_TYPE;

/*********************** Empty USB Header **********************/	
typedef struct __attribute__ ((packed)) {
	uint8_t		bLength;
	uint8_t		bDescriptorType;
	}USB_HEADER_DESCRIPTOR_TYPE;
	
	
/**************** Interface Association Descriptor **************/	
typedef struct __attribute__ ((packed)){
	uint8_t				bLength;
	uint8_t				bDescriptorType;
	uint8_t				bFirstInterface;
	uint8_t				bInterfaceCount;
	uint8_t				bFunctionClass;
	uint8_t				bFunctionSubClass;
	uint8_t				bFunctionProtocol;
	uint8_t				iFunction;
}USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE;

/****************************************************************
						   Globals
****************************************************************/ 

/****************************************************************
					   Function Prototypes
****************************************************************/ 

#endif /* SYS_USB_STD_H_ */