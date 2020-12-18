/*
 * uart_rx.h
 *
 * Created: 12/6/2020 6:13:44 PM
 * Author : John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef UART_RX_H_
#define UART_RX_H_

/****************************************************************
							Header Files
****************************************************************/
#include "sam.h"


/****************************************************************
						 Global Variables
****************************************************************/
#define BUF_LEN			5


/****************************************************************
						 Global Variables
****************************************************************/
extern volatile char	RX_CHAR;
extern volatile char	BUFFER[BUF_LEN];
extern volatile uint8_t	buff_index;


/****************************************************************
						 Function Prototypes
****************************************************************/
char RX_char(void);
void RX_string(char c);

#endif /* UART_RX_H_ */