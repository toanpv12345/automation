#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

#include "board.h"

#define OUT     PORTB
#define OUT1    PORTBbits.RB7
#define OUT2    PORTBbits.RB6
#define OUT3    PORTBbits.RB5
#define OUT4    PORTBbits.RB4
#define OUT5    PORTBbits.RB3
#define OUT6    PORTBbits.RB2
#define OUT7    PORTBbits.RB1
#define OUT8    PORTBbits.RB0

#define LE1 PORTCbits.RC0
#define LE2 PORTCbits.RC1
#define LE3 PORTCbits.RC3
#define LE4 PORTCbits.RC2
#define LE5 PORTCbits.RC4

void io_config(void);

#endif // PIN_CONFIG_H