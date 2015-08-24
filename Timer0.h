/*
 * Timer0.h
 *
 *  Created on: Jul 15, 2015
 *      Author: Charles
 */

#ifndef TIMER0_H_
#define TIMER0_H_

#include <msp430.h>
#include <stdint.h>

#define PEDAL_SENSOR 42

uint8_t flagIdSensor;

inline void setupTimer0();
void adjustPedalTimerPeriod();
inline uint8_t getPedalTimeLsb();
inline uint8_t getPedalTimeMsb();



#endif /* TIMER0_H_ */
