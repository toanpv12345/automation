#include "delay.h"
#include "types.h"
void delay_us(uint32_t t_us)
{
    uint32_t i;
    for(i = 0; i < t_us; i++)
    {
        Delay1TCY();
        Delay1TCY();
    };
}

void delay_ms(uint32_t t_ms)
{
    uint32_t i;
    for(i = 0; i < t_ms; i++)
    {
        Delay1KTCYx(2);
    }
}

void delay_s(uint32_t t_s)
{
    uint32_t i;
    for(i = 0; i < t_s; i++)
    {
        Delay10KTCYx(200);
    }
}
