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
#define RX_DATA		RCREG

#ifdef	__cplusplus
extern "C" {
#endif

void uart_open(uint16_t baudrate);
void uart_write_byte(uint8_t data);
void uart_write_frame(uint8_t *frame, uint8_t len);
void uart_write_string( const char* str);
void uart_disable(uint8_t disable);
bool uart_is_received_data();
uint8_t uart_read_byte();

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

