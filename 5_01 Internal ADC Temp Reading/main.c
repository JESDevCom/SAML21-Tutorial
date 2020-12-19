/*
 * ADC Read Temp.c
 *
 * Created: 12/11/2020 4:45:23 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Read the internal temperature sensor inside the ADC and print results to PuTTY console.
 *
 * Hardware Usage:
 *		Host Computer OS: Windows 10
 *		MCU CPU: 4MHz Int OSC
 *		ADC0: Reads Temp Sensor
 *		USART0: Communicates with Console
 *		PuTTY Settings: 9600 Baud, 1 stop bit, no parity
 *
 *
 * Project Size:
 *		Optimization ~O0 (None):
 *				Program Memory Usage 	:	16636 bytes   6.2 % Full
 *				Data Memory Usage 		:	9424 bytes   23.0 % Full
 *		Optimization ~03 (Most):
 *				Program Memory Usage 	:	13364 bytes   4.9 % Full
 *				Data Memory Usage 		:	9416 bytes   23.0 % Full
 *
 * Conclusion:
 *		Temperature Sensor is reporting within specification of +/- 10 degrees Celsius (Pg. 1091)[1].
 *		The sensor did accurately read the room temp upon startup after the board had been off for 
 *		22 hours. This is good!
 * 
 *		After Running For Several Hours I saw:
 *		Current room temp is 23.3 [C] or 74 [F], and the sensor is reporting approximately 81 [F] or 27.22 [C].
 *		So the chip does heat-up during operation.
 *
 * Resources:
 *		[1] SAM L21 Family Data Sheet
 */ 

#include "main.h"

volatile uint32_t F_CPU = 4000000;
volatile uint8_t buff_index = 0;
volatile char RX_BUFFER[BUF_UART_RX_LEN] = {'\0'};
volatile char RX_CHAR = NULL_CHAR;
volatile uint32_t int_timer = 0;
volatile uint16_t ADCx = 0;
volatile char BUFFER_TEMPERATURE[BUF_TEMP_LEN] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x46};
	
char message[] = "=============== CPU Temperature ===============";


int main(void){
	
	/* Variable Declaration */
	static uint32_t time_srv = 0;
	uint32_t scheduler = 0;
	
    /* Initialize the SAM system */
    SystemInit();
	init_ports();
	cpu_freq_sel(OSC4MHz);
	init_timer(250); // 250Hz or 4 [ms]
	init_uart(uart_bps_9600);
	init_adc();
	TX_string(message);
	start_adc_conversion();

    /* Replace with your application code */
    while (1) 
    {
		scheduler = time_srv^int_timer;
		
		// -------------------------------------------- serviced every: 1.0 [s]
		if (scheduler & 0x100){
			time_srv = time_srv ^ 0x100;
			ledporttgl();
			floattostring(calc_temp(ADCx)); //calcs temp from ADC, then saves result to BUFFER_TEMPERATURE
			TX_string(BUFFER_TEMPERATURE);  // print to console
			start_adc_conversion();			// start another conversion, so we are ready for next service 1 second from now.
		}
    }
} // END MAIN




