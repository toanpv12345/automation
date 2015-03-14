#include "types.h"
#include "uart.h"
#include "delays.h"
#include <xc.h>

//__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & MCLRE_ON & CP_OFF & CPD_OFF & BOREN_OFF & IESO_OFF & FCMEN_OFF & LVP_OFF);
//__CONFIG(BOR4V_BOR40V & WRT_OFF);

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & MCLRE_ON & CP_ON & CPD_ON & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF);
__CONFIG(BOR4V_BOR21V & WRT_OFF);

#define FRAME_LENGTH    9

#define LED_DATA    PORTB
#define DIGIT1      PORTCbits.RC3
#define DIGIT2      PORTCbits.RC2
#define DIGIT3      PORTCbits.RC1
#define DIGIT4      PORTCbits.RC0

#define OUT_EN      PORTCbits.RC5

#define DISPLAY_CMD  '0'
#define CONFIRM_CMD  '1'
#define SETID_CMD    '2'
#define CHANGEID_CMD '3'
#define GETID_CMD    '4'

#define HEADER_P    0
#define ID1_P       1
#define ID2_P       2
#define CMD_P       3
#define DATA1_P     4
#define DATA2_P     5
#define DATA3_P     6
#define DATA4_P     7
#define END_P       8

void display();

void EEPROM_putc(unsigned char address, unsigned char data);
unsigned char EEPROM_getc(unsigned char address);

uint8_t num[10] = {0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6};

uint8_t d1 = 0;
uint8_t d2 = 0;
uint8_t d3 = 0;
uint8_t d4 = 0;

void main()
{
    uint8_t data_frame[FRAME_LENGTH] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t id[2] = {'0', '0'};

    uint8_t cmd = 0;
    uint8_t count = 0;
    uint8_t isComplete = false;
    uint8_t first = 0;

    TRISB = 0;
    ANSELH = 0x00;
    ANSEL = 0x00;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC5 = 0;
    PIE1bits.RCIE = 1;
    EEDATA = 0;

    LED_DATA = 0;
    DIGIT1 = 1;
    DIGIT2 = 1;
    DIGIT3 = 1;
    DIGIT4 = 1;
    open_uart(9600);
    delay_ms(20);

    while (1)
    {
        OUT_EN = 0;
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
            if((data_frame[HEADER_P] == 0x7E) && (data_frame[END_P] == 0x7F))
            {
                cmd = data_frame[CMD_P];
                if(cmd == DISPLAY_CMD)
                {
                    if(first == 0)
                    {
                        id[0] = EEPROM_getc(0x00);
                        id[1] = EEPROM_getc(0x01);
                        first = 1;
                    }
                    if((id[0] == data_frame[ID1_P]) && (id[1] == data_frame[ID2_P]))
                    {
                        d1 = data_frame[DATA1_P] - 48;
                        d2 = data_frame[DATA2_P] - 48;
                        d3 = data_frame[DATA3_P] - 48;
                        d4 = data_frame[DATA4_P] - 48;
                    }
                }//if set ID
                else if(cmd == SETID_CMD)
                {
                    if((data_frame[DATA1_P] == '0') && (data_frame[DATA2_P] == '0') &&
                            (data_frame[DATA3_P] == '0') && (data_frame[DATA4_P] == '0'))
                    {
                        EEPROM_putc(0x00, data_frame[ID1_P]);
                        EEPROM_putc(0x01, data_frame[ID2_P]);

                        OUT_EN = 1;
                        write_frame(data_frame, FRAME_LENGTH);
                        first = 0;
                    }// change ID
                    else
                    {
                        id[0] = EEPROM_getc(0x00);
                        id[1] = EEPROM_getc(0x01);

                        if((id[0] == data_frame[ID1_P]) && (id[1] == data_frame[ID2_P]))
                        {
                            EEPROM_putc(0x00, data_frame[DATA3_P]);
                            EEPROM_putc(0x01, data_frame[DATA4_P]);

                            id[0] = data_frame[DATA3_P];
                            id[1] = data_frame[DATA4_P];

                            data_frame[HEADER_P] = 0x7E;
                            data_frame[ID1_P] = id[0];
                            data_frame[ID2_P] = id[1];
                            data_frame[CMD_P] = cmd;
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
                }                
            }
            isComplete = false;
        }
        display();
    }
}

void displayDigit(int order)
{
    switch(order)
    {
        case 1:
            DIGIT1 = 0;
            DIGIT2 = 1;
            DIGIT3 = 1;
            DIGIT4 = 1;
            break;
        case 2:
            DIGIT1 = 1;
            DIGIT2 = 0;
            DIGIT3 = 1;
            DIGIT4 = 1;
            break;

        case 3:
            DIGIT1 = 1;
            DIGIT2 = 1;
            DIGIT3 = 0;
            DIGIT4 = 1;
            break;

        case 4:
            DIGIT1 = 1;
            DIGIT2 = 1;
            DIGIT3 = 1;
            DIGIT4 = 0;
            break;
    }
}

void display()
{
    LED_DATA = num[d1];
    DIGIT1 = 0;
    DIGIT2 = 1;
    DIGIT3 = 1;
    DIGIT4 = 1;

    delay_us(25);
    LED_DATA = num[d2];
    DIGIT1 = 1;
    DIGIT2 = 0;
    DIGIT3 = 1;
    DIGIT4 = 1;

    delay_us(25);
    LED_DATA = num[d3];
    DIGIT1 = 1;
    DIGIT2 = 1;
    DIGIT3 = 0;
    DIGIT4 = 1;

    delay_us(25);
    LED_DATA = num[d4];
    DIGIT1 = 1;
    DIGIT2 = 1;
    DIGIT3 = 1;
    DIGIT4 = 0;

    delay_us(25);
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