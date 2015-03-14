#include "uart.h"
#include "delays.h"
#include <string.h>

void uartOpen(uint16_t baudrate)
{
    uint16_t n;
    n = (uint16_t)((((uint32_t)_XTAL_FREQ / baudrate) >> 2) - 1);
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

void uartWriteFrame(uint8_t *frame, uint8_t len)
{
    while(len)
    {
        TXREG = *frame;
        while(!TXSTAbits.TRMT);
        frame++;
        len--;
    }
}

void uartDisable(uint8_t disable)
{
    if(disable)
    {
        TXSTAbits.TXEN = 0;
        RCSTAbits.CREN = 0;
        RCSTAbits.SPEN = 0;
    }
    else
    {
        TXSTAbits.TXEN = 1;
        RCSTAbits.CREN = 1;
        RCSTAbits.SPEN = 1;
    }
}

void uartWriteString( const char* str)
{
    while(*str)
    {
        TXREG = *str;
        while(!TXSTAbits.TRMT);
        str++;
    }
}