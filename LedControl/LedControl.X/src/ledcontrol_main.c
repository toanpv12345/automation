/* 
 * File:   ledcontrol_main.c
 * Author: Ken
 *
 * Created on Ngày 13 tháng 12 năm 2013, 23:53
 */

#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "timer.h"
#include "interrupt.h"

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF);
__CONFIG(BOR4V_BOR40V & WRT_OFF);

/*
 * 
 */

#define RF_IN   RB0
#define SENSOR1 RB1
#define SENSOR2 RB2

#define RELAY_PORT  PORTC

#define RL1 RC0
#define RL2 RC1
#define RL3 RC2
#define RL4 RC3
#define RL5 RC4
#define RL6 RC5
#define RL7 RC6
#define RL8 RC7

#define CYCLE 37500 // 100ms/timeout at 12MHz, prescale 1:8

#define TIMEOUT_120S    1200
#define TIMEOUT_60S     600
#define TIMEOUT1         120
#define TIMEOUT2         300

#define TIME_DELAY      300 // ms

bool is_sensor2 = false;
bool is_sensor1 = false;
uint16_t count_timeout = 0;

void port_init();
void run_action1(uint8_t count);
void run_action2(uint8_t count);
void delay_func1();

void interrupt isr_all()
{
    if (timer1_is_timeout())
    {
        count_timeout++;
        timer1_start(CYCLE);
        
        if (is_sensor1 == true)
        {
            if (count_timeout >= TIMEOUT1)
            {
                is_sensor1 = false;
                RL1 = 0;
                count_timeout = 0;
                timer1_stop();
            }
        }
        if (is_sensor2 == true)
        {
            if (count_timeout >= TIMEOUT2)
            {
                RL2 = 0;
                is_sensor2 = false;
                count_timeout = 0;
                timer1_stop();

                RL3 = 0;
                delay_ms(TIME_DELAY);
                RL4 = 0;
                delay_ms(TIME_DELAY);
                RL5 = 0;
                delay_ms(TIME_DELAY);
                RL6 = 0;
                delay_ms(TIME_DELAY);
                RL7 = 0;
                delay_ms(TIME_DELAY);
                RL8 = 0;
            }
        }
    }
}

int main(int argc, char** argv)
{
    is_sensor2 = false;
    is_sensor1 = false;
    count_timeout = 0;

    port_init();
    timer1_init(T1_DIV_8, true);
    isr_global_enable(true);
  
    while (1)
    {
       if (!SENSOR1)
        {
            is_sensor1 = true;
            count_timeout = 0;
            timer1_start(CYCLE);
            run_action1(3);
        }
        if (!SENSOR2)
        {
            is_sensor2 = true;
            count_timeout = 0;
            timer1_start(CYCLE);
            run_action2(3);
        }
    }
    return (EXIT_SUCCESS);
}

void run_action1(uint8_t count)
{
    uint8_t i = 0;
    RELAY_PORT = 0x01; // on RL1, others is off

    for(i = 0; i < count; i++)
    {
        RL2 = 1;
        delay_func1();
        RL3 = 1;
        delay_func1();
        RL4 = 1;
        delay_func1();
        RL5 = 1;
        delay_func1();
        RL6 = 1;
        delay_func1();
        RL7 = 1;
        delay_func1();
        RL8 = 1;
        delay_func1();

        RL8 = 0;
        delay_func1();
        RL7 = 0;
        delay_func1();
        RL6 = 0;
        delay_func1();
        RL5 = 0;
        delay_func1();
        RL4 = 0;
        delay_func1();
        RL3 = 0;
        delay_func1();
        RL2 = 0;
        delay_func1();

        if(!SENSOR2)
            goto stop;
    }

    RL2 = 1;
    delay_func1();
    RL3 = 1;
    delay_func1();
    RL4 = 1;
    delay_func1();
    RL5 = 1;
    delay_func1();
    RL6 = 1;
    delay_func1();
    RL7 = 1;
    delay_func1();
    RL8 = 1;
    delay_func1();

    RL2 = 0;
    delay_func1();
    RL3 = 0;
    delay_func1();
    RL4 = 0;
    delay_func1();
    RL5 = 0;
    delay_func1();
    RL6 = 0;
    delay_func1();
    RL7 = 0;
    delay_func1();
    RL8 = 0;

    stop: return;
}

void run_action2(uint8_t count)
{
    uint8_t i = 0;
    RELAY_PORT = 0x02; // on RL2, others is off

    for(i = 0; i < count; i++)
    {
        RL3 = 1;
        delay_ms(TIME_DELAY);
        RL4 = 1;
        delay_ms(TIME_DELAY);
        RL5 = 1;
        delay_ms(TIME_DELAY);
        RL6 = 1;
        delay_ms(TIME_DELAY);
        RL7 = 1;
        delay_ms(TIME_DELAY);
        RL8 = 1;
        delay_ms(TIME_DELAY);
        RELAY_PORT = 0x02;
        delay_ms(TIME_DELAY);
    }
    RL3 = 1;
    delay_ms(TIME_DELAY);
    RL4 = 1;
    delay_ms(TIME_DELAY);
    RL5 = 1;
    delay_ms(TIME_DELAY);
    RL6 = 1;
    delay_ms(TIME_DELAY);
    RL7 = 1;
    delay_ms(TIME_DELAY);
    RL8 = 1;
}

void delay_func1()
{
    if(!SENSOR2)
        is_sensor2 = true;

    if(is_sensor2 == true)
        return;
    
    delay_ms(200);
}

void port_init()
{
    TRISC = 0x00;
    TRISB0 = 1;
    TRISB1 = 1;
    TRISB2 = 1;
    ANSEL = 0;
    ANSELH = 0;
    PORTC = 0;
    PORTB = 0;
}