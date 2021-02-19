/*
 * ISR_USB.c
 *
 * Created: 12/21/2020 3:16:52 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Interrupt handler for the individual USB interrupts
 *
 * Notes:
 *		[1] Control Transfers [3]
 *				-	bi-directional transfers reserved for the host to send and request configuration 
 *					information to and from the device using the IN and OUT endpoint 0.
 *				-	Each Control transfer consists of 2 or more transactions. The maximum packet size
 *					for the control endpoint data is 8 bytes for low speed, 8/16/32/64 for full-speed.
 *				-   Control Transfers have 3 stages:
 *					(1) SETUP STAGE carries 8 bytes called the Setup packet, defining the request, 
 *                      and specifying how much data should be transferred in the Data stage.
 *                  (2) DATA STAGE is optional. If present it always starts with a transaction 
 *                      containing a Data1 packet. Then the transaction type alternates between Data0 and Data1 
 *                      until all required data have been transferred.
 *                  (3) STATUS STAGE is a transaction containing a zero-length Data1 packet. 
 *                      If the Data stage was IN, then the STATUS stage is OUT, and vice-versa.
 *		[2] Bulk Transfers [3]
 *				-	Bulk transfers take up all the bandwidth that is available after the other transfers have 
 *					completed. 
 *              -	The maximum packet size: 8/16/32/64 for full-speed
 *              -	for low-speed or full-speed endpoints the following is valid: if the bus is idle, multiple bulk transfers can take place in a single 1ms frame.
 *
 * Resources:
 *		[1] SAM L21 Family Data Sheet
 *			- USB Info Starts on Page 812
 *			- Device Registers Summary (Pg. 838)
 *		[2] Atmel SAM L21 Xplained Pro User Guide
 *		[3] https://www.keil.com/pack/doc/mw/USB/html/_u_s_b__control__transfers.html
 */ 
#include "ISR_USB.h"

/*
static void (*usb_control_recv_callback)(uint8_t *data, uint32_t size);

void usb_control_recv(void (*callback)(uint8_t *data, uint32_t size)){
	usb_control_recv_callback = callback;
}
*/

void usb_flag_handler(void){
	
	//--------------------------------------------------------- SERVICE: DEVICE RECEIVES RESET FROM HOST
	if (USB->DEVICE.INTFLAG.bit.EORST){
		USB->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_EORST;    // Cleared by writing a 1
		USB->DEVICE.DADD.reg = (USB_DEVICE_DADD_ADDEN | 0x00); // Enable Data Address after Reset to Store New Address Given by Host (Pg. 841) [1]
		
		// Clear all Device Endpoint Direction types (Pg. 852) [1]
		set_all_endpoint_types(USB_DEVICE_EPCFG_EPTYPE_DISABLED, USB_DEVICE_EPCFG_EPTYPE_DISABLED);
		
		// Config Endpoint 0 as Control
		set_endpoint_type(0, USB_DEVICE_EPCFG_EPTYPE_CONTROL, USB_DEVICE_EPCFG_EPTYPE_CONTROL);
		USB->DEVICE.DeviceEndpoint[USB_EP_0_CTRL].EPSTATUSCLR.bit.BK1RDY = 1; // clear IN (Bank 1) Ready
		USB->DEVICE.DeviceEndpoint[USB_EP_0_CTRL].EPSTATUSSET.bit.BK0RDY = 1; // set OUT (Bank 0) Ready
		
		// Config How Host Receives Packets from us (Pg. 863) [1]
		USB_EP_mem[0].in.PCKSIZE.bit.SIZE = USB_DEVICE_PCKSIZE_SIZE_64; // Width of our memory
		//USB_EP_mem[0].in.PCKSIZE.ADDR.reg = (uint32_t)USB_CTRL_I;
		
		// Config How Host Sends Packets to us (Pg. 863) [1]
		USB_EP_mem[0].out.ADDR.reg = (uint32_t)USB_CTRL_O; // Set the Base Address for Control Out
		USB_EP_mem[0].out.PCKSIZE.bit.SIZE = USB_DEVICE_PCKSIZE_SIZE_64; // Width of our memory
		USB_EP_mem[0].out.PCKSIZE.bit.MULTI_PACKET_SIZE = 64; // Width of our memory
		USB_EP_mem[0].out.PCKSIZE.bit.BYTE_COUNT = 0; // Holds the number of bytes received upon the last OUT transaction 

		
		// Default Configuration value: Reset to 0
		currentConfigurationDescriptor = 0;
		ConfigState = default_state; 
		
		// Enable: Received Setup Interrupt and Transfer Complete Interrupt for Bank 0 (Pg. 859) [1]
		USB->DEVICE.DeviceEndpoint[0].EPINTENSET.reg |= (USB_DEVICE_EPINTENSET_RXSTP | USB_DEVICE_EPINTENSET_TRCPT0);
	
	}
	
	//----------------------------------------------------------- SERVICE: DEVICE RECEIVES SETUP REQUEST FROM HOST
	// Flag cleared by writing a one 
	if (USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.bit.RXSTP){
		
		USB_REQUEST_TYPE *request = (USB_REQUEST_TYPE *)USB_CTRL_O;
		
		// If a Request not the correct size, then stall
		if (sizeof(USB_REQUEST_TYPE) == USB_EP_mem[0].out.PCKSIZE.bit.BYTE_COUNT) {
			
			// If Request not valid, then stall
			uint8_t request_status = usb_requests(request);
			
			if( request_status == valid){
				USB_EP_mem[0].out.PCKSIZE.bit.BYTE_COUNT = 0;
				USB->DEVICE.DeviceEndpoint[0].EPSTATUSCLR.bit.BK0RDY = 1;
				USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0;
			}
			else if( request_status == stall){
				endpoint_stall(USB_EP_0_CTRL, USB_EP_DIR_IN);
			}
		} else {
			endpoint_stall(USB_EP_0_CTRL, USB_EP_DIR_IN);
		}
		USB->DEVICE.DeviceEndpoint[USB_EP_0_CTRL].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_RXSTP; // Clear flag
	}
	
	//----------------------------------------------------------- SERVICE: TRANSFER FROM HOST TO DEVICE COMPLETE
	// Flag cleared by writing a one
	else if(USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.bit.TRCPT0){
		
		if(Set_Line_Coding_Flag == 1){
			Set_Line_Coding_Flag = 0;
			
			USB_CDC_LINECODING_TYPE *line_coding = (USB_CDC_LINECODING_TYPE *)USB_CTRL_O;
			
			if (sizeof(USB_CDC_LINECODING_TYPE) == USB_EP_mem[0].out.PCKSIZE.bit.BYTE_COUNT){
				usb_setup_vcom(line_coding); // setup uart interface for USB Virtual Comm.
				usb_control_send_zero_length_packet();
			}
		}
		USB->DEVICE.DeviceEndpoint[0].EPSTATUSSET.bit.BK0RDY = 1;
		USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0;    // Clear flag
	}
	
	//----------------------------------------------------------- SERVICE: Endpoint #1 - "CDC DATA IN"
	if(USB->DEVICE.EPINTSMRY.bit.EPINT1){

		if(USB->DEVICE.DeviceEndpoint[1].EPINTFLAG.bit.TRCPT0){
			USB->DEVICE.DeviceEndpoint[1].EPSTATUSSET.bit.BK0RDY = 1;	   // Device EP1 Set OUT Bank Ready
			USB->DEVICE.DeviceEndpoint[1].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0; // Clear Flag
			
			//usb_cdc_recv_callback(USB_EP_mem[USB_EP_1_CDC_DATA].out.PCKSIZE.bit.BYTE_COUNT);
			reg_cdc_data_IN.indx = 0;
			reg_cdc_data_IN.size = USB_EP_mem[USB_EP_1_CDC_DATA].out.PCKSIZE.bit.BYTE_COUNT;
		}

		if(USB->DEVICE.DeviceEndpoint[1].EPINTFLAG.bit.TRCPT1){
			USB->DEVICE.DeviceEndpoint[1].EPSTATUSCLR.bit.BK1RDY = 1;      // Device EP1 Set IN Bank Ready
			USB->DEVICE.DeviceEndpoint[1].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1; // Clear Flag
			
			//usb_cdc_send_callback();
			reg_cdc_data_IN.status.FREE = 1; // Set ready to load more data flag
		}
	}
	
	//----------------------------------------------------------- SERVICE: Endpoint #2 - "CDC DATA OUT"
	if(USB->DEVICE.EPINTSMRY.bit.EPINT2){
		if(USB->DEVICE.DeviceEndpoint[2].EPINTFLAG.bit.TRCPT0){
			USB->DEVICE.DeviceEndpoint[2].EPSTATUSSET.bit.BK0RDY = 1;	   // Device EP1 Set OUT Bank Ready
			USB->DEVICE.DeviceEndpoint[2].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0; // Clear Flag
			
			//usb_cdc_recv_callback();
			reg_cdc_data_OUT.indx = 0;
			reg_cdc_data_OUT.size = USB_EP_mem[USB_EP_2_CDC_DATA].out.PCKSIZE.bit.BYTE_COUNT;
		}

		if(USB->DEVICE.DeviceEndpoint[2].EPINTFLAG.bit.TRCPT1){
			USB->DEVICE.DeviceEndpoint[2].EPSTATUSSET.bit.BK1RDY = 1;      // Device EP1 Set IN Bank Ready
			USB->DEVICE.DeviceEndpoint[2].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1; // Clear Flag
			
			//usb_cdc_send_callback();
			reg_cdc_data_OUT.status.FREE = 1;
		}

	}
	
	//----------------------------------------------------------- SERVICE: Endpoint #3 - "CDC COMM IN"
	if(USB->DEVICE.EPINTSMRY.bit.EPINT3){
		/*
		if(USB->DEVICE.DeviceEndpoint[3].EPINTFLAG.bit.TRCPT0){
			USB->DEVICE.DeviceEndpoint[3].EPSTATUSSET.bit.BK0RDY = 1;	   // Device EP1 Set OUT Bank Ready
			USB->DEVICE.DeviceEndpoint[3].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0; // Clear Flag
			
			init_CDC_CommEndpoint(USB_EP_mem[USB_EP_3_CDC_COMM].out.PCKSIZE.bit.BYTE_COUNT)
		}
		*/
		if(USB->DEVICE.DeviceEndpoint[3].EPINTFLAG.bit.TRCPT1){
			USB->DEVICE.DeviceEndpoint[3].EPSTATUSCLR.bit.BK1RDY = 1;      // Device EP1 Set IN Bank Ready
			USB->DEVICE.DeviceEndpoint[3].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1; // Clear Flag
			
			//usb_cdc_send_callback();
			init_CDC_CommEndpoint(0);
		}			
	}
	
	//----------------------------------------------------------- SERVICE: TOGGLE LED
	/*
	if (USB->DEVICE.INTFLAG.bit.SOF){
		PORT->Group[portb].OUTTGL.reg = led; // Toggle LED at 1 [ms]
	}
	*/
	
}