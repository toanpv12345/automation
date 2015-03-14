#include "p74ls151.h"

void chip_select(uint8_t cs)
{
    switch (cs)
    {
        case 0:
            STBP1 = 0;
            STBP2 = 1;
            STBP3 = 1;
            STBP4 = 1;
            STBP5 = 1;
            break;
        case 1:
            STBP1 = 1;
            STBP2 = 0;
            STBP3 = 1;
            STBP4 = 1;
            STBP5 = 1;
            break;
        case 2:
            STBP1 = 1;
            STBP2 = 1;
            STBP3 = 0;
            STBP4 = 1;
            STBP5 = 1;
            break;
        case 3:
            STBP1 = 1;
            STBP2 = 1;
            STBP3 = 1;
            STBP4 = 0;
            STBP5 = 1;
            break;
        case 4:
            STBP1 = 1;
            STBP2 = 1;
            STBP3 = 1;
            STBP4 = 1;
            STBP5 = 0;
            break;
        default:
            STBP1 = 1;
            STBP2 = 1;
            STBP3 = 1;
            STBP4 = 1;
            STBP5 = 1;
            break;
    }
}

void input_select(uint8_t input_no)
{
    switch(input_no)
    {
        case 0:
            AP = 0;
            BP = 0;
            CP = 0;
            break;
        case 1:
            AP = 1;
            BP = 0;
            CP = 0;
            break;
        case 2:
            AP = 0;
            BP = 1;
            CP = 0;
            break;
        case 3:
            AP = 1;
            BP = 1;
            CP = 0;
            break;
        case 4:
            AP = 0;
            BP = 0;
            CP = 1;
            break;
        case 5:
            AP = 1;
            BP = 0;
            CP = 1;
            break;
        case 6:
            AP = 0;
            BP = 1;
            CP = 1;
            break;
        case 7:
            AP = 1;
            BP = 1;
            CP = 1;
            break;
        default:
            break;
    }
}

uint8_t scan_input(uint8_t cs, uint8_t input_no)
{
    uint8_t st;
    chip_select(cs);
    input_select(input_no);
    switch(cs)
    {
        case 0:
            st = TH1 & 0x01;
            break;
        case 1:
            st = TH2 & 0x01;
            break;
        case 2:
            st = TH3 & 0x01;
            break;
        case 3:
            st = TH4 & 0x01;
            break;
        case 4:
            st = TH5 & 0x01;
            break;
        default:
            break;
    }
    return st;
}