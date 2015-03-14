#ifndef UART_H
#define UART_H
#include "board.h"

#define TX_READY()    (TXSTAbits.TRMT)
#define TX_WRITE(ch8) {TXREG = (ch8);}
#define RX_READY()    (PIR1bits.RCIF)
#define RX_DATA()     (RCREG)

void uart_open(uint16_t baudrate);
void uart_write_frame(uint8_t *frame, uint8_t len);
void uart_write_char(uint8_t ch);

#endif