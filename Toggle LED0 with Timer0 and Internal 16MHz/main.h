/*
 * main.h
 *
 * Created: 10/19/2020 1:42:16 PM
 *  Author: John
 */ 


#ifndef MAIN_H_
#define MAIN_H_


#include "sam.h"
#include "delay.h"

#define led				PORT_PB10
#define porta			0
#define portb			1
#define ledset()		PORT->Group[portb].OUTSET.reg = led;
#define ledclr()		PORT->Group[portb].OUTCLR.reg = led;
#define ledtgl()		PORT->Group[portb].OUTTGL.reg = led;
#define ledon()			PORT->Group[portb].DIRSET.reg = led;
#define ledoff()		PORT->Group[portb].DIRCLR.reg = led;

#endif /* MAIN_H_ */