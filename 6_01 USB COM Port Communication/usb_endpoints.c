/*
 * usb_endpoints.c
 *
 * Created: 12/31/2020 1:43:56 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *
 * Resources:
 *		[1] SAM L21 Family Data Sheet
 */ 
#include "usb_endpoints.h"


void usb_initialize_endpoints(USB_ENDPOINT_DESCRIPTORS_TYPE *endpoint){
	/* Setup up the USB Controller with the corresponding endpoint settings */
	uint8_t		endpoint_num = (endpoint->bDescriptorType) & 0x0F; // Endpoint Number
	uint8_t		endpoint_dir = (endpoint->bDescriptorType) & 0x80; // D7	| 0 = OUT, 1 = IN
	uint8_t		endpoint_typ = (endpoint->bDescriptorType) & 0x03; // D1:0	| 0=Control, 1=Iso, 2=Bulk, 3=Int
	uint16_t	endpoint_len = (endpoint->wMaxPacketSize)  & 0x03FF; // [bytes]
	
	// Clear Type
	switch(endpoint_dir){
		case USB_DESCRIPTOR_ENDPOINT_bEndPointAddress_IN:
			USB->DEVICE.DeviceEndpoint[endpoint_num].EPCFG.bit.EPTYPE1 = 0x00; // Disable
			break;
		default:
			USB->DEVICE.DeviceEndpoint[endpoint_num].EPCFG.bit.EPTYPE0 = 0x00; // Disable
			break;
	}
	
	// Get Size
	endpoint_len = set_endpoint_PCKSIZE_SIZE(endpoint_len, endpoint_typ);
	
	// Get Type
	endpoint_typ = set_endpoint_direction(endpoint_typ);
	
	// Set Input
	switch(endpoint_dir){
		case USB_DESCRIPTOR_ENDPOINT_bEndPointAddress_IN:
			USB->DEVICE.DeviceEndpoint[endpoint_num].EPCFG.bit.EPTYPE1 = endpoint_typ;	// Set Size
			USB->DEVICE.DeviceEndpoint[endpoint_num].EPINTENSET.bit.TRCPT1 = 1;			// Set Transmission Complete Interrupt
			USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUSCLR.bit.BK1RDY = 1;		// Clear EPSTATUS Bank 1 Ready Flag
			USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUSCLR.bit.DTGLIN = 1;		// Clear Data In toggle
			USB_EP_mem[endpoint_num].in.PCKSIZE.bit.SIZE = endpoint_len;
			break;
		default:
			USB->DEVICE.DeviceEndpoint[endpoint_num].EPCFG.bit.EPTYPE0 = endpoint_typ;
			USB->DEVICE.DeviceEndpoint[endpoint_num].EPINTENSET.bit.TRCPT0 = 1;
			USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUSCLR.bit.BK0RDY = 1;
			USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUSCLR.bit.DTGLIN = 1;
			USB_EP_mem[endpoint_num].out.PCKSIZE.bit.SIZE = endpoint_len;
			break;
	}
}



void set_all_endpoint_types(uint8_t OUT_eptype_Bank0, uint8_t IN_eptype_Bank1){
	for(uint8_t ep = 0; ep < NUM_ENDPOINTS; ep++){
		set_endpoint_type(ep, OUT_eptype_Bank0, IN_eptype_Bank1);
	}
}

void set_endpoint_type(uint8_t n_ep, uint8_t OUT_eptype_Bank0, uint8_t IN_eptype_Bank1){
	/* Call to configure a given endpoint for a type class:               (Pg. 852) [1]
			(0) Disabled
			(1) Control, 
			(2) Isochronous, 
			(3) Bulk, 
			(4) Interrupt, or 
			(5) Dual-Bank
			*/
	USB->DEVICE.DeviceEndpoint[n_ep].EPCFG.reg = (	USB_DEVICE_EPCFG_EPTYPE1(IN_eptype_Bank1) | 
                                                    USB_DEVICE_EPCFG_EPTYPE0(OUT_eptype_Bank0));
}

void erase_EndpointDescriptors(void){
	/* Clear all Data for Each Endpoint */
	for (uint32_t x = 0; x < (uint32_t)sizeof(USB_EP_mem); x++){
		((uint8_t *)USB_EP_mem)[x] = 0x00;
	}
}

uint8_t set_endpoint_PCKSIZE_SIZE(uint16_t byte_len, uint8_t TransferType){
	/* Given the packet size [in bytes] of the endpoint, initialize the
	   Endpoint Hardware with the correct size.
	   
	   :param byte_len: size of the endpoint.
	   :param TransferType: 0=Control, 1=Isochronous, 2=Bulk, 3=Int
	   :return POS: value specifying the maximum packet size of the endpoint in accordance PCKSIZE.SIZE (Pg. 862) [1]
	  */
	uint8_t POS	= 0;
	uint8_t SIZE = 4;
	
	// Data Sheet specifies only Isochronous can have a packet size larger than 64 bytes.
	if (TransferType == USB_DESCRIPTOR_ENDPOINT_bmAttributes_TransferType_Isochronous){
		SIZE = 8;
	}
	
	// Calculate Endpoint Size in accordance to the data sheet. (Pg. 863) [1]
	for( POS = 0; POS < SIZE; POS++){
		if(byte_len <= (0x0010 << POS)){
			break;
		}
	}
	return POS;
}

uint8_t set_endpoint_direction(uint8_t TransferType){
	/* Given an Endpoint -> bmAttributes -> TransferType, return the 
	   EPCFGn.EPTYPE corresponding value (Pg. 852) [1].
	   :param TransferType: the configured Endpoint Transfer Type: 1=Control, 
	                        2=Isochronous, 3=Bulk, and 4=Interrupt. 
	*/
	
	uint8_t EPTYPE = 0x00;
	
	switch(TransferType){
		case USB_DESCRIPTOR_ENDPOINT_bmAttributes_TransferType_Control: 
			EPTYPE = USB_DEVICE_EPCFG_EPTYPE_CONTROL; 
			break;
		case USB_DESCRIPTOR_ENDPOINT_bmAttributes_TransferType_Isochronous: 
			EPTYPE = USB_DEVICE_EPCFG_EPTYPE_ISOCHRONOUS; 
			break;
		case USB_DESCRIPTOR_ENDPOINT_bmAttributes_TransferType_Bulk: 
			EPTYPE = USB_DEVICE_EPCFG_EPTYPE_BULK; 
			break;
		default:
			EPTYPE = USB_DEVICE_EPCFG_EPTYPE_INTERRUPT;
			break;
	}
	
	return EPTYPE;
}

uint8_t get_endpoint_type(uint8_t endpoint_num, uint8_t endpoint_dir){
	uint8_t EPTYPE = 0x00;
	
	if(endpoint_dir == USB_DESCRIPTOR_ENDPOINT_bEndPointAddress_IN){
		EPTYPE = USB->DEVICE.DeviceEndpoint[endpoint_num].EPCFG.bit.EPTYPE1;
	}
	else{
		EPTYPE = USB->DEVICE.DeviceEndpoint[endpoint_num].EPCFG.bit.EPTYPE0;
	}
	return EPTYPE;
}

uint8_t is_endpoint_enabled(uint8_t endpoint_num, uint8_t endpoint_dir){
	uint8_t EPTYPE = get_endpoint_type(endpoint_num, endpoint_dir);
	return ((EPTYPE == USB_DEVICE_EPCFG_EPTYPE_DISABLED) ? 1 : 0);
}

uint8_t is_endpoint_stalled(uint8_t endpoint_num, uint8_t endpoint_dir){
	uint8_t stall = 0;
	if (endpoint_dir == 0x80){
		stall = USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUS.bit.STALLRQ1;
	}else{
		stall = USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUS.bit.STALLRQ0;
	}
	return stall;
}

void endpoint_stall(uint8_t endpoint_num, uint8_t endpoint_dir){
	if(endpoint_dir == 0x80){
		USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUSSET.bit.STALLRQ1;
	} else {
		USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUSSET.bit.STALLRQ0;
	}
}