/*
 * sys_usb_macros.h
 *
 * Created: 12/31/2020 6:38:55 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Description:
 *		Contains the macros for configuring USB 2.0 Standard Descriptor Bit-Fields
 * 
 * Notes:
 *		Big Endian: (Internet)
 *					|	1A	|	2B	|	3C	|	4D	|	5E	|	6F	|	2D	|	5F	|
 *		  Address	|	0	|	1	|	2	|	3	|	4	|	5	|	6	|	7	|
 *
 *		Lil' Endian: (x86, ARM)
 *					|	5F	|	2D	|	6F	|	5E	|	4D	|	3C	|	2B	|	1A	|	
 *		  Address	|	0	|	1	|	2	|	3	|	4	|	5	|	6	|	7	|
 */ 


#ifndef SYS_USB_MACROS_H_
#define SYS_USB_MACROS_H_

/****************************************************************
						   Header Files
****************************************************************/ 
#include <stdio.h>
/****************************************************************
						   Definitions
****************************************************************/ 
/****************************************************************
						     Macros
****************************************************************/ 
// If Condition is true ? then X : otherwise value Y
#define LIMIT(a, b)     (((int)(a) > (int)(b)) ? (int)(b) : (int)(a))

#define MIN(a, b)		(((int)(a) < (int)(b)) ? (int)(a) : (int)(b))

#define LEN(a)			(sizeof(a) / sizeof(a[0]))

#define swap_u16(u16) ((uint16_t)(((uint16_t)(u16) >> 8) | ((uint16_t)(u16) << 8)))
#define swap_u32(u32) ((uint32_t)(((uint32_t)swap_u16((uint32_t)(u32) >> 16)) | ((uint32_t)swap_u16((uint32_t)(u32)) << 16)))

#define BE16(x)			swap_u16(x)					// 16-Bit Little Endian to Big Endian
#define BE32(x)			swap_u32(x)					// 32-Bit Little Endian to Big Endian
#define BE16B0(a)		((uint8_t)((a) >> 8))		// 16-Bit Big Endian Get Byte 0
#define BE16B1(a)		((uint8_t)((a) >> 0))		// 16-Bit Big Endian Get Byte 1

#define BE32B0(a)		((uint8_t)((a) >> 24))		// 32-Bit Big Endian Get Byte 0
#define BE32B1(a)		((uint8_t)((a) >> 16))		// 32-Bit Big Endian Get Byte 1
#define BE32B2(a)		((uint8_t)((a) >> 8))		// 32-Bit Big Endian Get Byte 2
#define BE32B3(a)		((uint8_t)((a) >> 0))		// 32-Bit Big Endian Get Byte 3

#define LE16B0(a)		((uint8_t)((a) >> 0))		// 16-Bit Little Endian Get Byte 0
#define LE16B1(a)		((uint8_t)((a) >> 8))		// 16-Bit Little Endian Get Byte 1
#define LE32B0(a)		((uint8_t)((a) >> 0))		// 32-Bit Little Endian Get Byte 0
#define LE32B1(a)		((uint8_t)((a) >> 8))		// 32-Bit Little Endian Get Byte 2
#define LE32B2(a)		((uint8_t)((a) >> 16))		// 32-Bit Little Endian Get Byte 3
#define LE32B3(a)		((uint8_t)((a) >> 24))		// 32-Bit Little Endian Get Byte 4



/****************************************************************
						   Structures
****************************************************************/ 



/****************************************************************
						    Globals
****************************************************************/ 


/****************************************************************
						Function Prototypes
****************************************************************/ 




#endif /* SYS_USB_MACROS_H_ */