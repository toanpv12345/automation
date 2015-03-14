#include "types.h"
#include "uart.h"
#include "delays.h"
#include <xc.h>
#include "keypad.h"

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & IESO_OFF & FCMEN_OFF & LVP_OFF);
__CONFIG(BOR4V_BOR21V & WRT_OFF);

//#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
//#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
//#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)
//
//// CONFIG2H
//#pragma config WDTE = OFF      // Watchdog Timer Enable bit (WDT is controlled by SWDTEN bit of the WDTCON register)
////#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)
//#pragma config LVP = OFF

#define FRAME_LENGTH    9

#define KEYBOARD_DATA   PORTD

#define LED_DATA    PORTB
#define DIGIT1      PORTCbits.RC3
#define DIGIT2      PORTCbits.RC2
#define DIGIT3      PORTCbits.RC1
#define DIGIT4      PORTCbits.RC0
#define DIGITK1     PORTAbits.RA3
#define DIGITK2     PORTAbits.RA4

#define OUT_EN      PORTCbits.RC5

#define UNKNOW_CMD  0
#define DISPLAY_CMD '0'
#define CONFIRM_CMD '1'
#define SETID_CMD   '2'
#define GETID_CMD   '3'

#define HEADER_P    0
#define ID1_P       1
#define ID2_P       2
#define CMD_P       3
#define DATA1_P     4
#define DATA2_P     5
#define DATA3_P     6
#define DATA4_P     7
#define END_P       8

#define EEPROM_ADDR1 0x02
#define EEPROM_ADDR2 0x03

void display();

void EEPROM_putc(unsigned char address, unsigned char data);
unsigned char EEPROM_getc(unsigned char address);

//uint8_t num[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uint8_t num[12] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x79, 0x39};

uint8_t d1 = 0;
uint8_t d2 = 0;
uint8_t d3 = 0;
uint8_t d4 = 0;
uint8_t dk1 = 0;
uint8_t dk2 = 0;

void main()
{
    uint8_t data_frame[FRAME_LENGTH] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t id[2] = {0, 0};
    uint8_t keyVal = 0;

    uint8_t cmd = 0;
    uint8_t count = 0;
    uint8_t isComplete = false;
    uint8_t first = 0;
    uint8_t keyCount = 0;
    uint8_t enablePress = true;

    TRISB = 0;
    ANSELH = 0x00;
    ANSEL = 0x00;
    ANSELbits.ANS3 = 0;
    ANSELbits.ANS4 = 0;
    ADCON0bits.ADON = 0;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC5 = 0;
    TRISAbits.TRISA3 = 0;
    TRISAbits.TRISA4 = 0;
    EEDATA = 0;

    LED_DATA = 0xff;
    DIGIT1 = 0;
    DIGIT2 = 0;
    DIGIT3 = 0;
    DIGIT4 = 0;
    DIGITK1 = 0;
    DIGITK2 = 0;

    open_uart(9600);
    keypad_init();

    while (1)
    {
        keyVal = 0;
        OUT_EN = 0;
        if(RX_READY)
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
            if(data_frame[END_P] == 0x7F)
            {
                cmd = data_frame[CMD_P];

                if(cmd == SETID_CMD)
                {
                    if((data_frame[DATA1_P] == '0') && (data_frame[DATA2_P] == '0') &&
                            (data_frame[DATA3_P] == '0') && (data_frame[DATA4_P] == '0'))
                    {
                        EEPROM_putc(EEPROM_ADDR1, data_frame[ID1_P]);
                        EEPROM_putc(EEPROM_ADDR2, data_frame[ID2_P]);

                        OUT_EN = 1;
                        write_frame(data_frame, FRAME_LENGTH);
                        first = 0;
                    }else
                    {
                        id[0] = EEPROM_getc(EEPROM_ADDR1);
                        id[1] = EEPROM_getc(EEPROM_ADDR2);

                        if((id[0] == data_frame[ID1_P]) && (id[1] == data_frame[ID2_P]))
                        {
                            EEPROM_putc(0x00, data_frame[DATA3_P]);
                            EEPROM_putc(0x01, data_frame[DATA4_P]);

                            id[0] = data_frame[DATA3_P];
                            id[1] = data_frame[DATA4_P];

                            data_frame[HEADER_P] = 0x7E;
                            data_frame[ID1_P] = id[0];
                            data_frame[ID2_P] = id[1];
                            data_frame[CMD_P] = SETID_CMD;
                            data_frame[DATA1_P] = '0';
                            data_frame[DATA2_P] = '0';
                            data_frame[DATA3_P] = '0';
                            data_frame[DATA4_P] = '0';
                            data_frame[END_P] = 0x7F;
                            OUT_EN = 1;
                            write_frame(data_frame, FRAME_LENGTH);
                            first = 1;
                        }
                    }
                }else
                {
                    if(first == 0)
                    {
                        id[0] = EEPROM_getc(EEPROM_ADDR1);
                        id[1] = EEPROM_getc(EEPROM_ADDR2);
                        first = 1;
                    }
                    if((id[0] == data_frame[ID1_P]) && (id[1] == data_frame[ID2_P]))
                    {
                        if(cmd == DISPLAY_CMD)
                        {
                            d1 = data_frame[DATA1_P] - 48;
                            d2 = data_frame[DATA2_P] - 48;
                            d3 = data_frame[DATA3_P] - 48;
                            d4 = data_frame[DATA4_P] - 48;
                        }else if(cmd == CONFIRM_CMD)
                        {
                            dk1 = data_frame[DATA4_P] - 48;
                            dk2 = data_frame[DATA3_P] - 48;
                        }
                    }
                }
            }
            isComplete = false;
        }

        if(cmd == CONFIRM_CMD)
        {
            if(enablePress)
                keyVal = keypad_get();

            if((keyVal >= '0') && (keyVal <= '9'))
            {
                keyCount++;
                if(keyCount == 1)
                {
                    dk2 = 0;
                    dk1 = (keyVal - 48);
                }else if(keyCount == 2)
                {
                    dk2 = dk1;
                    dk1 = (keyVal - 48);
                    enablePress = false;
                }else if(keyCount == 255)
                    keyCount = 0;
                keyVal = 0;
            }

            if(!enablePress)
            {
                PORTD = 0b00001011;
                if(!PORTDbits.RD4)
                {
                    while (!PORTDbits.RD4)
                    {
                        DIGIT1 = DIGIT2 = DIGIT3 = DIGIT4 = DIGITK1 = DIGITK2 = 1;
                    }
                    delay_us(120);
                    keyVal = 'E';
                }
                PORTD = 0b00000111;
                if(!PORTDbits.RD4)
                {
                    while (!PORTDbits.RD4)
                    {
                        DIGIT1 = DIGIT2 = DIGIT3 = DIGIT4 = DIGITK1 = DIGITK2 = 1;
                    }
                    delay_us(120);
                    keyVal = 'C';
                }
            }

            if(keyVal == 'C')
            {
                dk1 = 0;
                dk2 = 0;
                keyVal = 0;
                keyCount = 0;
                enablePress = true;
                //cmd = 0;
            }
            else if(keyVal == 'E')
            {
                keyVal = 0;
                keyCount = 0;
                cmd = UNKNOW_CMD;
                enablePress = true;
                data_frame[HEADER_P] = 0x7E;
                data_frame[ID1_P] = id[0];
                data_frame[ID2_P] = id[1];
                data_frame[CMD_P] = CONFIRM_CMD;
                data_frame[DATA1_P] = '0';
                data_frame[DATA2_P] = '0';
                data_frame[DATA3_P] = (dk2 + 48);
                data_frame[DATA4_P] = (dk1 + 48);
                data_frame[END_P] = 0x7F;
                OUT_EN = 1;
                write_frame(data_frame, FRAME_LENGTH);
                dk1 = 0;
                dk2 = 0;
            }
        }
        display();
    }
}

void display()
{
    LED_DATA = num[d4];
    DIGIT1 = 0;
    DIGIT2 = 1;
    DIGIT3 = 1;
    DIGIT4 = 1;
    DIGITK1 = 1;
    DIGITK2 = 1;
    delay_us(15);

    LED_DATA = num[d3];
    DIGIT1 = 1;
    DIGIT2 = 0;
    DIGIT3 = 1;
    DIGIT4 = 1;
    DIGITK1 = 1;
    DIGITK2 = 1;
    delay_us(15);

    LED_DATA = num[d2];
    DIGIT1 = 1;
    DIGIT2 = 1;
    DIGIT3 = 0;
    DIGIT4 = 1;
    DIGITK1 = 1;
    DIGITK2 = 1;
    delay_us(15);

    LED_DATA = num[d1];
    DIGIT1 = 1;
    DIGIT2 = 1;
    DIGIT3 = 1;
    DIGIT4 = 0;
    DIGITK1 = 1;
    DIGITK2 = 1;
    delay_us(15);

    LED_DATA = num[dk1];
    DIGIT1 = 1;
    DIGIT2 = 1;
    DIGIT3 = 1;
    DIGIT4 = 1;
    DIGITK1 = 0;
    DIGITK2 = 1;
    delay_us(15);

    LED_DATA = num[dk2];
    DIGIT1 = 1;
    DIGIT2 = 1;
    DIGIT3 = 1;
    DIGIT4 = 1;
    DIGITK1 = 1;
    DIGITK2 = 0;
    delay_us(15);
}

void EEPROM_putc(unsigned char address, unsigned char data)
{
    unsigned char INTCON_SAVE;

    EEADR = address;
    EEDATA = data;

    EECON1bits.EEPGD = 0; // 0 = Access data EEPROM memory

    EECON1bits.WREN = 1; // enable writes to internal EEPROM

    INTCON_SAVE = INTCON; // Save INTCON register contants
    INTCON = 0; // Disable interrupts, Next two lines SHOULD run without interrupts

    EECON2 = 0x55; // Required sequence for write to internal EEPROM
    EECON2 = 0xaa; // Required sequence for write to internal EEPROM

    EECON1bits.WR = 1; // begin write to internal EEPROM
    INTCON = INTCON_SAVE; //Now we can safely enable interrupts if previously used

    NOP();

    while (PIR2bits.EEIF == 0)//Wait till write operation complete
    {
        NOP();
    }

    EECON1bits.WREN = 0; // Disable writes to EEPROM on write complete (EEIF flag on set PIR2 )
    PIR2bits.EEIF = 0; //Clear EEPROM write complete flag. (must be cleared in software. So we do it here)

}

// This function reads data from address given in internal EEPROM of PIC

unsigned char EEPROM_getc(unsigned char address)
{
    EEADR = address;
    EECON1bits.EEPGD = 0; // 0 = Access data EEPROM memory

    EECON1bits.RD = 1; // EEPROM Read
    return EEDATA; // return data
}