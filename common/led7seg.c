#include "board.h"
#include "led7seg.h"

#define DATA_PORT   PORTB
#define DATA_DIR    TRISB

#define ANODE_PORT  PORTA
#define ANODE_TRIS  TRISA

#define ANODE_PIN_DIR1  TRISA0
#define ANODE_PIN_DIR2  TRISA1
#define ANODE_PIN_DIR3  TRISA2
#define ANODE_PIN_DIR4  TRISA3
#define ANODE_PIN_DIR5  TRISA4
#define ANODE_PIN_DIR6  TRISA5

#define ANODE_PIN1      RA0
#define ANODE_PIN2      RA1
#define ANODE_PIN3      RA2
#define ANODE_PIN4      RA3
#define ANODE_PIN5      RA4
#define ANODE_PIN6      RA5

#define NUM_OF_DIGIT    6

#define DELAY_TIME_MS   1

#define DELAY_LED()     __delay_us(300)

//uint8_t digit[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90}; // not ULN2803 buffer
//uint8_t digit[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f}; // ULN2803 buffer
uint8_t digit[10] = {0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6}; // ULN2803 buffer, a-->RB7

void led7seg_init()
{    
    DATA_DIR = 0x00;
    ANODE_PIN_DIR1 = 0;
    ANODE_PIN_DIR2 = 0;
    ANODE_PIN_DIR3 = 0;
    ANODE_PIN_DIR4 = 0;
    ANODE_PIN_DIR5 = 0;
    ANODE_PIN_DIR6 = 0;
    led7seg_clear();
}

void led7seg_clear()
{
    DATA_PORT = 0;
//    ANODE_PIN1 = 0;
//    ANODE_PIN2 = 0;
//    ANODE_PIN3 = 0;
//    ANODE_PIN4 = 0;
//    ANODE_PIN5 = 0;
//    ANODE_PIN6 = 0;
    ANODE_PORT = 0x00;
}
void led7seg_display(uint8_t* data)
{
    led7seg_clear();
    DATA_PORT = digit[data[0]];
    ANODE_PORT = 0x01;
//    ANODE_PIN1 = 1;
//    ANODE_PIN2 = 0;
//    ANODE_PIN3 = 0;
//    ANODE_PIN4 = 0;
//    ANODE_PIN5 = 0;
//    ANODE_PIN6 = 0;
    DELAY_LED();

    led7seg_clear();
    DATA_PORT = digit[data[1]];
    ANODE_PORT = 0x02;
//    ANODE_PIN1 = 0;
//    ANODE_PIN2 = 1;
//    ANODE_PIN3 = 0;
//    ANODE_PIN4 = 0;
//    ANODE_PIN5 = 0;
//    ANODE_PIN6 = 0;
    DELAY_LED();

    led7seg_clear();
    DATA_PORT = digit[data[2]];
    ANODE_PORT = 0x04;
//    ANODE_PIN1 = 0;
//    ANODE_PIN2 = 0;
//    ANODE_PIN3 = 1;
//    ANODE_PIN4 = 0;
//    ANODE_PIN5 = 0;
//    ANODE_PIN6 = 0;
    DELAY_LED();

    led7seg_clear();
    DATA_PORT = digit[data[3]];
    ANODE_PORT = 0x08;
//    ANODE_PIN1 = 0;
//    ANODE_PIN2 = 0;
//    ANODE_PIN3 = 0;
//    ANODE_PIN4 = 1;
//    ANODE_PIN5 = 0;
//    ANODE_PIN6 = 0;
    DELAY_LED();

    led7seg_clear();
    DATA_PORT = digit[data[4]];
    ANODE_PORT = 0x10;
//    ANODE_PIN1 = 0;
//    ANODE_PIN2 = 0;
//    ANODE_PIN3 = 0;
//    ANODE_PIN4 = 0;
//    ANODE_PIN5 = 1;
//    ANODE_PIN6 = 0;
    DELAY_LED();

    led7seg_clear();
    DATA_PORT = digit[data[5]];
    ANODE_PORT = 0x20;
//    ANODE_PIN1 = 0;
//    ANODE_PIN2 = 0;
//    ANODE_PIN3 = 0;
//    ANODE_PIN4 = 0;
//    ANODE_PIN5 = 0;
//    ANODE_PIN6 = 1;
    DELAY_LED();
    led7seg_clear();
}