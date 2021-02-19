/*
 * usb_uart.c
 *
 * Created: 1/1/2021 2:42:01 PM
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

/* ---- HEADER FILE ---------------------------------------------------------------------- */
#include "usb_uart.h"

/* ---- LOCAL FUNCTION PROTOTYPES -------------------------------------------------------- */
static void init_usb_uart_ports(void);
static uint8_t get_CHSIZE(uint8_t bDataBits);
static uint8_t get_FORM(uint8_t bPartityType);
static uint32_t get_PARITY_MODE(uint8_t bPartityType);
static uint32_t get_SBMODE(uint8_t bParityType);

/* ---- GLOBAL VARIABLE DECLARATIONS ----------------------------------------------------- */
volatile uart_buf_type sercom0_rx_buffer;
volatile uart_buf_type sercom0_tx_buffer;

void usb_setup_vcom(const USB_CDC_LINECODING_TYPE *linecoding){
	
	// Setup I/O
	init_usb_uart_ports();
	
	//Extract LineCoding()
	uint64_t baud		= (uint64_t)F_CPU / (16 * (linecoding->deDTERate));
	uint64_t frac_part	= (((uint64_t)F_CPU) / (linecoding->deDTERate) - 16 * baud) / 2;
	uint8_t PMODE		= get_PARITY_MODE(linecoding->bParityType);
	uint8_t FORM		= get_FORM(linecoding->bParityType);
	uint8_t CHSIZE		= get_CHSIZE(linecoding->bDataBits);
	uint8_t SBMODE		= get_SBMODE(linecoding->bParityType);
	
	// Clear UART Buffer Indices
	sercom0_rx_buffer.read	= 0;
	sercom0_rx_buffer.write = 0;
	sercom0_tx_buffer.read	= 0;
	sercom0_tx_buffer.write = 0;
	
	// Enable SERCOM0 Access to Master Clock (Pg. 146) [1]
	MCLK->APBCMASK.bit.SERCOM0_ = 1;
	
	// Enable SERCOM0 Access to Peripheral Channel Control (Pg. 137) [1]
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN; // Gen 0 Selected | Channel Enabled
	while((GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg  & GCLK_PCHCTRL_CHEN) == 0);           // Wait till complete
	
	// Reset SERCOM0 Disable Write-Protected Registers & Clear Settings
	SERCOM0->USART.CTRLA.bit.SWRST = 1;
	while(SERCOM0->USART.CTRLA.bit.SWRST);
	
	// ----------------------- Settings for SERCOM0 ----------------------- (Pg. 531) [1]
	// Default Mode: Synchronous, 1-Stop Bit, No Parity.
	SERCOM0->USART.CTRLA.reg = (SERCOM_USART_CTRLA_DORD		   | /* LSB is transmitted first */
								SERCOM_USART_CTRLA_MODE(1)	   | /* Use USART Internal Clock */
								SERCOM_USART_CTRLA_FORM(FORM)  | /* Initialize Parity or None */
								SERCOM_USART_CTRLA_SAMPR(1)	   | /* 16x Over Sampling w/ Fractional Baud Rate */
								SERCOM_USART_CTRLA_RXPO(1)	   | /* Receive  Data on SERCOM Pad 1	 */
								SERCOM_USART_CTRLA_TXPO(0));     /* Transmit Data on SERCOM Pad 0	 */
	
	// Default Mode: 8-bit Data, RX Enabled, TX Enabled	(Pg. 534) [1]			
	SERCOM0->USART.CTRLB.reg = (SERCOM_USART_CTRLB_CHSIZE(CHSIZE) | /* Channel Size 8-bits by default */
								SERCOM_USART_CTRLB_RXEN           | /* RX Enabled */
								SERCOM_USART_CTRLB_TXEN           | /* TX Enabled */
								PMODE                             | /* Parity Mode */ 
								SBMODE);                            /* Stop Bit Mode: 0=1bit, 1=2bits */
								
	while(SERCOM0->USART.SYNCBUSY.bit.CTRLB);
	
	// Set Baud Rate (Pg. 536) [1]
	SERCOM0->USART.BAUD.reg =  (SERCOM_USART_BAUD_BAUD((uint16_t)baud) | SERCOM_USART_BAUD_FRAC_BAUD((uint16_t)frac_part));
	
	SERCOM0->USART.CTRLA.bit.ENABLE = 1;
	
	// ------------------------- Interrupts for SERCOM0 --------------------- (Pg. 540) [1]
	SERCOM0->USART.INTENSET.reg = SERCOM_USART_INTENSET_RXC;
	NVIC_EnableIRQ(SERCOM0_IRQn);
}



static void init_usb_uart_ports(void){
	
	/* Initialize USART 0 TX */
	PORT->Group[porta].PINCFG[4].bit.PMUXEN = 1;		// Enable Peripheral MUX
	PORT->Group[porta].PMUX[4>>1].bit.PMUXE = PMUX_D;	// Set the PMUX bit (if pin is even, use PMUXE) (Pg. 486, 28) [2]
		
	/* Initialize USART 0 RX */
	PORT->Group[porta].PINCFG[5].bit.PULLEN = 0;		// Disable Pull-Up/Down
	PORT->Group[porta].PINCFG[5].bit.PMUXEN = 1;		// Enable Peripheral MUX
	PORT->Group[porta].PMUX[5>>1].bit.PMUXO = PMUX_D;	// Set the PMUX bit (if pin is odd, use PMUXO) (Pg. 486, 28) [2]

}



static uint8_t get_CHSIZE(uint8_t bDataBits){
	/* Convert the CDC Character Size value to the Hardware value */
	uint8_t CHSIZE = 0;
	switch(bDataBits){
		case USB_CDC_LINECODING_bDataBits_Width_8:
			CHSIZE = 0;
			break;
		case USB_CDC_LINECODING_bDataBits_Width_7:
			CHSIZE = 7;
			break;
		case USB_CDC_LINECODING_bDataBits_Width_6:
			CHSIZE = 6;
			break;
		case USB_CDC_LINECODING_bDataBits_Width_5:
			CHSIZE = 5;
			break;
		default:
			CHSIZE = 0; // Default 8-bit Character Size
	}
	return CHSIZE;
}



static uint8_t get_FORM(uint8_t bPartityType){
	/* Convert the CDC Parity value to the Hardware value */
	uint8_t FrameFormat = 0;
	if(bPartityType == USB_CDC_LINECODING_bParityType_None){
		FrameFormat = 0;
	}else{
		FrameFormat = 1; 
	}
	return FrameFormat;
}



static uint32_t get_PARITY_MODE(uint8_t bParityType){
	/* Convert the CDC Parity Odd/Even to the Hardware value */
	uint32_t PMODE = 0;
	if(bParityType == USB_CDC_LINECODING_bParityType_Odd){
		PMODE = SERCOM_USART_CTRLB_PMODE;
	}
	return PMODE;
}



static uint32_t get_SBMODE(uint8_t bParityType){
	/* Convert the CDC Parity One or Two Stop bits to the Hardware value 
	   The host can accept 1, 1.5, or 2 stop bits, but device only supports
	   1 or 2 stop bits. */
	
	uint32_t SBMODE = 0; // Default is 1 Stop Bit
	
	if(bParityType == USB_CDC_LINECODING_bCharFormat_2StopBit){
		SBMODE = SERCOM_USART_CTRLB_PMODE; // Two Stop Bits
	}
	return SBMODE;
}



uint8_t write_to_uart_tx_buffer(uint8_t byte){
	/* Write a byte from EP_CDC_DATA_IN 64-byte buffer to sercom0_tx_buffer 256-byte buffer */
	uint32_t write_index = (sercom0_tx_buffer.write + 1) % SERCOM0_BUF_LEN;
	sercom0_tx_buffer.status.TXC = 0;
	sercom0_tx_buffer.status.BUSY = 1;
	
	NVIC_DisableIRQ(SERCOM0_IRQn);
	
	// If we are not writing over an area that hasn't been read.
	if( write_index != sercom0_tx_buffer.read){
		sercom0_tx_buffer.data[sercom0_tx_buffer.write] = byte;
		sercom0_tx_buffer.write = write_index;
		sercom0_tx_buffer.status.TXC = 1;
		
		SERCOM0->USART.INTENSET.bit.DRE = 1; // Data Register Empty Interrupt Enable (Pg. 541) [1] 
	}
	
	sercom0_tx_buffer.status.BUSY = 0;
	NVIC_EnableIRQ(SERCOM0_IRQn);
	
	return sercom0_tx_buffer.status.TXC; // return status, 1=transmit complete, 0=transmit failed
}

uint8_t read_from_uart_rx_buffer(uint8_t *byte){
	/* Data received by UART0 is written to sercom0_rx_buffer in the ISR_SERCOM0.c
	   With this function we are reading the received data out of sercom0_rx_buffer
	   byte-by-byte into an Endpoint  */
	sercom0_rx_buffer.status.RXC = 0;
	sercom0_rx_buffer.status.BUSY = 1;
	
	NVIC_DisableIRQ(SERCOM0_IRQn);
	
	// If we are not reading where we are writing
	if(sercom0_rx_buffer.read != sercom0_rx_buffer.write){
		*byte = sercom0_rx_buffer.data[sercom0_rx_buffer.read];
		sercom0_rx_buffer.read = (sercom0_rx_buffer.read + 1) % SERCOM0_BUF_LEN;
		sercom0_rx_buffer.status.RXC = 1;
	}
	
	sercom0_rx_buffer.status.BUSY = 0;
	NVIC_EnableIRQ(SERCOM0_IRQn);
	
	return sercom0_rx_buffer.status.RXC; // return status, 1=read complete, 0=read failed
}