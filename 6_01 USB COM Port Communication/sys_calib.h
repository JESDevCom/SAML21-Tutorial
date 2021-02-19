/*
 * sys_calib.h
 *
 * Created: 12/17/2020 4:44:40 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Definitions:
 *		This header file contains the commonly used definition declarations for calibration values
 *
 * Notes:
 *		[1] Internal High-Speed RAM is single cycle access at full speed.
 *		[2] Internal Low-power RAM is a single cycle access at full speed.
 *		[3] Internal High-Speed Flash with Read-While-Write (RWW) capability on a section of the array.
 *
 * Resources:
 *		[1] SAM L21 Family Data Sheet
 *				- Read "NVM Software Calibration Area Mapping" for Memory addresses (Pg. 45)
 */ 


#ifndef SYS_CALIB_H_
#define SYS_CALIB_H_

/****************************************************************
						   Definitions
****************************************************************/
// ------------------- Clock Calibration -----------------------
// Table 11-5 NVM Software Calibration Area Mapping (Pg. 45) [1]
#define NVMCAL_BIASREFBUF_MASK			0x00000007
#define NVMCAL_BIASREFBUF_POS			0
#define NVMCAL_BIASCOMP_MASK			0x00000038
#define NVMCAL_BIASCOMP_POS				3
#define NVMCAL_OSC32K_MASK				0x00001FC0
#define NVMCAL_OSC32K_POS				6
#define NVMCAL_USB_TRANSN_MASK			0x0003E000
#define NVMCAL_USB_TRANSN_POS			13
#define NVMCAL_USB_TRANSP_MASK			0x007C0000
#define NVMCAL_USB_TRANSP_POS			18
#define NVMCAL_USB_TRIM_MASK			0x03800000
#define NVMCAL_USB_TRIM_POS				23
#define DFLL48M_COURSE_CAL_MASK			0xFC000000
#define DFLL48M_COURSE_CAL_POS			26

#define CAL_DFLL48M_COURSE				(*((uint32_t *)NVMCTRL_OTP5 + (DFLL48M_COURSE_CAL_POS / 32)) & DFLL48M_COURSE_CAL_MASK) >> DFLL48M_COURSE_CAL_POS
#define CAL_DFLL48M_FINE				0x1FF 
#define CAL_USB_TRIM					(*((uint32_t *)NVMCTRL_OTP5) & NVMCAL_USB_TRIM_MASK)   >> NVMCAL_USB_TRIM_POS
#define CAL_USB_TRANSP					(*((uint32_t *)NVMCTRL_OTP5) & NVMCAL_USB_TRANSP_MASK) >> NVMCAL_USB_TRANSP_POS
#define CAL_USB_TRANSN					(*((uint32_t *)NVMCTRL_OTP5) & NVMCAL_USB_TRANSN_MASK) >> NVMCAL_USB_TRANSN_POS

// --------------- Temperature Calibration  -------------------
// Table 11-6 NVM Temperature Log Row (Pg. 45) [1]
#define CAL_BIASREFBUF	(*((uint32_t*)NVMCTRL_OTP5) & NVMCAL_BIASREFBUF_MASK) >> NVMCAL_BIASREFBUF_POS
#define CAL_BIASCOMP	(*((uint32_t*)NVMCTRL_OTP5) & NVMCAL_BIASCOMP_MASK)   >> NVMCAL_BIASCOMP_POS

#define ROOM_TEMP_INT_MASK	0x00000000000000FF
#define ROOM_TEMP_DEC_MASK	0x0000000000000F00
#define HOT_TEMP_INT_MASK	0x00000000000FF000
#define HOT_TEMP_DEC_MASK	0x0000000000F00000
#define ROOM_INT1V_MASK		0x00000000FF000000
#define HOT_INT1V_MASK		0x000000FF00000000
#define ROOM_ADC_MASK		0x000FFF0000000000
#define HOT_ADC_MASK		0xFFF0000000000000
#define ROOM_TEMP_INT_POS	0
#define ROOM_TEMP_DEC_POS	8
#define HOT_TEMP_INT_POS	12
#define HOT_TEMP_DEC_POS	20
#define ROOM_INT1V_POS		24
#define HOT_INT1V_POS		32
#define ROOM_ADC_POS		40
#define HOT_ADC_POS			52

#define NVMTemperatureLogRow	*((uint64_t*)NVMCTRL_TEMP_LOG) // 0x C65 A73 01 00 9 54 0 1E
#define NVMTEMP_ROOM_INT		(uint32_t)((NVMTemperatureLogRow & ROOM_TEMP_INT_MASK) >> ROOM_TEMP_INT_POS)
#define NVMTEMP_ROOM_DEC		(uint32_t)((NVMTemperatureLogRow & ROOM_TEMP_DEC_MASK) >> ROOM_TEMP_DEC_POS)
#define NVMTEMP_HOT_INT			(uint32_t)((NVMTemperatureLogRow & HOT_TEMP_INT_MASK)  >> HOT_TEMP_INT_POS)
#define NVMTEMP_HOT_DEC			(uint32_t)((NVMTemperatureLogRow & HOT_TEMP_DEC_MASK)  >> HOT_TEMP_DEC_POS)
#define NVMTEMP_INT1Vr			(int32_t)((NVMTemperatureLogRow & ROOM_INT1V_MASK)	   >> ROOM_INT1V_POS)
#define NVMTEMP_INT1Vh			(int32_t)((NVMTemperatureLogRow & HOT_INT1V_MASK)	   >> HOT_INT1V_POS)
#define ADCr					(uint32_t)((NVMTemperatureLogRow & ROOM_ADC_MASK)      >> ROOM_ADC_POS)
#define ADCh					(uint32_t)((NVMTemperatureLogRow & HOT_ADC_MASK)	   >> HOT_ADC_POS)

#define	tempH					(float)(NVMTEMP_HOT_INT  + ((float)NVMTEMP_HOT_DEC  / 10))
#define tempR					(float)(NVMTEMP_ROOM_INT + ((float)NVMTEMP_ROOM_DEC / 10))
#define INT1Vr					(float)(1 - ((float)NVMTEMP_INT1Vr / 1000))
#define INT1Vh					(float)(1 - ((float)NVMTEMP_INT1Vh / 1000))

// --------------- Temperature Calibration  -------------------
// Device Serial Number (Pg. 46) [1]
#define SerialNumberWord0			((uint32_t*) 0x0080A00C)
#define SerialNumberWord1			((uint32_t*) 0x0080A040)
#define SerialNumberWord2			((uint32_t*) 0x0080A044)
#define SerialNumberWord3			((uint32_t*) 0x0080A048)




#endif /* SYS_CALIB_H_ */