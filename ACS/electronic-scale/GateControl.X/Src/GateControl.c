/* 
 * File:   GateControl.c
 * Author: Ken
 *
 * Created on January 27, 2013, 3:16 PM
 */
#include "delays.h"
#include "types.h"
#include "board.h"

#define BUTTON1     PORTAbits.RA0
#define BUTTON2     PORTAbits.RA1
#define BUTTON3     PORTAbits.RA2

#define OUT1        PORTBbits.RB0
#define OUT2        PORTBbits.RB1
#define OUT3        PORTBbits.RB2
#define OUT4        PORTBbits.RB3
#define OUT5        PORTBbits.RB4
#define OUT6        PORTBbits.RB5
#define OUT7        PORTBbits.RB6
#define OUT8        PORTBbits.RB7

/*
 * 
 */

void PinConfig();
void OutPin(uint8_t pin, bool status);

int main()
{
    bool isButton1 = false;
    bool isButton2 = false;
    bool isButton3 = false;
    uint32_t timeOut = 0;

    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS0 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS2 = 1;
    INTCONbits.T0IF = 0;

    PinConfig();
    timer_init();

    while(1)
    {
        if(!BUTTON1)
        {
            isButton1 = true;
        }
        if(!BUTTON2)
        {
            isButton2 = true;
        }
        if(!BUTTON3)
        {
            isButton3 = true;
        }

        if(isButton1)
        {
            isButton1 = false;
            OUT1 = 1;
            TMR0 = 0;
        }
        if(INTCONbits.T0IF)
        {
            INTCONbits.T0IF = 0;
            TMR0 = 0;
            timeOut++;
            // 23,6 ms for 1 overflow
            if(timeOut >= 5000)
            {
                OUT1 = 0;
                timeOut = 0;
            }
        }
        if(isButton2)
        {
            isButton2 = false;
            OUT2 = 0;
            OUT3 = 0;
            OUT4 = 0;
            OUT5 = 0;
            OUT6 = 0;
            OUT7 = 0;
            OUT8 = 0;
            delay_ms(1000);
            OUT2 = 1;
            delay_ms(1000);
            OUT3 = 1;
            delay_ms(1000);
            OUT4 = 1;
            delay_ms(1000);
            OUT5 = 1;
            delay_ms(1000);
            OUT6 = 1;
            delay_ms(1000);
            OUT7 = 1;
            delay_ms(1000);
            OUT8 = 1;
            delay_ms(1000);
            OUT2 = 0;
            OUT3 = 0;
            OUT4 = 0;
            OUT5 = 0;
            OUT6 = 0;
            OUT7 = 0;
            OUT8 = 0;

            delay_ms(1000);
            OUT2 = 1;
            delay_ms(1000);
            OUT3 = 1;
            delay_ms(1000);
            OUT4 = 1;
            delay_ms(1000);
            OUT5 = 1;
            delay_ms(1000);
            OUT6 = 1;
            delay_ms(1000);
            OUT7 = 1;
            delay_ms(1000);
            OUT8 = 1;
        }
        if(isButton3)
        {
            OUT1 = 1;
            OUT2 = 1;
            OUT3 = 1;
            OUT4 = 1;
            OUT5 = 1;
            OUT6 = 1;
            OUT7 = 1;
            OUT8 = 1;
        }
    }
    return 0;
}

void PinConfig()
{
    ADCON1 = 0x07;
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;

    TRISB = 0x00;

    OUT1 = 0;
    OUT2 = 0;
    OUT3 = 0;
    OUT4 = 0;
    OUT5 = 0;
    OUT6 = 0;
    OUT7 = 0;
    OUT8 = 0;
}
