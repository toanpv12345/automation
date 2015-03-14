#include "types.h"
#include "uart.h"
#include "delays.h"

// PIC16F883 Configuration Bit Settings

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


#define RED         PORTCbits.RC3
#define BLUE        PORTCbits.RC2

#define SENSOR1     PORTAbits.RA2
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

void main()
{
    uint8_t data_frame[FRAME_LENGTH] = {0};
    uint8_t cmd = 0;
    uint8_t count = 0;
    uint8_t isComplete = false;

    ANSEL = 0x00;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA1 = 1;
    PIE1bits.RCIE = 0;
    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 0;

    open_uart(9600);
    timer_init();
    delay_ms(100);
    
    while (1)
    {
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
            if((data_frame[END_P] == 0x7F) && (data_frame[ID_P] == DEVICE_ID))
            {
                cmd = data_frame[CMD_P];

                if(cmd == REQUEST_SENSOR_CMD)
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

                    write_frame(data_frame);
                }
                cmd = 0;
            }
            isComplete = false;
        }
    }
}

