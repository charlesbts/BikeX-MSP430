/*
 * Timer1.c
 *
 *  Created on: Jul 15, 2015
 *      Author: Charles
 */

#include "Timer0.h"
#include "common.h"

#define TIMER_OVERFLOW 32768
#define TIMER_MAX_DIVISOR 8
#define TIMER_MIN_DIVISOR 1

uint8_t flagIdSensor;
uint16_t timerRangePedal = 4096;
uint16_t pedalTime = 0;
uint8_t divisorTimerPedal = 1;
uint16_t timeOverflowPedal = 0;

#define _adjustRangeUp() timerRangePedal = timerRangePedal << 1; divisorTimerPedal = divisorTimerPedal << 1; TA0CCR1 |= timerRangePedal - 1;
#define _adjustRangeDown() timerRangePedal = timerRangePedal >> 1; divisorTimerPedal = divisorTimerPedal >> 1; TA0CCR1 |= timerRangePedal - 1;

inline void setupTimer0(){
	TA0CCR1 = timerRangePedal - 1;
	TA0CTL |= TASSEL_1 | ID_3 | MC_2 | TACLR | TAIE;
	TA0CCTL0 |= CM_2 | SCS | CAP | CCIS_0;
	TA0CCTL0 |= CCIE;
	TA0CCTL1 |= CCIE;
}

void adjustPedalTimerPeriod(){
	if(pedalTime == TIMER_OVERFLOW && divisorTimerPedal < TIMER_MAX_DIVISOR){
		_adjustRangeUp();
	}
	else if(pedalTime < (timerRangePedal >> 1) && divisorTimerPedal > TIMER_MIN_DIVISOR){
		_adjustRangeDown();
	}
}

inline uint8_t getPedalTimeLsb(){
	return (uint8_t) pedalTime & 0x00FF;
}

inline uint8_t getPedalTimeMsb(){
	return (uint8_t) (pedalTime >> 8) & 0x00FF;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void overflow_timer0(){
	flagIdSensor = PEDAL_SENSOR;
	switch(TA0IV){
		case TA0IV_TACCR1:
			pedalTime = TIMER_OVERFLOW; /* Velocidade 0 */
			timeOverflowPedal += timerRangePedal;
			TA0CTL |= TACLR;
			break;
		default:
			break;
	}
	__low_power_mode_off_on_exit();
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void capture_timer0(){
	flagIdSensor = PEDAL_SENSOR;
	pedalTime = TA0CCR0 + timeOverflowPedal;
	TA0CTL |= TACLR;
	timeOverflowPedal = 0;
	__low_power_mode_off_on_exit();
}
