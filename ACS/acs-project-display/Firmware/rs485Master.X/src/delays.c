#include "delays.h"
#include <xc.h>

#define DELAY1US() {NOP(); NOP(); NOP();}

void delay_us(int us)
{
    int i;
    for(i = 0; i < us; i++)
    {
        DELAY1US();
    }
}

void delay_ms(int ms)
{
    int i;
    for(i = 0; i < ms; i++)
    {
        delay_us(1000);
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