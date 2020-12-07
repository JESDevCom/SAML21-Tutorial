/*
 * uart_tx.h
 *
 * Created: 12/6/2020 6:13:28 PM
 * Author : John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 */ 


#ifndef UART_TX_H_
#define UART_TX_H_

/****************************************************************
							Header Files
****************************************************************/
#include "sam.h"

/****************************************************************
						 Function Prototypes
****************************************************************/
void TX_string(char *string);
void TX_char(char c);

#endif /* UART_TX_H_ */