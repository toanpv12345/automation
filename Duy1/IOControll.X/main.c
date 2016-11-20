/*
 * File:   main.c
 * Author: Toanpv
 *
 * Created on 19 November 2016, 21:02
 */

#include <stdint.h>
#include <xc.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ  8000000
#endif

// CONFIG1
#pragma config OSC = HS         // Oscillator selection bits (HS oscillator)
#pragma config WDT = OFF        // Watchdog timer enable bit (WDT disabled)
#pragma config CP = OFF         // Code protection bit (Code protection off)

#define delay_ms(ms)    __delay_ms(ms)
#define delay_us(us)    __delay_us(us)

#define IN_PUT_A        RB3
#define IN_PUT_B        RB1

#define OUT_PUT_RL_A    RA1
#define OUT_PUT_RL_B    RA0

#define OUT_WAIT        RB4

void main(void) 
{
    uint8_t inputA = 0;
    uint8_t inputB = 0;
    uint8_t outputRL1 = 0;
    uint8_t outputRL2 = 0;
    
    uint32_t count = 0;
    
    TRISB = 0b00001010;
    TRISA = 0;
    //OPTION = 0b00000111; // F = 8M,  Timer0 module will increment every 0.128ms
    //TMR0 = 0;
    OUT_WAIT = 0;
    
    while(1)
    {
        count++; //increment every 0.5us
        
        if(IN_PUT_A)
        {
            while(IN_PUT_A);
            delay_ms(10);
            inputA++;
            if(inputA == 1)
            {
                OUT_PUT_RL_A = 1;
                outputRL1 = 1;
            }
            else if(inputA == 2)
            {
                OUT_PUT_RL_A = 0;
                outputRL1 = 0;
                inputA = 0;
            }
        }
        
        if(IN_PUT_B)
        {
            while(IN_PUT_B);
            delay_ms(10);
            inputB++;
            if(inputB == 1)
            {
                OUT_PUT_RL_B = 1;
                outputRL2 = 1;
            }
            else if(inputB == 2)
            {
                OUT_PUT_RL_B = 0;
                outputRL2 = 0;
                inputB = 0;
            }
        }
        
        if((outputRL1 == 1) || (outputRL2 == 1))
        {
            if(count < 20000) // on duration 200 ms
            {
                OUT_WAIT = 1;
            }
            else
            {
                OUT_WAIT = 0;
                if(count > 40000)
                    count = 0;
            }
        }
        else
        {
            if(count < 100000)
            {
                OUT_WAIT = 1;
            }
            else
            {
                OUT_WAIT = 0;
                if(count > 200000)
                    count = 0;
            }
        }
    }
}
