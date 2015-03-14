/* 
 * File:   main.c
 * Author: TN
 *
 * Created on February 2, 2012, 12:54 PM
 */
#include "uart.h"
#include "p74ls151.h"
#include "delay.h"
#include <xc.h>

//#pragma config WDTEN = OFF, STVREN = OFF, XINST = OFF
//#pragma config IESO = OFF, FOSC = HS
//#pragma config LVP = OFF

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & IESO_OFF & FCMEN_OFF & LVP_OFF);
__CONFIG(BOR4V_BOR21V & WRT_OFF);

#define CS0 0
#define CS1 1
#define CS2 2
#define CS3 3
#define CS4 4

#define MAX_LEN 40
uint8_t scan_buffer[MAX_LEN];
uint8_t last_buffer[MAX_LEN];

void init(void);
void send_input_status(uint8_t cmd, uint8_t status);
void reset_all_relay(void);

void main()
{
    uint8_t i, j, k = 0, input_status;
//    uint8_t scan_idx = 0;
    uint8_t data;
    init();
    //reset_all_relay();
    while(1)
    {
//        for(i = 0; i < 5; i++)
//        {
//            for(j = 0; j < 8; j++)
//            {
//                input_status = scan_input(i, j);
//                send_input_status(j + k, input_status);
//            }
//            k += 8;
//        }
//        k = 0;

        // Scan 40 inputs
        for(i = 0; i < 5; i++)
        {
            for(j = 0; j < 8; j++)
            {
                input_status = scan_input(i, j);
                data = ((j + k) << 1) | input_status;
                scan_buffer[j + k] = data;
            }
            k += 8;
        }
        k = 0;

        // Compare input status currently with input status befor
        for(i = 0; i < MAX_LEN; i++)
        {
            if(scan_buffer[i] != last_buffer[i])
            {
                uart_write_char(scan_buffer[i]);
                delay_us(50);
            }
        }

        // Save input status curently
        for ( i = 0; i < MAX_LEN; i++)
        {
            last_buffer[i] = scan_buffer[i];
        }
        //delay_ms(50);
    }
}

void init(void)
{
    io_config();
    uart_open(9600);
    STBP1 = 1;
    STBP2 = 1;
    STBP3 = 1;
    STBP4 = 1;
    STBP5 = 1;
}

void send_input_status(uint8_t input_no, uint8_t status)
{
    uint8_t cmd;
    cmd = (input_no << 1) | (status & 0x01);
    uart_write_char(cmd);
}

void reset_all_relay()
{
    uint8_t i;
    for(i = 0; i < 40; i++)
    {
        send_input_status(i, 0);
        delay_ms(5);
    }
}
