//library DS18B20
//Creat byt Tran Trong Tan
//================================================
#include "ds18b20.h"         

void ds18b20_reset(void)
{
    DQ_write();
    DQ = 0;
    __delay_us(480);
    DQ = 1;
    __delay_us(60);
    DQ_read();
    while(!DQ);
    DQ_write();
    DQ = 1;
    __delay_us(50);
}

unsigned char ds18b20_read(void)
{
    unsigned char i = 0;
    unsigned char dat = 0;

    for(i = 0; i < 8; i++)
    {
        DQ_write();
        DQ = 0;
        __delay_us(5);
        DQ_read();
        dat >>= 1;
        if(DQ)
            dat |= 0x80;
        DQ = 1;
        __delay_us(50);
    }
    return (dat);
}

void ds18b20_write(unsigned char data)
{
    unsigned char i = 0;
    DQ_write();
    for(i = 0; i < 8; i++)
    {
        DQ = 0;
        __delay_us(5);
        if((data & 0x01) == 0)
        {
            DQ = 0;
            __delay_us(120);
        }
        DQ = 1;
        __delay_us(45);
        data >>= 1;
    }
}

uint16_t ds18b20_read_temp(void)
{
    unsigned char a = 0;
    unsigned char b = 0;
    unsigned int nhiet_thuc;

    ds18b20_reset();
    ds18b20_write(SKIP_ROM);
    ds18b20_write(CONVERT_T);

    //__delay_ms(1);

    ds18b20_reset();
    ds18b20_write(SKIP_ROM);
    ds18b20_write(READ_SCRATCHPAD);

    //__delay_ms(1);

    a = ds18b20_read();
    b = ds18b20_read();
    //__delay_ms(1);

    //nhiet_thuc=b<<4;			//bo so le
    //nhiet_thuc|=(a&0xf0)>>4;

    nhiet_thuc = b << 8;
    nhiet_thuc |= a;

    return nhiet_thuc;
}

void ds18b20_init(uint8_t resolution)
{
    ds18b20_reset();
    ds18b20_write(SKIP_ROM);
    ds18b20_write(WRITE_SCRATCHPAD);
    ds18b20_write(0);
    ds18b20_write(0);
    ds18b20_write(resolution);

    __delay_ms(1);

    ds18b20_reset();
    ds18b20_write(SKIP_ROM);
    ds18b20_write(COPY_SCRATCHPAD);
    __delay_ms(20);
}