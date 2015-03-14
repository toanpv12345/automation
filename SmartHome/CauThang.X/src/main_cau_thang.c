/* 
 * File:   main.c
 * Author: Toanpv
 *
 * Created on January 30, 2015, 10:19 PM
 */

#include "../../../common/pic16f88x/board.h"
#include "../../../common/pic16f88x/timer.h"

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



#define POWER       RB1
#define POWER_TRIS  TRISB1

#define LED2        RB2
#define LED2_TRIS   TRISB2

#define LED1        RB3
#define LED1_TRIS   TRISB3

#define SENSOR1         RA0
#define SENSOR1_TRIS    TRISA0

#define SENSOR2         RA1
#define SENSOR2_TRIS    TRISA1

#define LDR         RA2
#define LDR_TRIS    TRISA2

#define RELAY        RA3
#define RELAY_TRIS   TRISA3

#define TIME_DELAY  100 // 100ms

void main(int argc, char** argv)
{
    bool is_sensor1 = false;
    bool is_sensor2 = false;

    uint8_t count = 0;
    uint8_t time_out = 0;

    ANSEL = 0x00;
    ANSELH = 0x00;
    ADON = 0;

    POWER_TRIS = 1;
    LED1_TRIS = 0;
    LED2_TRIS = 0;
    SENSOR1_TRIS = 1;
    SENSOR2_TRIS = 1;
    LDR_TRIS = 1;
    RELAY_TRIS = 0;

    RELAY = 0; // tat den

    timer1_init(T1_DIV_8, false);
    
    while(1)
    {
        if(LDR == 0) // Ban ngay
        {
            RELAY = 0; // tat den
        }
        else
        {
            // co cam bien 1
            if(SENSOR1 == 0)
            {
                delay_ms(300);
                is_sensor1 = true;
                count++;
            }

            // co cam bien 2
            if(SENSOR2 == 0)
            {
                delay_ms(300);
                is_sensor2 = true;
                count++;
            }

            // co cam bien 1 va chua co cam bien 2 (dang di len)
            if((is_sensor1 == true) && (is_sensor2 == false))
            {
                RELAY = 1; // Bat den
            }
            
            // dang di len, nhung giua chung di xuong
            if((is_sensor1 == true) && (is_sensor2 == false) && (count >= 2))
            {
                count = 0;
                is_sensor1 = false;
                is_sensor2 = false;
                timer1_start_ms(TIME_DELAY);
            }

            // co cam bien 1 va co cam bien 2 (da di het cau thang)
            if((is_sensor1 == true) && (is_sensor2 == true))
            {
                count = 0;
                is_sensor1 = false;
                is_sensor2 = false;
                timer1_start_ms(TIME_DELAY);
            }

            // co cam bien 2 va chua co cam bien 1 (dang di xuong)
            if((is_sensor1 == false) && (is_sensor2 == true))
            {
                 RELAY = 1; // Bat den
            }
            
            // co cam bien 2 va chua co cam bien 1 (dang di xuong), giua chung di len
            if((is_sensor1 == false) && (is_sensor2 == true) && (count >= 2))
            {
                count = 0;
                is_sensor1 = false;
                is_sensor2 = false;
                timer1_start_ms(TIME_DELAY);
            }

            if((is_sensor1 == false) && (is_sensor2 == false) && (timer1_is_timeout() == true))
            {
                time_out++;
                timer1_start_ms(TIME_DELAY);
            }
            
            if((is_sensor1 == false) && (is_sensor2 == false) && (time_out >= 50))
            {
                RELAY = 0; // tat den
                time_out = 0;
                timer1_stop();
            }
        }
    }
}

