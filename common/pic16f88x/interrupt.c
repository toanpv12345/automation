#include "interrupt.h"

void isr_global_enable(bool is_enable)
{
    if (is_enable)
    {
        PEIE = 1;
        GIE = 1;
    }
    else
    {
        PEIE = 0;
        GIE = 0;
    }
}

void isr_external_enable(bool is_enable)
{
    INTF = 0;
    INTEDG = 1;
    if (is_enable)
    {
        INTE = 1;
    }
    else
    {
        INTE = 0;
    }
}

void isr_portb_change_enable(bool is_enable)
{
    RBIF = 0;
    if (is_enable)
    {
        RBIE = 1;
    }
    else
    {
        RBIE = 0;
    }
}

void isr_pinb_change_enable(bool is_enable, int8_t pin)
{
    switch (pin)
    {
        case 0:
            IOCB0 = is_enable;
            break;
        case 1:
            IOCB1 = is_enable;
            break;
        case 2:
            IOCB2 = is_enable;
            break;
        case 3:
            IOCB3 = is_enable;
            break;
        case 4:
            IOCB4 = is_enable;
            break;
        case 5:
            IOCB5 = is_enable;
            break;
        case 6:
            IOCB6 = is_enable;
            break;
        case 7:
            IOCB7 = is_enable;
            break;
    }
}

void isr_pinb_change_enable_all(bool is_enable)
{
    if(is_enable)
    {
        IOCB = 0xFF;
    }
    else
    {
        IOCB = 0;
    }
}