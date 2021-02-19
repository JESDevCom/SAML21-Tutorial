/*
 * usb_utils.c
 *
 * Created: 12/27/2020 11:25:01 AM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Contains basic functions for transferring and receiving data
 */ 
#include "usb_utils.h"


void usb_control_send(uint8_t *packet, uint32_t length){
	// Send data from Flash 
	while(length){
		
		uint32_t transfer_size = (length > (uint32_t)reg_device.bMaxPacketSize0 ? (uint32_t)reg_device.bMaxPacketSize0 : (uint32_t)length);
		
		for (uint32_t x = 0; x < transfer_size; x++){
			USB_CTRL_I[x] = packet[x];
		}
		
		USB_EP_mem[0].in.ADDR.reg = (uint32_t)USB_CTRL_I;
		USB_EP_mem[0].in.PCKSIZE.bit.BYTE_COUNT = transfer_size;
		USB_EP_mem[0].in.PCKSIZE.bit.MULTI_PACKET_SIZE = 0;
			
		// Clear Transfer Complete Interrupt Flag
		USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1;
			
		// Set Ready to Transmit to Flag
		USB->DEVICE.DeviceEndpoint[0].EPSTATUSSET.bit.BK1RDY = 1;
			
		// Wait till Transmission Complete
		while(USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.bit.TRCPT1 == 0);
			
		length -= transfer_size;
		packet += transfer_size;

	}
}

void usb_control_stall(void){
	/* Send a stall handshake */
	USB->DEVICE.DeviceEndpoint[0].EPSTATUSSET.bit.STALLRQ1 = 1; 
}

void usb_control_send_zero_length_packet(void){
	/* Send a packet of length 0, which has no data inside */
	/* Used to tell sender that the transfer is complete */
	USB_EP_mem[0].in.PCKSIZE.bit.BYTE_COUNT = 0;
	USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1;
	USB->DEVICE.DeviceEndpoint[0].EPSTATUSSET.bit.BK1RDY = 1;
	
	// Wait till Transmission Complete
	while(USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.bit.TRCPT1 == 0);
}


void EndpointIN_DeviceSend(uint8_t ep_num, volatile uint8_t *data, uint32_t length){
	/* Transmit a packet of data */
	USB_EP_mem[ep_num].in.ADDR.reg = (uint32_t)data;
	USB_EP_mem[ep_num].in.PCKSIZE.bit.BYTE_COUNT = length;
	USB_EP_mem[ep_num].in.PCKSIZE.bit.MULTI_PACKET_SIZE = 0;
	
	// Clear Bank 1 (IN). This means the endpoint ready to send data to the Host.
	USB->DEVICE.DeviceEndpoint[ep_num].EPSTATUSSET.bit.BK1RDY = 1;
}


void EndpointOUT_DeviceRecv(uint8_t ep_num, volatile uint8_t *data, uint32_t length){
	/* Receive a packet of data */
	USB_EP_mem[ep_num].out.ADDR.reg = (uint32_t)data;
	USB_EP_mem[ep_num].out.PCKSIZE.bit.MULTI_PACKET_SIZE = length;
	USB_EP_mem[ep_num].out.PCKSIZE.bit.BYTE_COUNT = 0;

	
	// Clear Bank 0 (OUT). This means the endpoint is ready to receive data from the Host.
	USB->DEVICE.DeviceEndpoint[ep_num].EPSTATUSCLR.bit.BK0RDY = 1;
}



void usb_set_address(uint8_t address){
	/* 
	Set our device address provided by the host. In the SetAddress() Request, 
	the lower byte of the WValue field will contain the address
	*/
	USB->DEVICE.DADD.reg = (USB_DEVICE_DADD_ADDEN | USB_DEVICE_DADD_DADD(address));
}

