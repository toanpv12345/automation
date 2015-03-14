#include "types.h"
#include "uart.h"
#include "delays.h"


// PIC16F886 Configuration Bit Settings

// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF     // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR21V   // Brown-out Reset Selection bit (Brown-out Reset set to 2.1V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

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
#define DATA_LENGTH     9
#define TIME_DELAY      1

void display();

uint8_t num[10] = {0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6};
//uint8_t num[10] = {0x03, 0x9f, 0x25, 0x0d, 0x99, 0x49, 0x40, 0x1f, 0x01, 0x09};

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

    TRISB = 0;
    ADCON1 = 0x07;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;
    PIE1bits.RCIE = 0;
    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 0;

    open_uart(9600);
    timer_init();
    LED_DATA = 0;
    DIGIT1 = 0;
    DIGIT2 = 0;
    DIGIT3 = 0;
    DIGIT4 = 0;
    DIGIT5 = 0;
    delay_ms(100);

//    OPTION_REGbits.T0CS = 0;// Fosc/4
//    OPTION_REGbits.PSA = 0;
//    OPTION_REGbits.PS0 = 1;
//    OPTION_REGbits.PS1 = 1;
//    OPTION_REGbits.PS2 = 1;
//    INTCONbits.T0IF = 0;
//    TMR0 = 0;
    
    while (1)
    {
        OUT_EN = 0;
        if(RX_READY())
        {
            data_frame[count] = RCREG;
            count++;
            if(data_frame[0] != 0x7E)
                count = 0;

            if(count == DATA_LENGTH)
            {
                count = 0;
                isComplete = true;
            }
        }
        if(isComplete)
        {
            disable_uart(true);
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
                    delay_ms(100);
                    
                   
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
                    
                    OUT_EN = 1;
                    disable_uart(false);
                    write_frame(data_frame);
                    disable_uart(true);
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
//        if(INTCONbits.T0IF == 1)
//        {
//            INTCONbits.T0IF = 0;
//            time++;
//            TMR0 = 0;
//        }
//        if(time < TIMEOUT_10P)
            display();
            disable_uart(false);
//        else
//        {
//            time = TIMEOUT_10P;
//            LED_DATA = 0x01;
//            DIGIT1 = 1;
//            DIGIT2 = 1;
//            DIGIT3 = 1;
//            DIGIT4 = 1;
//            DIGIT5 = 1;
//        }
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
