#include "timer.h"

void timer_init()
{
    T0CONbits.TMR0ON = 0;
    T0CONbits.T08BIT = 1; // 8 bit
    T0CONbits.T0SE = 0;
    T0CONbits.T0CS = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS = 0x06; // 1:128
}

void timer_on()
{
    T0CONbits.TMR0ON = 0;
    TMR0L = 0xFF - 156; // Set timer of 10ms
    INTCONbits.TMR0IF = 0;
    T0CONbits.TMR0ON = 1;
}

void timer_reset()
{
    T0CONbits.TMR0ON = 0;
    INTCONbits.TMR0IF = 0;
}

bool timer_is_timeout()
{
    return INTCONbits.TMR0IF;
}
