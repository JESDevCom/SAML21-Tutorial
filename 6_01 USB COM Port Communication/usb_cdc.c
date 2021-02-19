/*
 * usb_cdc.c
 *
 * Created: 1/3/2021 1:44:46 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Function computes 
 *
 * System Timer (SysTick):
 *      Timer counts down from the loaded value.
 *		SysTick Interrupt Period = (SysTick_LOAD + 1) * ClockPeriod
 *
 *      SysTick_CTRL:
 *            Bits: |31 ............. 17|   16    |15 ............ 3|        2      |   1     |    0    |
 *            Name: |     Reserved      |COUNTFLAG|     Reserved    |ClockSource Sel| TICKINT | ENABLE  |
 *                                                                     1=IntClk      1=EnInt   1=Enable
 *                                                                     0=ExtClk      0=DisInt  0=Disable
 *
 *		SysTick_LOAD: [Load a value](Writing to 0 disables SysTick, Independently of TICKINT)
 *			  Bits: |31 ...... 24|23 ....................... 0|
 *            Name: |  Reserved  |         Reload Val         |
 *
 *      SysTick_VAL: [Read Current Value](Writing to 0 clears the counter and COUNTFLAG to 0, causing the 
 *                   counter to reload on the next timer clock, but doesn't trigger an SysTick interrupt.
 *			  Bits: |31 ...... 24|23 ....................... 0|
 *            Name: |  Reserved  |        Current Value       |
 *
 *      SysTick_CALIB: [Read Only Reg](TENMS or 10ms) holds the reload value, which will yield a 10 ms period.
 *			  Bits: | 31  | 30 |29  ... 24|23 ............. 0|
 *            Name: |NOREF|SKEW| Reserved |      TENMS       |
 */  

#include "usb_cdc.h"

static uint8_t lineStatusReg = 0;
static uint8_t EP_COMM_BUSY = 0;
static alignas(4) USB_CDC_REQUEST_TYPE reg_cdc_request;

volatile alignas(4) cdc_data_t reg_cdc_data_OUT; // EP_OUT
volatile alignas(4) cdc_data_t reg_cdc_data_IN;  // EP_IN

static int app_system_time = 0;
static int app_uart_timeout = 0;
static uint8_t app_status = 0;
static int app_status_timeout = 0;



void usb_cdc_init(void){
	
	//usb_set_callback(USB_EP_3_CDC_COMM, init_CDC_CommEndpoint);
	//usb_set_callback(USB_EP_1_CDC_DATA, init_CDC_DataINEndpoint);
	//usb_set_callback(USB_EP_2_CDC_DATA, init_CDC_DataOUTEndpoint);
	
	reg_cdc_request.request.bmRequestType = (bmRequestType_DeviceToHost_val    | 
	                                         bmRequestType_Recipient_Interface | 
											 bmRequestType_Type_Class);
	reg_cdc_request.request.bRequest = USB_CDC_CIC_SERIAL_STATE;
	reg_cdc_request.request.wValue = 0x0000;
	reg_cdc_request.request.wIndex = 0x0000;
	reg_cdc_request.request.wLength = sizeof(uint16_t);
	reg_cdc_request.data = 0;
	
	// Initialize Flags
	lineStatusReg = 0;
	EP_COMM_BUSY = 0;
	reg_cdc_data_IN.status.FREE = 1;
	reg_cdc_data_OUT.status.FREE = 1;
	
	usb_setup_vcom(&reg_line_coding);
}

void usb_cdc_send(uint8_t *data, int size){
	EndpointIN_DeviceSend(USB_EP_1_CDC_DATA, data, size);
}

void usb_cdc_recv(uint8_t *data, int size){
	EndpointOUT_DeviceRecv(USB_EP_2_CDC_DATA, data, size);
}

void USB_CDC_SERIALSTATE_(uint8_t setclear, uint8_t lineStatus){
	switch(setclear){
		case 0x01:
			lineStatusReg |= lineStatus;
			break;
		default:
			lineStatusReg &= ~(lineStatus);
			break;
	}
	send_cdc_notification();
}

void send_cdc_notification(void){
	
	 if(EP_COMM_BUSY == 0){                          // If not busy
		 if(lineStatusReg != reg_cdc_request.data){  // If the new serial state != current serial state
			EP_COMM_BUSY = 1;                        // Set busy flag
			reg_cdc_request.data = lineStatusReg;    // Update data message
			EndpointIN_DeviceSend(USB_EP_3_CDC_COMM, (uint8_t *)&reg_cdc_request, sizeof(USB_CDC_REQUEST_TYPE)); // send
		 }
	 }
}


void init_CDC_CommEndpoint(uint32_t size){
	/* Initialize Data at Startup for CDC Communication Endpoint */
	uint16_t lineStatesSupported = (bBreak | bRingSignal | bFraming | bParity | bOverRun); 
	EP_COMM_BUSY = 0;
	
	reg_cdc_request.data &= ~lineStatesSupported;
	lineStatusReg &= ~lineStatesSupported;
	
	send_cdc_notification();
	(void)size;
}

void init_CDC_DataINEndpoint(uint32_t size){
	/* Initialize Data at Startup for CDC Data IN Endpoint */
	usb_cdc_send_callback();
	(void)size;
}

void init_CDC_DataOUTEndpoint(uint32_t size){
	/* Initialize Data at Startup for CDC Data OUT Endpoint */
	//USB_EP_mem[USB_EP_2_CDC_DATA].out.PCKSIZE.bit.BYTE_COUNT
	usb_cdc_recv_callback(size);
}


void sys_time_init(void){
	SysTick->VAL = 0;							// Clear Count Value
	SysTick->LOAD = (F_CPU / 1000ul);			// Loaded for a 10 [ms] timer
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;    // Enable
	app_system_time = 0;
}


void sys_time_task(void){
	if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
		app_system_time++;
	}
}


uint32_t get_system_time(void){
	return app_system_time;
}


void set_status_state(uint16_t lineStatus){
	if(lineStatus == 1){
		#if (STATUS_ACTIVE_STATE == 0)
		PORT->Group[0].DIRSET.reg = PORT_PA09;
		PORT->Group[0].PINCFG[9].bit.INEN = 1;
		PORT->Group[0].OUTCLR.reg = PORT_PA09;	
		#elif (STATUS_ACTIVE_STATE == 1)
		PORT->Group[0].DIRSET.reg = PORT_PA09;
		PORT->Group[0].PINCFG[9].bit.INEN = 1;
		PORT->Group[0].OUTSET.reg = PORT_PA09;
		#else
		PORT->Group[0].DIRCLR.reg = PORT_PA09;
		PORT->Group[0].PINCFG[9].bit.INEN = 1;
		PORT->Group[0].PINCFG[9].bit.PULLEN = 0;
		#endif
	} else {
		#if (STATUS_INACTIVE_STATE == 0)
		PORT->Group[0].DIRSET.reg = PORT_PA09;
		PORT->Group[0].PINCFG[9].bit.INEN = 1;
		PORT->Group[0].OUTCLR.reg = PORT_PA09;
		#elif (STATUS_INACTIVE_STATE  == 1)
		PORT->Group[0].DIRSET.reg = PORT_PA09;
		PORT->Group[0].PINCFG[9].bit.INEN = 1;
		PORT->Group[0].OUTSET.reg = PORT_PA09;
		#else
		PORT->Group[0].DIRCLR.reg = PORT_PA09;
		PORT->Group[0].PINCFG[9].bit.INEN = 1;
		PORT->Group[0].PINCFG[9].bit.PULLEN = 0;
		#endif
	}
}


void usb_set_vcom_linestate(uint16_t wValue){
	
	uint16_t lineStatus = wValue & USB_CDC_LINESTATE_wValue_DTEpresent;
	
	#if STATUS_RISING_EDGE > 0
		if ((app_status == 0) && (lineStatus == 1)){
			set_status_state(1);
			app_status_timeout = get_system_time() + STATUS_RISING_EDGE;
		}
	#endif
	
	#if STATUS_FALLING_EDGE > 0
		if((app_status == 1) && (lineStatus === 0)){
			set_status_state(1);
			app_status_timeout = get_system_time() + STATUS_FALLING_EDGE;
		}
	#endif
	
	#if ((STATUS_RISING_EDGE == 0) && (STATUS_FALLING_EDGE == 0))
		set_status_state(lineStatus);
	#endif
	
	app_status = (uint8_t)lineStatus;
}


void status_task(void){
	if(app_status_timeout && get_system_time()  > app_status_timeout){
		set_status_state(0);
		app_status_timeout = 0;
	}
}



void buffer_send(uint8_t endpoint_num){
	
	reg_cdc_data_IN.status.FREE = 0;
	
	//If our buffer is full, send a zero length packet afterwards to conclude transfer.
	if (USB_EP_CDC_DATA_BUF_LEN == reg_cdc_data_IN.indx){
		reg_cdc_data_IN.status.ZLP = 1;
	}else{
		reg_cdc_data_IN.status.ZLP = 0;
	}
	
	// Send buffer from device to host using CDC Communication Endpoint
	EndpointIN_DeviceSend(endpoint_num, reg_cdc_data_IN.buffer, reg_cdc_data_IN.indx);
	
	// reset index
	reg_cdc_data_IN.indx = 0; 
}

void usb_cdc_send_callback(void){
	reg_cdc_data_IN.status.FREE = 1;
}

void usb_cdc_recv_callback(uint32_t size){
	reg_cdc_data_OUT.indx = 0;
	reg_cdc_data_OUT.size = size;
}

void usb_configuration_callback(void){
	
	//EndpointOUT_DeviceRecv(USB_EP_2_CDC_DATA, reg_cdc_data_OUT.buffer, sizeof(reg_cdc_data_OUT.buffer));
	reg_cdc_data_IN.status.FREE = 0;
}

void SendDataViaUART_RecvFromHost(void){
	/* (1) Handles task of transmitting data over uart, that was sent from the host. 
	   (2) After uart transmission, this function requests more data. */
	
	while(reg_cdc_data_OUT.size){
		
		/* Write the data received by the host to the uart tx buffer to be sent */
		if(write_to_uart_tx_buffer(reg_cdc_data_OUT.buffer[reg_cdc_data_OUT.indx]) == 0){
			break;
		}
		
		/* The Index is reset and the Size is updated in ISR_USB.c upon receival of more data from host */
		reg_cdc_data_OUT.indx++;
		reg_cdc_data_OUT.size--;
		
		/* Once we have sent all of reg_cdc_data_OUT data out over uart,
		   we will tell the USB Controller that our device is ready to
		   receive more CDC data at Endpoint 2*/
		if(reg_cdc_data_OUT.size == 0){
			EndpointOUT_DeviceRecv(USB_EP_2_CDC_DATA, reg_cdc_data_OUT.buffer, sizeof(reg_cdc_data_OUT.buffer) );
		}
	}
}

void SendToHost_DataReceivedViaUart(void){
	/* (1) Handles task of copying data that was received via uart hardware to a CDC EP to be sent to host. */
	
	uint8_t temp_byte;
	
	// If Sending Buffer Free
	if(reg_cdc_data_IN.status.FREE == 1){
		
		/* While there is data in the uart received buffer, keep reading byte-by-byte 
		   and save to reg_cdc_data_IN.buffer */
		while(read_from_uart_rx_buffer(&temp_byte)){
			
			app_uart_timeout = get_system_time() + USART0_TIMEOUT;
			
			reg_cdc_data_IN.buffer[reg_cdc_data_IN.indx++] = temp_byte; // save
			
			/* Once reg_cdc_data_IN is filled with data received via uart,
			   we will tell the USB Controller that our device is ready to 
			   send this data to the host. */
			if (reg_cdc_data_IN.indx == USB_EP_CDC_DATA_BUF_LEN){
				buffer_send(USB_EP_1_CDC_DATA);
				break;
			}
		}
	}
}

void uart_timer_task(void){
	if(app_uart_timeout && get_system_time() > app_uart_timeout){
		if(reg_cdc_data_IN.status.ZLP || reg_cdc_data_IN.indx){
			buffer_send(USB_EP_1_CDC_DATA);
		}
		app_uart_timeout = 0;
	}
}




void SendStringToHost(char *string){
	
	if(reg_cdc_data_IN.status.FREE == 1){
		
		uint32_t size = strlen(string)+1; // +1 to include Null '\0' character
		reg_cdc_data_IN.indx = 0;

		
		app_uart_timeout = get_system_time() + USART0_TIMEOUT;
		
		// Fill reg_cdc_data_IN.buffer with chars from *string
		while(size){
			if (reg_cdc_data_IN.indx == USB_EP_CDC_DATA_BUF_LEN){
				break;
			}
			reg_cdc_data_IN.buffer[reg_cdc_data_IN.indx++] = *string++;
			size--;
		}
		buffer_send(USB_EP_1_CDC_DATA); // Send data in reg_cdc_data_IN to host.
	}
}

/*
__attribute__ ((weak)) void usb_cdc_control_line_state_update(uint32_t linestate){
	(void)linestate;
}
*/

