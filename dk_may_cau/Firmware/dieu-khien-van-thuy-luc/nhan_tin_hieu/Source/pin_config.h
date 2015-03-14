#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H
#include "board.h"

#define TH1 PORTCbits.RC0
#define TH2 PORTCbits.RC1
#define TH3 PORTCbits.RC2
#define TH4 PORTCbits.RC3
#define TH5 PORTCbits.RC4

#define AP      LATBbits.LATB0
#define BP      LATBbits.LATB1
#define CP      LATBbits.LATB2
#define STBP1   LATBbits.LATB3
#define STBP2   LATBbits.LATB4
#define STBP3   LATBbits.LATB5
#define STBP4   LATAbits.LATA0
#define STBP5   LATAbits.LATA1

void io_config(void);

#endif // PIN_CONFIG_H