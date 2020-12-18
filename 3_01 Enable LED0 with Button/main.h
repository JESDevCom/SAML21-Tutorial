/*
 * main.h
 *
 * Created: 11/13/2020 2:42:10 PM
 *  Author: John
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include "sam.h"
#include "delay.h"

#define led					PORT_PB10
#define button				PORT_PA02
#define porta				0
#define portb				1
#define ledset()			PORT->Group[portb].OUTSET.reg = led;
#define ledclr()			PORT->Group[portb].OUTCLR.reg = led;
#define ledtgl()			PORT->Group[portb].OUTTGL.reg = led;
#define ledon()				PORT->Group[portb].DIRSET.reg = led;
#define ledoff()			PORT->Group[portb].DIRCLR.reg = led;



#endif /* MAIN_H_ */