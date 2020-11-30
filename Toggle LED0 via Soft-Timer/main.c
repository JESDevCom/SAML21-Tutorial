/*
 * Blinky.c
 *
 * Created: 10/15/2020 5:37:11 PM
 * Author : John
 *
 * Description:
 *		Blink the built in LED on PB10
 *
 * Datasheet: SAM_L21_Family_DataSheet_DS60001477C
 *		Pg. 466 - Pin Configuration Summary
 *
 * Dev Board Datasheet: Atmel SAM L21 Xplained Pro [User Guide]
 *		Pg. 15 LED0 Pin Number in Table 4-3
 */


#include "sam.h"

#define LED0 PORT_PB10
#define LED0_GROUP 1
#define LED0_SET()		PORT->Group[LED0_GROUP].OUTSET.reg = LED0;
#define LED0_CLR()		PORT->Group[LED0_GROUP].OUTCLR.reg = LED0;
#define LED0_TGL()		PORT->Group[LED0_GROUP].OUTTGL.reg = LED0;
#define LED_ON()		PORT->Group[LED0_GROUP].DIRSET.reg = LED0;
#define LED_OFF()		PORT->Group[LED0_GROUP].DIRCLR.reg = LED0;

void delay(void);

void delay(void){

	for (volatile uint16_t x = 0; x < 65534; x++){
		__asm ("nop");
	}
}

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	LED0_CLR();
	//REG_PORT_DIR1 |= LED0; // Set Port Direction to Output

    /* Replace with your application code */
    while(1)
    {
		LED_ON();
		//REG_PORT_OUT1 |= LED0;
		delay();

		LED_OFF();
		//REG_PORT_OUT1 &= ~LED0;
		delay();

    }

}
