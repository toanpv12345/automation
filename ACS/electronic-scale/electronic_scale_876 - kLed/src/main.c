#include "types.h"
#include "uart.h"
#include "delays.h"

// PIC16F876 Configuration Bit Settings

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config CP = OFF         // FLASH Program Memory Code Protection bits (Code protection off)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low Voltage In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection (Code Protection off)
#pragma config WRT = OFF        // FLASH Program Memory Write Enable (Unprotected program memory may not be written to by EECON control)

#define LED_DATA    PORTB
#define DIGIT5      PORTCbits.RC4
#define DIGIT4      PORTCbits.RC3
#define DIGIT3      PORTCbits.RC2
#define DIGIT2      PORTCbits.RC1
#define DIGIT1      PORTCbits.RC0

#define OUT_EN      PORTCbits.RC5

#define SENSOR1     PORTAbits.RA0
#define SENSOR2     PORTAbits.RA1

#define DISPLAY_CMD         '0'
#define RESPONE_CMD         '1'
#define REQUEST_SENSOR_CMD  '2'

#define HEADER_P    0
#define ID_P        1
#define CMD_P       2
#define DATA1_P     3
#define DATA2_P     4
#define DATA3_P     5
#define DATA4_P     6
#define DATA5_P     7
#define END_P       8

#define DEVICE_ID   '9'

#define TIMER	24 // 24ms for a time overflow

#define TIMEOUT_24S	1000
#define TIMEOUT_1P	2500
#define TIMEOUT_5P	12500
#define TIMEOUT_10P	25000
#define	TIMEOUT_15P	37500
#define TIMEOUT_20P	50000

#define FRAME_LENGTH    10
#define TIME_DELAY      5

void display();

uint8_t num[10] = {0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6};

uint8_t d1 = 0;
uint8_t d2 = 0;
uint8_t d3 = 0;
uint8_t d4 = 0;
uint8_t d5 = 0;

void main()
{
    uint8_t data_frame[FRAME_LENGTH] = {0};
    uint8_t cmd = 0;
    uint8_t count = 0;
    uint8_t isComplete = false;
    uint32_t time = 0;
    
    open_uart(9600);
    TRISB = 0;
    ADCON1 = 0x07;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 0;

    
    timer_init();
    LED_DATA = 0;
    DIGIT1 = 0;
    DIGIT2 = 0;
    DIGIT3 = 0;
    DIGIT4 = 0;
    DIGIT5 = 0;

    OUT_EN = 0;
    delay_ms(200);
    while (1)
    {
        //OUT_EN = 0;
        if(RX_READY())
        {
            data_frame[count] = RCREG;
            count++;
            if(data_frame[0] != 0x7E)
                count = 0;

            if(count == 9)
            {
                count = 0;
                isComplete = true;
            }
        }
        if(isComplete)
        {
            if((data_frame[END_P] == 0x7F) && (data_frame[ID_P] == DEVICE_ID))
            {
                cmd = data_frame[CMD_P];

                if(cmd == DISPLAY_CMD)
                {
                    d1 = data_frame[DATA1_P] - 48;
                    d2 = data_frame[DATA2_P] - 48;
                    d3 = data_frame[DATA3_P] - 48;
                    d4 = data_frame[DATA4_P] - 48;
                    d5 = data_frame[DATA5_P] - 48;
                }
                else if(cmd == REQUEST_SENSOR_CMD)
                {
                    delay_ms(10);
                    
                    data_frame[HEADER_P] = 0x7E;
                    data_frame[ID_P] = DEVICE_ID;
                    data_frame[CMD_P] = RESPONE_CMD;
                    data_frame[DATA1_P] = 0x30;
                    data_frame[DATA2_P] = 0x30;
                    data_frame[DATA3_P] = 0x30;
                    data_frame[DATA4_P] = (0x30 | SENSOR1);
                    data_frame[DATA5_P] = (0x30 | SENSOR2);
                    data_frame[END_P] = 0x7F;
                    data_frame[9] = 0;
                    
                    //OUT_EN = 1;
                    write_frame(data_frame);
                }
                cmd = 0;
                data_frame[HEADER_P] = 0;
                data_frame[ID_P] = 0;
                data_frame[CMD_P] = 0;
                data_frame[DATA1_P] = 0;
                data_frame[DATA2_P] = 0;
                data_frame[DATA3_P] = 0;
                data_frame[DATA4_P] = 0;
                data_frame[DATA5_P] = 0;
                data_frame[END_P] = 0;
                data_frame[9] = 0;
            }
            isComplete = false;
        }
        //display();
    }
}

void display()
{
    LED_DATA = 0;
    DIGIT1 = 0;
    DIGIT2 = 0;
    DIGIT3 = 0;
    DIGIT4 = 0;
    DIGIT5 = 0;
    LED_DATA = num[d1];
    DIGIT1 = 1;
    DIGIT2 = 0;
    DIGIT3 = 0;
    DIGIT4 = 0;
    DIGIT5 = 0;

    delay_us(TIME_DELAY);
    LED_DATA = 0;
    DIGIT1 = 0;
    DIGIT2 = 0;
    DIGIT3 = 0;
    DIGIT4 = 0;
    DIGIT5 = 0;

    LED_DATA = num[d2];
    DIGIT1 = 0;
    DIGIT2 = 1;
    DIGIT3 = 0;
    DIGIT4 = 0;
    DIGIT5 = 0;

    delay_us(TIME_DELAY);
    LED_DATA = 0;
    DIGIT1 = 0;
    DIGIT2 = 0;
    DIGIT3 = 0;
    DIGIT4 = 0;
    DIGIT5 = 0;

    LED_DATA = num[d3];
    DIGIT1 = 0;
    DIGIT2 = 0;
    DIGIT3 = 1;
    DIGIT4 = 0;
    DIGIT5 = 0;

    delay_us(TIME_DELAY);
    LED_DATA = 0;
    DIGIT1 = 0;
    DIGIT2 = 0;
    DIGIT3 = 0;
    DIGIT4 = 0;
    DIGIT5 = 0;

    LED_DATA = num[d4];
    DIGIT1 = 0;
    DIGIT2 = 0;
    DIGIT3 = 0;
    DIGIT4 = 1;
    DIGIT5 = 0;

    delay_us(TIME_DELAY);
    LED_DATA = 0;
    DIGIT1 = 0;
    DIGIT2 = 0;
    DIGIT3 = 0;
    DIGIT4 = 0;
    DIGIT5 = 0;

    LED_DATA = num[d5];
    DIGIT1 = 0;
    DIGIT2 = 0;
    DIGIT3 = 0;
    DIGIT4 = 0;
    DIGIT5 = 1;

    delay_us(TIME_DELAY);
    LED_DATA = 0;
    DIGIT1 = 0;
    DIGIT2 = 0;
    DIGIT3 = 0;
    DIGIT4 = 0;
    DIGIT5 = 0;
}

//void timer_init()
//{
//    T1CONbits.T1CKPS0 = 1; // 1:8
//    T1CONbits.T1CKPS1 = 1;
//    T1CONbits.T1OSCEN = 0; //
//    T1CONbits.TMR1CS = 0; // Timer mode
//}
//void delay_1ms()
//{
//    // Set the timer of 1ms
//    T1CONbits.TMR1ON = 0;
//    TMR1H = ((0xFFFF - 346) >> 8) & 0xFF;
//    TMR1L = (0xFFFF - 346) & 0xFF;
//    PIR1bits.TMR1IF = 0;
//    T1CONbits.TMR1ON = 1;
//}
//void delay_nms(int nms)
//{
//    while(nms)
//    {
//        delay_1ms();
//        while(!PIR1bits.TMR1IF);
//        PIR1bits.TMR1IF = 0;
//        nms--;
//    }
//}