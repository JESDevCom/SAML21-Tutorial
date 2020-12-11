/*
 * Project: CPU Frequency Change
 * Created: 12/6/2020 7:07:24 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 * 
 * Description:
 *		Change the clock frequency of the CPU to visually see the difference on the LED 0 (PB10) and port (PB13).
 *		We will be changing from a 4 MHz default startup clock to a user specified clock.
 * 
 * Results:
 *		We should start at this frequency:
 *			Freq = 4MHz /  ([2] * [1] * [1024] * [180]) = 10.85 [Hz]  <--- Checked Calc with Fluke 87V. Measured: 10.85 Hz @ PORT_PB10
 *
 *		If you try some of the different clocks:
 *			Freq = 16MHz / ([2] * [1] * [1024] * [180]) = 43.4 [Hz]   <--- Checked Calc with Fluke 87V. Measured: 43.21 Hz @ PORT_PB10
 *			Freq = 48MHz / ([2] * [1] * [1024] * [180]) = 130.2[Hz]   <--- Checked Calc with Fluke 87V. Measured: 130.0 Hz @ PORT_PB10
 *			Freq = 32768 / ([2] * [1] * [1024] * [180]) = 0.089[Hz]   <--- Seemed pretty slow. Period=11.25 [s]
 *
 *		You can also measure TC0 output at PORT_PB13. Measured: 43.20 Hz @ PORT_PB13, CPU=16MHz
 *
 *
 * Lesson Notes:
 *		(1) Changing CPU Clock frequencies
 *		(2) Using PMUX to Connect a Timer's signal to a pin on the board
 *		(3) Timer setup with interrupts
 *		(4) Verifying CPU frequency by toggling an LED
 *
 * Resources:
 *		[1] SAM L21 Family Data Sheet
 */ 


#include "main.h"


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	init_ports();	
	init_MCLK();
	init_timer0();
	
	/* Try changing the CPU frequency and notice the difference at LED 0 (PB_10) */
	stop_timer0();
	cpu_freq_sel(OSC4MHz); // Try: OSC48MHz, OSC16MHz, OSC12MHz, OSC8MHz, OSC4MHz, OSCULP32KHz, OSC32KHz, XOSC32KHz
	start_timer0();

    /* Replace with your application code */
    while (1) 
    {
    }
}
