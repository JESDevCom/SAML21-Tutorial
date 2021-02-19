/*
 * init_USB_hw.c
 *
 * Created: 12/19/2020 3:55:21 PM
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
 *		USB Speed: Full-Speed 12 [Mbps]
 *
 * Initialization USB:
 *		[1] Initialize Ports
 *		[2] Enable MCU to operate at Power Level 2 (PL2) [done in cpu_freq_sel.c]
 *		[3] Clocks
 *				[3.1] Enable USB bus clock (CLK_USB_AHB) it's enabled by default
 *				[3.2] Start a 32K clock and enable both a 32k (used as ref) and 1k (used as SOF) output
 *				[3.3] Attach the 32K clock to GCLK1, this will be the reference clock for the 48MHz clock
 *				[3.4] Start a 48MHz clock, attach to USB Controller
 *				[3.5] Start a minimum 8 MHz clock, attach to USB Controller. We are attaching the 12MHz CPU clock.
 *		[4] Configure USB for DEVICE operation (since we are creating a mass storage device)
 *			[4.1] Disable USB 
 *			[4.2] Load calibration values	
 *			[4.3] Configure Speed (CTRLB.SPDCONF)
 *			[4.4] Configure Operating Mode (CTRLA.MODE)
 *			[4.5] Initialize Endpoint anywhere in RAM and save address (DEVICE.DESCADD.reg)
 *			[4.6] Configure interrupts, but do not enable interrupt vector yet.
 *			[4.6] Enable USB
 *
 *
 * Notes:
 *		[1] Requires an external 12 MHz Clock as reference for 480 MHz Phase Locked Loop (PPL)
 *			used to retrieve USB differential data at 480 Mbit/s (60MB/s)
 *		[2] After a hardware reset, the USB is disabled. The user should (CTRLA.ENABLE) in either
 *			device or host mode. The module clock is stopped in order to minimize power consumption.
 *			The internal states and registers of the device and host are reset. (Pg. 815) [1].
 *
 *
 * USB Frame Overview:
 *		[Start of Frame (SOF)][Control or Data Packets]
 *
 *		Example: [SOF][Interrupt][Bulk][Bulk][Bulk] [SOF][Bulk][Bulk][Bulk][Bulk][Bulk]
 *				 |<----------- 1 [ms] ----------->| |<------------ 1 [ms] ----------->|   Period: 1KHz
 *
 *
 * Resources:
 *		[1] SAM L21 Family Data Sheet
 *			- USB Info Starts on Page 812
 *			- Device Registers Summary (Pg. 838)
 *
 *		[2] Atmel SAM L21 Xplained Pro User Guide
 */
 
/* ----- HEADER FILE ----------------------------------------------------------------------- */
#include "init_USB_hw.h"

/* ----- LOCAL FUNCTION PROTOTYPES --------------------------------------------------------- */
static void write_serial_number(void);
static uint32_t writeNum(char *buf, uint32_t n, uint8_t full);
static void init_USB_ports(void);

/* ----- GLOBAL VARIABLES ------------------------------------------------------------------ */
volatile alignas(4) mem_DEVICE_DESC_BANK_type USB_EP_mem[8];
volatile alignas(4) uint8_t USB_CTRL_I[64]; // 64 Byte Buffer: Endpoint[0]	| IN  | Control
volatile alignas(4) uint8_t USB_CTRL_O[64]; // 64 Byte Buffer: Endpoint[0]	| OUT | Control
//static void (*usb_control_recv_callback)(uint8_t *data, uint32_t size);

volatile uint8_t currentConfigurationDescriptor = 0;
volatile uint8_t ConfigState = default_state; // State: default_state, address_state, configured_state
char USB_SERIAL_NUMBER[SERIAL_NUM_LENGTH];




void usb_hw_init(void){
	
	// Initialize I/O at Target USB Port
	init_USB_ports();
	
	// Enable Hardware and Peripheral Bus Clock
	//MCLK->AHBMASK.bit.USB_ = 1;		// 12 MHz CPU
	MCLK->APBBMASK.bit.USB_ = 1;	// Will be 48 MHz from GCLK0
	
	// Enable Peripheral Channel Connection (Pg. 138) [1] 
	GCLK->PCHCTRL[GCLK_PCHCTRL_USB].reg = GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_GEN_GCLK0;
	while(0 == (GCLK->PCHCTRL[USB_GCLK_ID].reg & GCLK_PCHCTRL_CHEN));
	
	// Reset USB
	USB->DEVICE.CTRLA.bit.SWRST = 1;
	while(USB->DEVICE.SYNCBUSY.bit.SWRST);
	
	// Load Calibration
	USB->DEVICE.PADCAL.reg = (USB_PADCAL_TRANSP(CAL_USB_TRANSP) | USB_PADCAL_TRANSN(CAL_USB_TRANSN) | USB_PADCAL_TRIM(CAL_USB_TRIM));

	erase_EndpointDescriptors(); // clear data for case when usb_hw_init() is called to reinitialize after a reset.

	/* Store Descriptor Address (Pg. 835) [1]
		Notes:
			[1] The least significant bits must be written to zero. We do this because the we are storing
				a whole word inside the DEVICE.DESCADD register, which defines the base address of the main
				USB descriptor in RAM.
			[2] Having the last 2 LSB zero means that the base address is divisible
				by 4, so the address is 32-bit. We are on a 32-bit architecture, so this should make sense.
			[3] Double (64-bit) Address end in hex [000], which is a number divisible by 8.
				Word (32-bit) addresses end in hex [0, 4, 8, and c], which is a number divisible by 4.
				Halfword (16-bit) addresses end in hex [0, 2, 4, 6, 8, a, c, and e], which is a number divisible by 2.
			*/
	USB->DEVICE.DESCADD.reg = (uint32_t)USB_EP_mem; // Set base address
	
	// Initialize Host Mode (Pg. 832) [1]
	USB->DEVICE.CTRLA.reg = USB_CTRLA_MODE_DEVICE | USB_CTRLA_RUNSTDBY;
	

	
	// COM Settings (Pg. 839) [1]
	USB->DEVICE.CTRLB.reg =  (	(0 << USB_DEVICE_CTRLB_SPDCONF_Pos) | // No handshake. LPM is not supported
								(0 << USB_DEVICE_CTRLB_GNAK_Pos)	| // The handshake packet reports the status of the USB transaction
								(0 << USB_DEVICE_CTRLB_OPMODE2_Pos)	| // The UTMI transceiver is in normal operation mode
								(0 << USB_DEVICE_CTRLB_TSTPCKT_Pos)	| // No test transmission of a test packet
								(0 << USB_DEVICE_CTRLB_NREPLY_Pos)	| // Disable the no reply feature. Any transaction to endpoint 0 will be handled according to the USB 2.0 standard
								(0 << USB_DEVICE_CTRLB_SPDCONF_Pos)	| // Full Speed mode
								(0 << USB_DEVICE_CTRLB_UPRSM_Pos));   // Writing a 1 will generate an upstream resume to the host for a remote wakeup
	attach_USB_to_host();
			
	// Enable USB Interrupts (Pg. 844) [1]
	USB->DEVICE.INTENSET.reg = (USB_DEVICE_INTENSET_EORST);		// Enable End of Reset Interrupt 
							  //USB_DEVICE_INTENSET_SOF);       // Enable Start of Frame Interrupt  

	
	// Enable to allow user to manually enable interrupt (Pg. 859) [1]
	//NVIC_EnableIRQ(USB_IRQn);
	USB->DEVICE.DeviceEndpoint[0].EPINTENSET.bit.RXSTP = 1;	  // Enable Received Setup Interrupt
	
	// Enable USB Controller to I/O
	enable_USB();
		
	// Clear Endpoint Type by disabling (Pg. 852) [1]
	set_all_endpoint_types(USB_DEVICE_EPCFG_EPTYPE_DISABLED, USB_DEVICE_EPCFG_EPTYPE_DISABLED);

	// Get Serial Number
	write_serial_number();   
}

static void init_USB_ports(void){
	/* Initialize GPIO ports for the target USB port pins  */
	
	/* Setup PA12 to Detect VBUS Voltage */
	//PORT->Group[porta].PINCFG[12].reg = PORT_PINCFG_PULLEN | PORT_PINCFG_INEN;
	//PORT->Group[porta].OUTSET.reg  = USB_VBUS; // 0=Pull-Down, 1=Pull-Up
	//PORT->Group[porta].DIRCLR.reg = USB_VBUS;
	
	/* Setup PB02 as Device ID */
	//PORT->Group[portb].OUTCLR.reg = USB_ID; // LOGIC 0
	//PORT->Group[portb].DIRSET.reg = USB_ID; // OUTPUT DIRECTION
	
	/* Setup PA24 as USB D- */
	//PORT->Group[porta].DIRSET.reg = USB_DN;	// OUTPUT
	//PORT->Group[porta].OUTCLR.reg = USB_DN;	// LOGIC 0
	PORT->Group[porta].PINCFG[24].reg |= PORT_PINCFG_PMUXEN;
	PORT->Group[porta].PMUX[24>>1].bit.PMUXE = PMUX_G; // Set the PMUX bit (if pin is even, use PMUXE) (Pg. 486, 28) [1]
	
	/* Setup PA25 as USB D+ */
	//PORT->Group[porta].DIRSET.reg = USB_DP; // OUTPUT
	//PORT->Group[porta].OUTCLR.reg = USB_DP; // LOGIC 0
	PORT->Group[porta].PINCFG[25].reg |= PORT_PINCFG_PMUXEN;
	PORT->Group[porta].PMUX[25>>1].bit.PMUXO = PMUX_G; // Set the PMUX bit (if pin is odd, use PMUXO) (Pg. 486, 28) [1]
	
}

void enable_USB(void){
	/* Call the enable USB after a reset. Can enable either host or device (Pg. 815) */
	USB->DEVICE.CTRLA.bit.ENABLE = 1;
	while(USB->DEVICE.SYNCBUSY.bit.ENABLE); // will go to zero once an enable or disable operation is complete (Pg. 832) [1]
}

void disable_USB(void){
	/* Call to disable USB */
	USB->DEVICE.CTRLA.bit.ENABLE = 0;
	while(USB->DEVICE.SYNCBUSY.bit.ENABLE); // will go to zero once an enable or disable operation is complete (Pg. 832) [1]
}

void attach_USB_to_host(void){
	/* Call to attach the USB device to start the USB communications from the USB host. */
	USB->DEVICE.CTRLB.bit.DETACH = 0;
}

void detach_USB_from_host(void){
	/* Call to detach the USB device to stop communications with the USB host */
	USB->DEVICE.CTRLB.bit.DETACH = 1;
}


static void write_serial_number(void){
	/* Write the serial number stored in NVM Memory to the array USB_SERIAL_NUMBER */
	uint32_t* addresses[4] = {SerialNumberWord0, SerialNumberWord1, SerialNumberWord2, SerialNumberWord3};
	uint32_t index = 0;
	for (uint8_t x=0; x<4; x++){
		index += writeNum(&(USB_SERIAL_NUMBER[index]), *(addresses[x]), 1);
	}
	USB_SERIAL_NUMBER[index] = '\0'; // Last element is NULL character.
}

static uint32_t writeNum(char *buf, uint32_t n, uint8_t full) {
	/* Takes a 32-bit number 'n', converts 'full' value to ASCII, and saves to '*buf'.
	   :param *buf: pointer to the address location of the buffer to save too.
	   :param n: 32-bit number we will be operating on.
	   :param full: (either 1 or 0), used to skip converting empty address space. 1=to convert all
	   :return i: total number of writes to *buf performed.
	   
	   Examples:
	    INPUT:
			*buf = [0, 0, 0, 0, 0, 0, 0, 0]
			n = 0x0080a00c
		RETURNS: 
			buf = ['0', '0', '8', '0', 'A', '0', '0', 'C'];
			i = 8
	*/
	
	uint32_t i = 0;
	uint32_t sh = 28;
	while (sh >= 0) {
		uint32_t d = (n >> sh) & 0xf;
		if (full  || d || sh == 0 || i) {
			buf[i++] = d > 9 ? A_CHAR + d - 10 : ZERO_CHAR + d;
		}
		if(sh == 0){ break; }
		sh -= 4;
	}
	return i;
}