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

char message[] = "=============== CPU Temperature ===============";
volatile char BUFFER_TEMPERATURE[BUF_TEMP_LEN] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x46};

int main(void){
	
	/* Variable Declaration */
	static uint32_t time_srv = 0;
	uint32_t scheduler = 0;
	
	float temp_f = 0;
	
	
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
			temp_f = calc_temp(ADCx);
			floattostring(temp_f);
			TX_string(BUFFER_TEMPERATURE);
			start_adc_conversion();
		}
    }
} // END MAIN




