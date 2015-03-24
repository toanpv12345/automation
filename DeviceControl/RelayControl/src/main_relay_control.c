/* 
 * File:   main.c
 * Author: Toanpv
 *
 * Created on January 30, 2015, 10:19 PM
 */

#include "../../../common/pic16f88x/board.h"
#include "../../../common/pic16f88x/uart.h"

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)



#define EN          RC2
#define EN_TRIS     TRISC2

#define TEST1       RC0
#define TEST1_TRIS  TRISC0

#define TEST2       RC1
#define TEST2_TRIS  TRISC1

#define RELAY1      RB0
#define RELAY2      RB1
#define RELAY3      RB2
#define RELAY4      RB3
#define RELAY5      RB4
#define RELAY6      RB5
#define RELAY7      RB6
#define RELAY8      RB7

#define RELAY_DATA  PORTB
#define RELAY_TRIS  TRISB

#define START       0
#define DEVICE_ID   1
#define CMD         2
#define DATA        3
#define CRC         4
#define END         5

#define BUFFER_LENGTH   6

// Error code
#define SYSTEM_ERROR    99
#define ID_ERROR        98
#define CMD_ERROR       97
#define DATA_ERROR      96
#define CRC_ERROR       95

// Cmd code
#define CMD_RELAY_ON        0
#define CMD_RELAY_OFF       1
#define CMD_GET_ID          2
#define CMD_SET_ID          3
#define CMD_ALL_RELAY_ON    4
#define CMD_ALL_RELAY_OFF   5

// ID address
#define ID_ADDR     0x03

void EEPROM_write(uint8_t address, uint8_t data);
uint8_t EEPROM_read(uint8_t address);

void main(int argc, char** argv)
{
    uint8_t count = 0;
    uint8_t buffer[BUFFER_LENGTH] = {0};
    uint8_t crc = 0;

    ANSEL = 0x00;
    ANSELH = 0x00;
    ADON = 0;

    EN_TRIS = 0;
    RELAY_TRIS = 0;
    RELAY_DATA = 0;

    uart_open(9600);
    
    while(1)
    {
        EN = 0; // cho phep RS485 nhan du lieu
        if(RX_READY())
        {
            buffer[count] = RX_DATA;
            count++;
            if(buffer[START] != 0xA0)
                count = 0;
        }

        if(count == BUFFER_LENGTH) // nhan xong du lieu
        {
            count = 0;
            EN = 1; // khong cho RS485 nhan du lieu

            if((buffer[START] == 0xA0) && (buffer[END] == 0xB0))
            {
                crc = buffer[START] | buffer[CMD] | buffer[DEVICE_ID] | buffer[DATA];
                if(crc == buffer[CRC])
                {
                    if(buffer[CMD] == CMD_RELAY_ON)
                    {
                        if(buffer[DEVICE_ID] == EEPROM_read(ID_ADDR))
                        {
                            if(buffer[DATA] == 0)
                                RELAY1 = 1;
                            else if(buffer[DATA] == 1)
                                RELAY2 = 1;
                            else if(buffer[DATA] == 2)
                                RELAY3 = 1;
                            else if(buffer[DATA] == 3)
                                RELAY4 = 1;
                            else if(buffer[DATA] == 4)
                                RELAY5 = 1;
                            else if(buffer[DATA] == 5)
                                RELAY6 = 1;
                            else if(buffer[DATA] == 6)
                                RELAY7 = 1;
                            else if(buffer[DATA] == 7)
                                RELAY8 = 1;

                            //uart_write_frame(buffer, BUFFER_LENGTH);
                        }
                    }
                    else if(buffer[CMD] == CMD_RELAY_OFF)
                    {
                        if(buffer[DEVICE_ID] == EEPROM_read(ID_ADDR))
                        {
                            if(buffer[DATA] == 0)
                                RELAY1 = 0;
                            else if(buffer[DATA] == 1)
                                RELAY2 = 0;
                            else if(buffer[DATA] == 2)
                                RELAY3 = 0;
                            else if(buffer[DATA] == 3)
                                RELAY4 = 0;
                            else if(buffer[DATA] == 4)
                                RELAY5 = 0;
                            else if(buffer[DATA] == 5)
                                RELAY6 = 0;
                            else if(buffer[DATA] == 6)
                                RELAY7 = 0;
                            else if(buffer[DATA] == 7)
                                RELAY8 = 0;

                            //uart_write_frame(buffer, BUFFER_LENGTH);
                        }
                    }
                    else if(buffer[CMD] == CMD_ALL_RELAY_OFF)
                    {
                        if(buffer[DEVICE_ID] == EEPROM_read(ID_ADDR))
                        {
                            RELAY_DATA = 0x00;
                            //uart_write_frame(buffer, BUFFER_LENGTH);
                        }
                    }
                    else if(buffer[CMD] == CMD_ALL_RELAY_ON)
                    {
                        if(buffer[DEVICE_ID] == EEPROM_read(ID_ADDR))
                        {
                            RELAY_DATA = 0xFF;

                            //uart_write_frame(buffer, BUFFER_LENGTH);
                        }
                    }
                    else if(buffer[CMD] == CMD_GET_ID)
                    {
                        buffer[DEVICE_ID] = EEPROM_read(ID_ADDR);
                        buffer[CRC] = buffer[START] | buffer[CMD] | buffer[DEVICE_ID] | buffer[DATA];

                        uart_write_frame(buffer, BUFFER_LENGTH);
                    }
                    else if(buffer[CMD] == CMD_SET_ID)
                    {
                        EEPROM_write(ID_ADDR, buffer[DATA]);
                        buffer[DEVICE_ID] = buffer[DATA];
                        buffer[CRC] = buffer[START] | buffer[CMD] | buffer[DEVICE_ID] | buffer[DATA];

                        uart_write_frame(buffer, BUFFER_LENGTH);
                    }
                }
            }
        }
    }
}

void EEPROM_write(uint8_t address, uint8_t data)
{
    uint8_t INTCON_SAVE = 0;

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

uint8_t EEPROM_read(uint8_t address)
{
    EEADR = address;
    EECON1bits.EEPGD = 0; // 0 = Access data EEPROM memory

    EECON1bits.RD = 1; // EEPROM Read
    return EEDATA; // return data
}