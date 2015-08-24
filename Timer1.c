/*
 * Timer.c
 *
 *  Created on: Jul 15, 2015
 *      Author: Charles
 */

#include "Timer1.h"
#include "common.h"

#define TIMER_OVERFLOW 32768
#define TIMER_MAX_DIVISOR 8
#define TIMER_MIN_DIVISOR 1

uint8_t flagIdSensor;
uint16_t timer_range = 4096;
uint16_t wheelTime = 0;
uint8_t divisor_timer = 1;
uint8_t flagIsNotAdjustable = 0;
uint16_t timeOverflow = 0;

#define _adjustRangeUp() timer_range = timer_range << 1; divisor_timer = divisor_timer << 1; TA1CCR0 = timer_range - 1;
#define _adjustRangeDown() timer_range = timer_range >> 1; divisor_timer = divisor_timer >> 1; TA1CCR0 = timer_range - 1;

inline void setupTimer1(){
	TA1CCR0 = timer_range - 1;
	TA1CTL |= TASSEL_1 | ID_3 | MC_1 | TACLR | TAIE;
	TA1CCTL1 |= CM_2 | SCS | CAP | CCIS_0 ;
	TA1CCTL1 |= CCIE;
}

void adjustTimerPeriod(){
	if(wheelTime == TIMER_OVERFLOW && divisor_timer < TIMER_MAX_DIVISOR){
		_adjustRangeUp();
	}
	else if(wheelTime < (timer_range >> 1) && divisor_timer > TIMER_MIN_DIVISOR){
		_adjustRangeDown();
	}
}

inline uint8_t getWheelTimeLsb(){
	return (uint8_t) wheelTime & 0x00FF;
}

inline uint8_t getWheelTimeMsb(){
	return (uint8_t) (wheelTime >> 8) & 0x00FF;
}

#pragma vector = TIMER1_A1_VECTOR
#pragma vector = TIMER1_A0_VECTOR
__interrupt void capture_overflow_timer1(){
	flagIdSensor = WHEEL_SENSOR;
	switch(TA1IV){
		case TA1IV_TAIFG:
			wheelTime = TIMER_OVERFLOW; /* Velocidade 0 */
			timeOverflow += timer_range;
			break;
		case TA1IV_TACCR1:
			wheelTime = TA1CCR1 + timeOverflow;
			timeOverflow = 0;
			TA1CTL |= TACLR; /* Zera o contador, pois ocorreu interrupção pelo reed switch. */
			break;
		default:
			break;
	}
	__low_power_mode_off_on_exit();
}

