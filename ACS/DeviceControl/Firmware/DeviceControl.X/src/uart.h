/* 
 * File:   uart.h
 * Author: Ken
 *
 * Created on August 22, 2012, 9:26 AM
 */

#ifndef UART_H
#define	UART_H

#include "board.h"

#define TX_READY()     (TXSTAbits.TRMT)
#define TX_WRITE(ch8)  {TXREG = (ch8);}
#define RX_READY()     (PIR1bits.RCIF)
#define RX_DATA()      (RCREG)

#ifdef	__cplusplus
extern "C" {
#endif

void uartOpen(uint16_t baudrate);
void uartWriteFrame(uint8_t *frame, uint8_t len);
void uartWriteString( const char* str);
void uartDisable(uint8_t disable);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

