/*
 * UartBluetooth.h
 *
 *  Created on: Jul 15, 2015
 *      Author: Charles
 */

#ifndef UARTBLUETOOTH_H_
#define UARTBLUETOOTH_H_

#include <msp430.h>
#include <stdint.h>

#define UART_RX BIT1
#define UART_TX BIT2

/*
 * Configura módulo serial Uart.
 */
inline void setupUart();
void sendByte(uint8_t byte);


#endif /* UARTBLUETOOTH_H_ */
