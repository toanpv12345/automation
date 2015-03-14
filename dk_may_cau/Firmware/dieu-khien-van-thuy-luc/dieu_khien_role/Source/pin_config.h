#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

#include "board.h"

#define OUT     LATB
#define OUT1    LATBbits.LATB7
#define OUT2    LATBbits.LATB6
#define OUT3    LATBbits.LATB5
#define OUT4    LATBbits.LATB4
#define OUT5    LATBbits.LATB3
#define OUT6    LATBbits.LATB2
#define OUT7    LATBbits.LATB1
#define OUT8    LATBbits.LATB0

#define LE1 LATCbits.LATC0
#define LE2 LATCbits.LATC1
#define LE3 LATCbits.LATC3
#define LE4 LATCbits.LATC2
#define LE5 LATCbits.LATC4

void io_config(void);

#endif // PIN_CONFIG_H