//---------------------------------------
// DS1820 driver
// www.08tdt.com/forum
//---------------------------------------

//---------------------------------------
// Delay mS function
//---------------------------------------
#include "ds1820.h"

#define DQ      RC0
#define DQ_DIR  TRISC0
#define OUT     0
#define IN      1
//----------------------------------------
// Reset DS1820
//----------------------------------------

bit ResetDS1820(void)
{
    static bit presence = 1;
    while(presence)
    {
        DQ = 0; //pull DQ line low
        __delay_us(29); // leave it low for about 490us
        DQ = 1; // allow line to return high
        __delay_us(3); // wait for presence 55 uS
        presence = DQ; // get presence signal
        __delay_us(25); // wait for end of timeslot 316 uS
    }
    return (presence); // presence signal returned
}

//-----------------------------------------
// Read one bit from DS1820
//-----------------------------------------

bit ReadBit(void)
{
    unsigned char i = 0;
    DQ = 0; // pull DQ low to start timeslot
    DQ = 1;
    for(i = 0; i < 3; i++); // delay 17 us from start of timeslot
    return (DQ); // return value of DQ line
}

//-----------------------------------------
// Write one bit to DS1820
//-----------------------------------------

void WriteBit(bit Dbit)
{
    unsigned char i = 0;
    DQ = 0;
    DQ = Dbit;
    __delay_us(5); // delay about 39 uS
    DQ = 1;
}

//-----------------------------------------
// Read 1 byte from DS1820
//-----------------------------------------

unsigned char ReadByte(void)
{
    unsigned char i;
    unsigned char Din = 0;
    for(i = 0; i < 8; i++)
    {
        Din |= ReadBit() ? 0x01 << i : Din; //FIFO
        __delay_us(6);
    }
    return (Din);
}

//-----------------------------------------
// Write 1 byte
//-----------------------------------------

void WriteByte(unsigned char Dout)
{
    unsigned char i;
    for(i = 0; i < 8; i++) // writes byte, one bit at a time
    {
        WriteBit((bit)(Dout & 0x1)); // write bit in temp into
        Dout = Dout >> 1;
    }
    __delay_us(5);
}

//-----------------------------------------
// config for ds18b20
//-----------------------------------------

void Config(unsigned char A_th, unsigned char A_tl, unsigned char mode)
{
    ResetDS1820();
    WriteByte(0x4e);
    WriteByte(A_th);
    WriteByte(A_tl);
    WriteByte(mode);
}

//-----------------------------------------
// Read temperature
//-----------------------------------------

void ReadTemp(unsigned char * buff)
{
    unsigned char n;
    // disable all interrupt
    ResetDS1820();
    WriteByte(0xcc); // skip ROM
    WriteByte(0x44); // perform temperature conversion
    //    while (ReadByte()==0xff); 	// wait for conversion complete
    ResetDS1820();
    WriteByte(0xcc); // skip ROM
    WriteByte(0xbe); // read the result

    for(n = 0; n < 9; n++) // read 9 bytes but, use only one byte
    {
        buff[n] = ReadByte(); // read DS1820
    }
}



