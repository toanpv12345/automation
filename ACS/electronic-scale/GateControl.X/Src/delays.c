#include "delays.h"
#include "board.h"

#define DELAY1US() {T1CONbits.TMR1ON = 0;\
    TMR1H = ((0xFFFF - 3) >> 8) & 0xFF;\
    TMR1L = (0xFFFF - 3) & 0xFF;\
    PIR1bits.TMR1IF = 0;\
    T1CONbits.TMR1ON = 1;}

#define DELAY1MS() {T1CONbits.TMR1ON = 0;\
    TMR1H = ((0xFFFF - 3000) >> 8) & 0xFF;\
    TMR1L = (0xFFFF - 3000) & 0xFF;\
    PIR1bits.TMR1IF = 0;\
    T1CONbits.TMR1ON = 1;}

void timer_init()
{
    T1CON = 0;
    T1CONbits.T1CKPS0 = 0;
    T1CONbits.T1CKPS1 = 0; // Prescale = 1:1
    T1CONbits.TMR1CS = 0; // Fosc/4
}

void delay_us(int us)
{
    while(us)
    {
        DELAY1US();
        while(!PIR1bits.TMR1IF);
        us--;
    }
}

void delay_ms(int ms)
{
    while(ms)
    {
        DELAY1MS();
        while(!PIR1bits.TMR1IF);
        ms--;
    }
}

void delay_s(int s)
{
    int i;
    for(i = 0; i < s; i++)
    {
        delay_ms(1000);
    }
}