/*
 * UartBluetooth.c
 *
 *  Created on: Jul 15, 2015
 *      Author: Charles
 */
#include "UartBluetooth.h"

/*
 * Configura o módulo UART do MSP para falar com o módulo bluetooth.
 * Sem paridade, LSB Primeiro, 8 bits de dados, 1 bit de parada e modo assíncrono.
 */
inline void setupUart(){
    UCA0CTL1 = UCSWRST;
    UCA0CTL0 = 0x0;
    UCA0CTL1 |= UCSSEL_2;	/* Using SMCLK = 1MHz */
    UCA0BR0 = 104;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_1;
    UCA0CTL1 &= ~UCSWRST;
}

void sendByte(uint8_t byte){
	UCA0TXBUF = byte;
	while (UCA0STAT & UCBUSY);
}



