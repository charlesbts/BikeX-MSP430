/*
 * Timer.h
 *
 *  Created on: Jul 15, 2015
 *      Author: Charles
 */

#ifndef TIMER1_H_
#define TIMER1_H_

#include <msp430.h>
#include <stdint.h>

#define WHEEL_SENSOR 13

extern uint16_t wheelTime;
extern uint16_t timer_range;
extern uint8_t divisor_timer;

inline void setupTimer1();
void adjustWheelTimerPeriod();
inline uint8_t getWheelTimeLsb();
inline uint8_t getWheelTimeMsb();

#endif /* TIMER1_H_ */
