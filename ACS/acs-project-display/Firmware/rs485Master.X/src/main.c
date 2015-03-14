#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "uart.h"
#include "delays.h"

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2H
#pragma config WDTE = OFF      // Watchdog Timer Enable bit (WDT is controlled by SWDTEN bit of the WDTCON register)
//#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)
#pragma config LVP = OFF

#define FRAME_LENGTH    9

#define LED_DATA    PORTB
#define DIGIT1  PORTCbits.RC3
#define DIGIT2  PORTCbits.RC2
#define DIGIT3  PORTCbits.RC1
#define DIGIT4  PORTCbits.RC0

#define OUT_EN  PORTCbits.RC5

#define DISPLAY_CMD 0
#define SETID_CMD   2

void delay(int t);

uint8_t num[10]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6};

int main()
{
    uint8_t setid_frame[FRAME_LENGTH] = {0x31,0x31,0x31,0x32,0x30,0x30,0x30,0x30,0x31};
    uint8_t data[9] = {0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31};
    uint8_t tmp[9] = {0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32};
    uint8_t i = 0;

    open_uart(19200);
    TRISB = 0;
    ANSELH = 0x00;
    ANSEL = 0x00;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC5 = 0;
    LED_DATA = 0;
    DIGIT1 = 1;
    DIGIT2 = 1;
    DIGIT3 = 1;
    DIGIT4 = 1;
    OUT_EN = 1;
    write_frame(setid_frame, 9);
    delay_ms(100);
    while(1)
    {
        OUT_EN = 1;
        
        write_frame(data, 9);
        delay_ms(200);
        write_frame(tmp, 9);
        delay_ms(200);
    }
    return (EXIT_SUCCESS);
}


void delay(int t)
{
    register int i;
    for(i = 0; i < t; i++);
}