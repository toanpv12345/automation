#include "timer.h"

void timer1_init(uint8_t prescale, bool is_isr_enable)
{
    T1CON = 0;
    T1CONbits.T1CKPS = prescale;
//    T1CKPS0 = 1;
//    T1CKPS1 = 1; // Prescale = 1:8
    TMR1CS = 0; // Fosc/4
    TMR1GE = 0;
    TMR1ON = 0;
    TMR1IF = 0;
    TMR1H = 0;
    TMR1L = 0;
    TMR1IE = is_isr_enable;
}

void timer1_start(uint16_t cycle)
{
    TMR1ON = 0;
    TMR1IF = 0;
    if(cycle > 0)
    {
        TMR1H = ((0xFFFF - cycle) >> 8) & 0xFF;
        TMR1L = (0xFFFF - cycle) & 0xFF;
    }
    else
    {
        TMR1H = 0x00;
        TMR1L = 0x00;
    }
    TMR1IF = 0;
    TMR1ON = 1;
}

void timer1_start_ms(uint8_t ms)
{
    uint32_t cycle = ((ms*_XTAL_FREQ >> 2) >> T1CONbits.T1CKPS)/1000;
    if(cycle >= UINT16_MAX)
        cycle = UINT16_MAX;

    timer1_start((uint16_t)cycle);
}

bool timer1_is_timeout()
{
    return TMR1IF;
}

void timer1_stop()
{
    TMR1IF = 0;
    TMR1ON = 0;
}
void timer1_reset_irs_flag()
{
    TMR1IF = 0;
}

uint16_t timer1_get()
{
    uint16_t val = 0;
    val = ((TMR1H & 0x00FF) << 8) | (TMR1L & 0x00FF);
    return val;
}
