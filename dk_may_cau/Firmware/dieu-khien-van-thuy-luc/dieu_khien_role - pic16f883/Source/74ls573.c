#include "74ls573.h"
//#include "delay.h"

static uint8_t last_data0;
static uint8_t last_data1;
static uint8_t last_data2;
static uint8_t last_data3;
static uint8_t last_data4;

void chip_select(uint8_t cs)
{
    switch(cs)
    {
        case 0:
            LE1 = 1;
            LE1 = 0;
            LE2 = LE3 = LE4 = LE5 = 0;
            break;
        case 1:
            LE2 = 1;
            LE2 = 0;
            LE1 = LE3 = LE4 = LE5 = 0;
            break;
        case 2:
            LE3 = 1;
            LE3 = 0;
            LE2 = LE1 = LE4 = LE5 = 0;
            break;
        case 3:
            LE4 = 1;
            LE4 = 0;
            LE2 = LE3 = LE1 = LE5 = 0;
            break;
        case 4:
            LE5 = 1;
            LE5 = 0;
            LE2 = LE3 = LE4 = LE1 = 0;
            break;
        default:
            LE1 = 0;
            LE2 = 0;
            LE2 = 0;
            LE3 = 0;
            LE4 = 0;
            LE5 = 0;
            break;
    }
}

void output(uint8_t out_no, uint8_t status)
{
    switch(out_no)
    {
        case 0:
            OUT1 = (status ? 1 : 0);
            break;
        case 1:
            OUT2 = (status ? 1 : 0);
            break;
        case 2:
            OUT3 = (status ? 1 : 0);
            break;
        case 3:
            OUT4 = (status ? 1 : 0);
            break;
        case 4:
            OUT5 = (status ? 1 : 0);
            break;
        case 5:
            OUT6 = (status ? 1 : 0);
            break;
        case 6:
            OUT7 = (status ? 1 : 0);
            break;
        case 7:
            OUT8 = (status ? 1 : 0);
            break;
        default :
            break;
    }
}

void control_relay(int8_t relay_no, uint8_t status)
{   
    if((relay_no >= 0) && (relay_no <= 7))
    {
        OUT = last_data0;
        output(relay_no, status);
        chip_select(0);
        last_data0 = OUT;
        OUT = 0;        
    }
    else if((relay_no >= 8) && (relay_no <= 15))
    {
        OUT = last_data1;
        output(relay_no - 8, status);
        chip_select(1);
        last_data1 = OUT;
        OUT = 0;
    }
    else if((relay_no >= 16) && (relay_no <= 23))
    {
        OUT = last_data2;
        output(relay_no - 16, status);
        chip_select(2);
        last_data2 = OUT;
        OUT = 0;
    }
    else if((relay_no >= 24) && (relay_no <= 31))
    {
        OUT = last_data3;
        output(relay_no - 24, status);
        chip_select(3);
        last_data3 = OUT;
        OUT = 0;      
    }
    else if((relay_no >= 32) && (relay_no <= 39))
    {       
        OUT = last_data4;
        output(relay_no - 32, status);
        chip_select(4);
        last_data4 = OUT;
        OUT = 0;        
    }
    else
    {
        LE1 = 0;
        LE2 = 0;
        LE3 = 0;
        LE4 = 0;
        LE5 = 0;
    }
}

void reset_74ls573(void)
{    
    last_data0 = 0;
    last_data1 = 0;
    last_data2 = 0;
    last_data3 = 0;
    last_data4 = 0;
}

void all_relay(uint8_t status)
{
    if(status == 1)
    {
        OUT = 0xff;
        chip_select(0);
        chip_select(1);
        chip_select(2);
        chip_select(3);
        chip_select(4);
    }
    else
    {
        OUT = 0x00;
        chip_select(0);
        chip_select(1);
        chip_select(2);
        chip_select(3);
        chip_select(4);
    }
}