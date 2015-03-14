#include "uart.h"

void open_uart(uint16_t baudrate)
{
    uint16_t n;
    n = (uint16_t)((((uint32_t)FOS / baudrate) >> 2) - 1);
    TXSTA = 0;
    RCSTA = 0;
    BAUDCTL = 0;
    // Set baudrate
    BAUDCTLbits.BRG16 = 1;
    SPBRGH = (uint8_t)(n >> 8);
    SPBRG = (uint8_t)(n);
    
    TXSTAbits.SYNC = 0;
    TXSTAbits.TX9 = 0;
    TXSTAbits.SENDB = 0;
    TXSTAbits.BRGH = 1;
    TXSTAbits.TXEN = 1;

    RCSTAbits.RX9 = 0;
    RCSTAbits.ADDEN = 0;
    RCSTAbits.CREN = 1;
    RCSTAbits.SPEN = 1;

    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
}

void write_frame(uint8_t *frame, uint8_t len)
{
    while(len)
    {
        if(TXSTAbits.TRMT)
        {
            TXREG = *frame;
            frame++;
            len--;
        }
    }
}
