#include <msp430.h>
#include <stdint.h>
#include <intrinsics.h>

#include "common.h"
#include "Timer0.h"
#include "Timer1.h"
#include "UartBluetooth.h"

inline void setupOutput();
inline void setupInput();
inline void setupClock();


/*
 * main.c
 */
uint8_t flagIdSensor;
main(void) {

uint8_t flagIdAtual, timeMsb, timeLsb;

	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

	setupClock();
	setupInput();
	setupOutput();
	setupUart();
	setupTimer0();
	setupTimer1();

	__enable_interrupt();
	while(1){
		if(flagIdSensor == flagIdAtual)
			__low_power_mode_3();
		/**
		 * Implementação do protocolo de envio para o smartphone.
		 * Tamanho fixo: 3 bytes
		 * 1º byte: Identificador do sensor
		 * 2º byte: MSB da medida obtida
		 * 3º byte: LSB da medida obtida
		 */
		switch(flagIdSensor){
			case WHEEL_SENSOR:
				flagIdAtual = WHEEL_SENSOR;
				if(wheelTime > 190){
					__disable_interrupt();
					timeMsb = getWheelTimeMsb();
					timeLsb = getWheelTimeLsb();
					adjustWheelTimerPeriod();
					__enable_interrupt();
					sendByte(WHEEL_SENSOR);
					sendByte(timeMsb);
					sendByte(timeLsb);
				}
				break;
			case PEDAL_SENSOR:
				flagIdAtual = PEDAL_SENSOR;
				if(pedalTime > 800){
					__disable_interrupt();
					timeMsb = getPedalTimeMsb();
					timeLsb = getPedalTimeLsb();
					adjustPedalTimerPeriod();
					__enable_interrupt();
					sendByte(PEDAL_SENSOR);
					sendByte(timeMsb);
					sendByte(timeLsb);
				}
				break;
		}
	}

}

inline void setupClock(){
	DCOCTL = 0; // Select lowest DCOx and MODx settings<
	BCSCTL1 = CALBC1_1MHZ; // Set DCO
    DCOCTL = CALDCO_1MHZ;

    BCSCTL1 |= DIVA_0;
    BCSCTL3 |= XCAP_3;
    __delay_cycles(55000);   // let crystal stabilize
}

inline void setupInput(){
	P2DIR &= ~BIT1;
	P2SEL |= BIT1;
	P1DIR &= ~BIT1;
	P1SEL |= BIT1;
}

inline void setupOutput(){
	P1SEL |=  UART_TX;
	P1SEL2 |= UART_TX;
	P1DIR |= BIT0;
}

