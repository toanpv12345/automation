#include "uart.h"
#include "delays.h"

void open_uart(uint16_t baudrate)
{
    uint8_t n;
    n = (uint8_t)((((uint32_t)FOS / baudrate) >> 4) - 1);
    TXSTA = 0;
    RCSTA = 0;
    SPBRG = 0;
    TXREG = 0;
    RCREG = 0;

    PIE1bits.RCIE = 0;
    PIE1bits.TXIE = 0;
    
    SPBRG = n;

    TXSTAbits.BRGH = 1;
    TXSTAbits.TXEN = 1;

    RCSTAbits.CREN = 1;
    RCSTAbits.SPEN = 1;

    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
}

void write_frame(uint8_t *frame)
{
    while(*frame)
    {
        TXREG = (*frame);
        while(!TXSTAbits.TRMT);
        frame++;
    }
}

