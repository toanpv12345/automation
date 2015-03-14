#include "pin_config.h"
void io_config()
{
    TRISB = 0;
    ANSEL = 0x00;
    ANSELH = 0x00;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC4 = 0;    
}
