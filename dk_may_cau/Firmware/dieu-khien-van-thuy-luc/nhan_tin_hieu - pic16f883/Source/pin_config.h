#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H
#include "board.h"

#define TH1 PORTCbits.RC0
#define TH2 PORTCbits.RC1
#define TH3 PORTCbits.RC2
#define TH4 PORTCbits.RC3
#define TH5 PORTCbits.RC4

#define AP      PORTBbits.RB0
#define BP      PORTBbits.RB1
#define CP      PORTBbits.RB2
#define STBP1   PORTBbits.RB3
#define STBP2   PORTBbits.RB4
#define STBP3   PORTBbits.RB5
#define STBP4   PORTAbits.RA0
#define STBP5   PORTAbits.RA1

void io_config(void);

#endif // PIN_CONFIG_H