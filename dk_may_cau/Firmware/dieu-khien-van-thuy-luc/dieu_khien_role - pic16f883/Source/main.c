/* 
 * File:   main.c
 * Author: TN
 *
 * Created on February 2, 2012, 12:54 PM
 */
#include "uart.h"
#include "74ls573.h"
#include <xc.h>
#include "delay.h"

//#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
//#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
//#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)
//
//// CONFIG2H
//#pragma config WDTE = OFF      // Watchdog Timer Enable bit (WDT is controlled by SWDTEN bit of the WDTCON register)
////#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)
//#pragma config LVP = OFF

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & IESO_OFF & FCMEN_OFF & LVP_OFF);
__CONFIG(BOR4V_BOR21V & WRT_OFF);

void init(void);
void main()
{
    uint8_t cmd, relay_no, status;

    init();
   
    while(1)
    {
        if(RX_READY())
        {
            cmd = RX_DATA();
            relay_no = cmd >> 1;
            status = cmd & 0x01;
            control_relay(relay_no, status);
        }
    }
}

void init()
{
    reset_74ls573();
    io_config();
    all_relay(0);
    uart_open(9600);
}
