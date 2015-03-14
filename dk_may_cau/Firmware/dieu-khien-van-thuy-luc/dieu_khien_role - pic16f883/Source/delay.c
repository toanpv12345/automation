#include "delay.h"
#include "types.h"
#include <xc.h>
void delay_us(uint32_t t_us)
{
    uint32_t i;
    for(i = 0; i < t_us; i++)
    {
        NOP();
        NOP();
    };
}

void delay_ms(uint32_t t_ms)
{
    uint32_t i;
    for(i = 0; i < t_ms; i++)
    {
        delay_us(1000);
    }
}

void delay_s(uint32_t t_s)
{
    uint32_t i;
    for(i = 0; i < t_s; i++)
    {
        delay_ms(1000);
    }
}
