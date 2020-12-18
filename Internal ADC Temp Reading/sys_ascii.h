/*
 * sys_ascii.h
 *
 * Created: 12/17/2020 4:54:40 PM
 * Author: John
 * Board: Atmel SAML21 Xplained Pro
 * MCU: Microchip SAML21J18B
 * IDE: Atmel Studio v7
 * ASF: No
 *
 * Definitions:
 *		This header file contains ASCII values in HEX form.
 */ 


#ifndef SYS_ASCII_H_
#define SYS_ASCII_H_

#define NULL_CHAR		0x00
#define SOH_CHAR		0x01 /* Start of Heading */
#define STX_CHAR		0x02 /* Start of Text */
#define ETX_CHAR		0x03 /* End of Text */
#define EOT_CHAR		0x04 /* End of Transmission */
#define ENQ_CHAR		0x05 /* Inquiry */
#define ACK_CHAR		0x06 /* Acknowledge */
#define BEL_CHAR		0x07 /* Bell */
#define BACKSPACE_CHAR	0x08 
#define TAB_CHAR		0x09
#define LF_CHAR			0x0A /* Line Feed, new line */
#define VT_CHAR			0x0B /* Vertical Tab */
#define FF_CHAR			0x0C /* Form Feed, new page */
#define CR_CHAR			0x0D /* Carriage Return */
#define SHIFTOUT_CHAR	0x0E
#define SHIFTIN_CHAR	0x0F
#define NAK_CHAR		0x15 /* Negative Acknowledge */
#define SYN_CHAR		0x16 /* Synchronous Idle */
#define ETB_CHAR		0x17 /* End of transmission, block */
#define CAN_CHAR		0x18 /* Cancel */
#define EM_CHAR			0x19 /* End of Medium */
#define SUB_CHAR		0x1A /* Substitute */
#define ESC_CHAR		0x1B /* Escape */
#define FS_CHAR			0x1C /* File Separator */
#define GS_CHAR			0x1D /* Group Separator */
#define RS_CHAR			0x1E /* Record Separator */
#define US_CHAR			0x1F /* Unit Separator */

// ------------------------------------------------------ COMMON KEYBOARD ASCII CHARS
#define SPACE_CHAR			0x20
#define EXCLAM_CHAR			0x21
#define QUOTE_CHAR			0x22
#define HASHTAG_CHAR		0x23 /* I'm a millennial give me a break. Only telemarketers say pound */
#define DOLLAR_CHAR			0x24
#define PERCENT_CHAR		0x25
#define AND_CHAR			0x26 /* Ampre Sign. Get over it */
#define APOSTROPHE_CHAR		0x27
#define FORWARDQUOTE_CHAR	0x27
#define LEFT_PAREN_CHAR		0x28
#define RIGHT_PAREN_CHAR	0x29
#define ASTERISK_CHAR		0x2A
#define ADD_CHAR			0x2B
#define PLUS_CHAR			0x2B
#define COMMA_CHAR			0x2C
#define MINUS_CHAR			0x2D
#define SUBTRACT_CHAR		0x2D
#define PERIOD_CHAR			0x2E
#define BACKSLASH_CHAR		0x2F
#define ZERO_CHAR			0x30
#define ONE_CHAR			0x31
#define TWO_CHAR			0x32
#define THREE_CHAR			0x33
#define FOUR_CHAR			0x34
#define FIVE_CHAR			0x35
#define SIX_CHAR			0x36
#define SEVEN_CHAR			0x37
#define EIGHT_CHAR			0x38
#define NINE_CHAR			0x39
#define COLON_CHAR			0x3A
#define SEMICOLON_CHAR		0x3B
#define LESSTHAN_CHAR		0x3C
#define EQUAL_CHAR			0x3D
#define GREATERTHAN_CHAR	0x3E
#define QUESTION_CHAR		0x3F
#define AT_CHAR				0x40
#define A_CHAR				0x41
#define B_CHAR				0x42
#define C_CHAR				0x43
#define D_CHAR				0x44
#define E_CHAR				0x45
#define F_CHAR				0x46
#define G_CHAR				0x47
#define H_CHAR				0x48
#define I_CHAR				0x49
#define J_CHAR				0x4A
#define K_CHAR				0x4B
#define L_CHAR				0x4C
#define M_CHAR				0x4D
#define N_CHAR				0x4E
#define O_CHAR				0x4F
#define P_CHAR				0x50
#define Q_CHAR				0x51
#define R_CHAR				0x52
#define S_CHAR				0x53
#define T_CHAR				0x54
#define U_CHAR				0x55
#define V_CHAR				0x56
#define W_CHAR				0x57
#define X_CHAR				0x58
#define Y_CHAR				0x59
#define Z_CHAR				0x5A                                                                                                   
#define LEFTBRACKET_CHAR	0x5B
#define FORWARDSLASH_CHAR	0x5C
#define RIGHTBRACKET_CHAR	0x5D
#define UPCARROT_CHAR		0x5E
#define UNDERSCORE_CHAR		0x5F
#define BACKQUOTE_CHAR		0x60
#define a_char				0x61
#define b_char				0x62
#define c_char				0x63
#define d_char				0x64
#define e_char				0x65
#define f_char				0x66
#define g_char				0x67
#define h_char				0x68
#define i_char				0x69
#define j_char				0x6A
#define k_char				0x6B
#define l_char				0x6C
#define m_char				0x6D
#define n_char				0x6E
#define o_char				0x6F
#define p_char				0x70
#define q_char				0x71
#define r_char				0x72
#define s_char				0x73
#define t_char				0x74
#define u_charr				0x75
#define v_char				0x76
#define w_char				0x77
#define x_char				0x78
#define y_char				0x79
#define z_char				0x7A
#define LEFTCURLY_CHAR		0x7B
#define VERTICALBAR_CHAR	0x7C
#define RIGHTCURLY_CHAR		0x7D
#define TILDE_CHAR			0x7E
#define DELETE_CHAR			0x7F



#endif /* SYS_ASCII_H_ */