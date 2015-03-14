#include "pin_config.h"

void io_config()
{
    // Configure PORTC as input
    TRISCbits.TRISC0 = 1;
    TRISCbits.TRISC1 = 1;
    TRISCbits.TRISC2 = 1;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;

    // Configure PORTB and PORTA as output
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;

    // Configure pins as digital I/O
    ANSELbits.ANS0 = 0; // RA0
    ANSELbits.ANS1 = 0; // RA1
    ANSELHbits.ANS8 = 0; // RB2
    ANSELHbits.ANS9 = 0; // RB3
    ANSELHbits.ANS10 = 0; // RB1
    ANSELHbits.ANS11 = 0; // RB4
    ANSELHbits.ANS12 = 0; // RB0
}
