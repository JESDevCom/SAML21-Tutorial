/*
 * usb_std.c
 *
 * Created: 1/12/2021 4:32:44 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Initialize hardware for high speed communication.
 *		No communication protocol setup in this file, see init_USB_com.c for that.
 *
 * Hardware:
 *		F_CPU: 4 [MHz]
 *		F_USB: 480 [MHz]
 *
 * Initialization Process:
 *		[5] After Enable (Handshake Process)
 *			[5.1] Read PA14 to detect the VBUS voltage on the Micro-USB Port
 *			[5.2] Call attach our DEVICE to the HOST (CTRLB.DETACH=0)
 *			[5.3] Host will request USB device descriptor using the default address zero.
 *			[5.4] On successful TX, the host will send a USB reset command. Followed by device address,
 *				  will be used for all following communication with the host.
 *			[5.5] Add 'device address' to Device Address Register (DADD.DADD) and enable (DADD.ADDEN=1).
 *				  DADD.ADDEN automatically cleared on RX of a USB reset command.
 *		[6] End Point Configuration (Address to look for data in memory)
 *
 * Notes:
 *		[1] Endpoint 0 is used for control transfer only and ca not be assigned to any other function.
 *		[2] OUT always refers to the direction from host to the device.
 *		[3] IN always refers to the direction pointing towards the host.
 *
 * USB Classes:
 *		Control - transfer command and status operations
 *		Bulk	- Mass Storage, Communication Device Class (CDC)
 *				  Features:		(1) Guaranteed delivery/error checking
 *								(2) High Data throughput (up to 19 x 64 byte packets per millisecond)
 *								(3) Lowest bus priority
 *		Interrupt - Human Interface Device like a Mouse
 *		Isochronous - intended for audio and video applications
 *
 * USB Frame Overview:
 *		[Start of Frame (SOF)][Control or Data Packets]
 *
 *		Example: [SOF][Interrupt][Bulk][Bulk][Bulk] [SOF][Bulk][Bulk][Bulk][Bulk][Bulk]
 *				 |<----------- 1 [ms] ----------->| |<------------ 1 [ms] ----------->|   Period: 1KHz
 *
 * USB Payload:
 *		Max Payload Size: 1024 bytes
 *
 * Resources:
 *		[1] SAM L21 Family Data Sheet
 *			- USB Info Starts on Page 812
 *			- Device Registers Summary (Pg. 838)
 *
 *		[2] Atmel SAM L21 Xplained Pro User Guide
 *		[3] USB 2.0 Manual: http://sdphca.ucsd.edu/lab_equip_manuals/usb_20.pdf
 */ 

/* ---- Include ------------------------------------------------------------ */
#include <stdbool.h>
#include <stdalign.h>
#include <string.h>
#include "usb_std.h"

#define RQST(cmd, rDir, recipient, typ)  ((cmd << 8) | bmRequestType_##rDir | bmRequestType_Recipient_##recipient | bmRequestType_Type_##typ)

/* ---- Typedef ------------------------------------------------------------- */
//typedef void (*usb_ep_callback_t)(int size);

/* ---- Local Global -------------------------------------------------------- */
//static usb_ep_callback_t usb_ep_callbacks[8];
uint8_t  Set_Line_Coding_Flag = 0;
uint8_t  Get_Line_Coding_Flag = 0;
uint8_t  Set_Control_Line_State_Flag = 0;
//uint16_t Set_Line_Coding_Length = 0;

void usb_init(void){
	/*
	for (int i = 0; i < 8; i++){
		usb_ep_callbacks[i] = NULL;
	}*/
	usb_hw_init();
}

/*
void usb_set_callback(int ep, void (*callback)(uint32_t size)){
	usb_ep_callbacks[ep] = callback;
}

__attribute__((weak)) bool usb_class_handle_request(USB_REQUEST_TYPE *request){
	(void)request;
	return false;
}

void usb_send_callback(int ep){
	if (usb_ep_callbacks[ep]){
		usb_ep_callbacks[ep](0);
	}
}

void usb_recv_callback(int ep, int size){
	if(usb_ep_callbacks[ep]){
		usb_ep_callbacks[ep](size);
	}
}
*/

uint8_t usb_requests(USB_REQUEST_TYPE *request_msg){
	
	static uint8_t request_valid = valid;
	
	// Extract Bit Fields
	uint8_t		bmRequestType = request_msg->bmRequestType;
	uint8_t	    bRequest = request_msg->bRequest;
	uint16_t	wValue = request_msg->wValue;
	uint16_t	wIndex = request_msg->wIndex;
	uint16_t	wLength = request_msg->wLength;
	
	
	
	switch((bRequest << 8) | bmRequestType){

	   /*----------------------------------------------------------------------------
		*   GET DESCRIPTOR
		*---------------------------------------------------------------------------*/
		case RQST(Get_Descriptor, DeviceToHost, Device, Standard): {
			
			uint8_t DescriptorType	= wValue >> 8;
			uint8_t DescriptorIndex = wValue & 0xff;
				
			if (DescriptorType == USB_DESCRIPTOR_TYPE_DEVICE){
				
					// If our packet is smaller than 64 bytes, send exact length; otherwise, send max packet size.
					// If wLength is less than 64 bytes, the device is indicating the end of the control transfer.
					uint32_t len = LIMIT(wLength, reg_device.bLength);
					usb_control_send((uint8_t *)&reg_device, len);
					
			}
			else if (DescriptorType == USB_DESCRIPTOR_TYPE_CONFIGURATION){
					
					uint32_t len = LIMIT(wLength, reg_cdc.reg_cdc_config.wTotalLength);
					usb_control_send((uint8_t *)&reg_cdc, len);
						
			}
			else if (DescriptorType == USB_DESCRIPTOR_TYPE_STRING){
				if (DescriptorIndex == 0){
					uint32_t len = LIMIT(wLength, reg_string_uni.bLength);
					usb_control_send((uint8_t *)&reg_string_uni, len);
				}
				else if(DescriptorIndex < 4){
					const char *str = usb_strings[wIndex];
					uint32_t len = strlen(str);
					uint32_t size = len*2 + 2;
					alignas(4) uint8_t buffer[size];
							
					buffer[0] = size; //.blength
					buffer[1] = USB_DESCRIPTOR_TYPE_STRING; //bDescriptorType
							
					// Convert 8-bit ASCII to UTF-16 in Little-Endian
					for (uint32_t i = 0; i < len; i++){
						buffer[2 + i*2] = str[i];
						buffer[3 + i*2] = 0;
					}
					uint32_t length = LIMIT(wLength, size);
					usb_control_send((uint8_t *)&buffer, length);
				} else{ request_valid =  stall; }
			} else {  request_valid = stall; }
		} break;
		
	   /*----------------------------------------------------------------------------
		*   SET ADDRESS
		*---------------------------------------------------------------------------*/
	    case RQST(Set_Address, HostToDevice, Device, Standard): { //b00000000
			
			
			
			//if(((wValue < 127) && (wValue > 0)) && (wIndex == 0) && (wLength == 0)){
			usb_control_send_zero_length_packet(); // Tell Host: Request Complete
			usb_set_address((uint8_t)wValue);      // Now we will begin responding to the new address
			ConfigState = address_state;
			//} else { 
				//	ConfigState = default_state;
				//	request_valid = undefined; 
			//}

		} break;
					


	   /*----------------------------------------------------------------------------
		*   SET CONFIGURATION
		*---------------------------------------------------------------------------*/				
		case RQST(Set_Configuration, HostToDevice, Device, Standard): { //b10000000
				
				
				
				//if (((wLength >> 8) == 0) && (wIndex == 0) && (wLength == 0)){
					
					currentConfigurationDescriptor = wValue;
					usb_control_send_zero_length_packet(); // Tell Host: Request Complete
				
					// If Config Descriptor Number is 1
					if (wValue == USB_CONFIGURATION_CDC){
						currentConfigurationDescriptor = 1;
						ConfigState = configured_state;
					
						// Get Number of Bytes in the CDC Configuration
						uint32_t len_bytes = reg_cdc.reg_cdc_config.wTotalLength;
					
						// Get Headers from the CDC Configuration
						USB_HEADER_DESCRIPTOR_TYPE *descriptors = (USB_HEADER_DESCRIPTOR_TYPE *)&reg_cdc;
					
						// Loop through Headers to Find & Setup Endpoints
						while(len_bytes){
							if (descriptors->bDescriptorType == USB_DESCRIPTOR_TYPE_ENDPOINT){
								usb_initialize_endpoints((USB_ENDPOINT_DESCRIPTORS_TYPE *)descriptors);
							}
							len_bytes = len_bytes - (descriptors->bLength);
							descriptors = (USB_HEADER_DESCRIPTOR_TYPE *)((uint8_t *)descriptors + descriptors->bLength);
						}
						usb_configuration_callback();
					}
					// Elif Config Descriptor is 0 (Control Config)
					//else if(wValue == USB_CONFIGURATION_CTRL){
					//	currentConfigurationDescriptor = 0;
					//	ConfigState = address_state;
					//} else { request_valid = undefined; }
				//} else { request_valid = undefined; }
			 // else { request_valid = undefined; }
		} break;

	   /*----------------------------------------------------------------------------
		*   Get CONFIGURATION
		*---------------------------------------------------------------------------*/								
		case RQST(Get_Configuration, DeviceToHost, Device, Standard): { //b10000000
			
			//if((wValue == 0) && (wIndex == 0) && (wLength == 1)){
				uint8_t config = currentConfigurationDescriptor;
				usb_control_send((uint8_t *)&config, sizeof(config));
			//} else { request_valid = undefined; }
		} break;				


	   /*----------------------------------------------------------------------------
		*   GET STATUS
		*---------------------------------------------------------------------------*/						
		case RQST(Get_Status, DeviceToHost, Device, Standard): { //b10000000
			uint16_t status = (GetStatus_Device_BusPowered | GetStatus_Device_WakeupDisabled);
			usb_control_send((uint8_t *)&status, sizeof(status));
		} break;
		
		case RQST(Get_Status, DeviceToHost, Interface, Standard): { //b10000001
			uint16_t status = GetStatus_Interface;
			usb_control_send((uint8_t *)&status, sizeof(status));
		} break;
		
		case RQST(Get_Status, DeviceToHost, Endpoint, Standard): { //b10000010

			uint8_t endpoint_dir = (uint8_t)(wIndex & 0x80);
			uint8_t endpoint_num = (uint8_t)(wIndex & 0x0F);
			
			uint16_t status = GetStatus_Endpoint_HaltDisabled;
					
			if (endpoint_dir == USB_DESCRIPTOR_ENDPOINT_bEndPointAddress_IN){ // If IN Endpoint
				if ((USB->DEVICE.DeviceEndpoint[endpoint_num].EPCFG.bit.EPTYPE1) != USB_DEVICE_EPCFG_EPTYPE_DISABLED){ // If Endpoint is setup
					status = (USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUS.reg & USB_DEVICE_EPSTATUS_STALLRQ1 ? 1 : 0);
					usb_control_send((uint8_t *)&status, sizeof(status));
				}
				else { request_valid = stall; }
			}
			else { // Else OUT Endpoint
				if ((USB->DEVICE.DeviceEndpoint[endpoint_num].EPCFG.bit.EPTYPE0) != USB_DEVICE_EPCFG_EPTYPE_DISABLED){ // If Endpoint is setup
					status = (USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUS.reg & USB_DEVICE_EPSTATUS_STALLRQ0 ? 1 : 0);
					usb_control_send((uint8_t *)&status, sizeof(status));
				}
				else { request_valid = stall; }
			}
		} break;
							
	   
	   
	   /*----------------------------------------------------------------------------
		*   Set Feature
		*---------------------------------------------------------------------------*/	

		case RQST(Set_Feature, HostToDevice, Device, Standard): { //b00000000
			request_valid = stall;
		} break;
		
		case RQST(Set_Feature, HostToDevice, Interface, Standard): { //b00000001
			usb_control_send_zero_length_packet();
		} break;

		case RQST(Set_Feature, HostToDevice, Endpoint, Standard): { //b00000010
			uint8_t endpoint_dir = (uint8_t)(wIndex & 0x80);
			uint8_t endpoint_num = (uint8_t)(wIndex & 0x0F);
				
			if((wValue == 0) && (endpoint_num > 0) && (is_endpoint_enabled(endpoint_num, endpoint_dir) != USB_DEVICE_EPCFG_EPTYPE_DISABLED)){
				if(endpoint_dir == 0x80){
					USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUSSET.bit. STALLRQ1 = 1;
				} else {
					USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUSSET.bit.STALLRQ0 = 1;
				}
				usb_control_send_zero_length_packet();
			} else { request_valid = stall; }
		} break;	   
	   
	   
	   /*----------------------------------------------------------------------------
		*   CLEAR FEATURE
		*---------------------------------------------------------------------------*/		   	   	   							
		case RQST(Clear_Feature, HostToDevice, Device, Standard): {	//b00000000
			request_valid = stall;
		} break;
		
		case RQST(Clear_Feature, HostToDevice, Interface, Standard): { //b00000001
			usb_control_send_zero_length_packet();
		} break;
		
		case RQST(Clear_Feature, HostToDevice, Endpoint, Standard): { //b00000010
			uint8_t endpoint_dir = (uint8_t)(wIndex & 0x80);
			uint8_t endpoint_num = (uint8_t)(wIndex & 0x0F);
				
			if((wValue == 0) && endpoint_num && (is_endpoint_enabled(endpoint_num, endpoint_dir) != USB_DEVICE_EPCFG_EPTYPE_DISABLED)){
					
				if(endpoint_dir == USB_DESCRIPTOR_ENDPOINT_bEndPointAddress_IN){
					if (USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUS.bit.STALLRQ1){	    // If Stall on Bank 1 Occurring
						USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUSCLR.bit.STALLRQ1 = 1;  // Clear Stall Bank 1 Status Flag
						if(USB->DEVICE.DeviceEndpoint[endpoint_num].EPINTFLAG.bit.STALL1){      // If Stall Interrupt Flag Set
							USB->DEVICE.DeviceEndpoint[endpoint_num].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_STALL1;  // Clear Stall Bank 1 Interrupt Flag
							USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUSCLR.bit.DTGLIN = 1;// Reset Data IN to Data0 Packet
						}
					}
				}else{
					if(USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUS.bit.STALLRQ0){			 // If Stall on Bank 0 Occurring
						USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUSCLR.bit.STALLRQ0 = 1;   // Clear Stall Bank 0 Status Flag
						if(USB->DEVICE.DeviceEndpoint[endpoint_num].EPINTFLAG.bit.STALL0){	     // If Stall Interrupt Flag Set
							USB->DEVICE.DeviceEndpoint[endpoint_num].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_STALL0;   // Clear Stall Bank 0 Interrupt Flag
							USB->DEVICE.DeviceEndpoint[endpoint_num].EPSTATUSCLR.bit.DTGLOUT = 1;// Reset Data OUT to Data0 Packet
						}
					}
				}
				usb_control_send_zero_length_packet();
			} else { request_valid = stall; }
		} break;
		
		
	   /*----------------------------------------------------------------------------
		*   SET LINE CODING
		*---------------------------------------------------------------------------*/			
		case RQST(USB_CDC_SET_LINE_CODING, HostToDevice, Interface, Class): {
			Set_Line_Coding_Flag = 1; // Service 
			//Set_Line_Coding_Length = LIMIT(wLength, sizeof(USB_CDC_LINECODING_TYPE));
		} break;
			
			
	   /*----------------------------------------------------------------------------
		*   GET LINE CODING
		*---------------------------------------------------------------------------*/				
		case RQST(USB_CDC_GET_LINE_CODING, DeviceToHost, Interface, Class): {
			uint16_t length = LIMIT(wLength, sizeof(USB_CDC_LINECODING_TYPE));
			usb_control_send((uint8_t *)&reg_line_coding, length);
		} break;

	   /*----------------------------------------------------------------------------
		*   SET LINE STATE
		*---------------------------------------------------------------------------*/				
		case RQST(USB_CDC_SET_LINE_STATE, HostToDevice, Interface, Class): {
			usb_set_vcom_linestate(wValue);
			usb_control_send_zero_length_packet();
		} break;
		
		/*
		case Set_Descriptor:
		case Get_Interface:	
		case Set_Interface:
		case SYNCH_Frame:
		*/

		default:{
			request_valid = stall;
			} break;
		

	}// END Switch
return request_valid;
} // END Func
