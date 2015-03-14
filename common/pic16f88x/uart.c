#include "uart.h"

void uart_open(uint16_t baudrate)
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

void uart_write_byte(uint8_t data)
{
     TXREG = data;
     while(!TXSTAbits.TRMT);
}

void uart_write_frame(uint8_t *frame, uint8_t len)
{
    while(len)
    {
        uart_write_byte(*frame);
        frame++;
        len--;
    }
}

void uart_disable(uint8_t disable)
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

void uart_write_string( const char* str)
{
    while(*str)
    {
        uart_write_byte(*str);
        str++;
    }
}

bool uart_is_received_data()
{
    return PIR1bits.RCIF;
}

uint8_t uart_read_byte()
{
    uint8_t data = 0;
    if(PIR1bits.RCIF)
    {
        data = RCREG;
    }
    return data;
}