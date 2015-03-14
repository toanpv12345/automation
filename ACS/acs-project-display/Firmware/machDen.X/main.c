/* 
 * File:   main.c
 * Author: Ken
 *
 * Created on September 11, 2012, 8:53 PM
 */

#include "htc.h"
#include "pic16f684.h"

#ifndef PIC684
#define PIC684	1 // 1 PIC16F684, 0: PIC16f630
#endif

// PIC16F630 Configuration Bit Settings
#if PIC684 == 0
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & BOREN_ON & CP_ON & CPD_ON);
#else
// PIC16F684 Configuration Bit Settings
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_ON & CPD_ON & BOREN_OFF & IESO_OFF & FCMEN_OFF);
#endif
/*
 * 
 */

#ifndef _XTAL_FREQ
#define _XTAL_FREQ  8000000
#endif

#define INPUT_RED   PORTCbits.RC0
#define ROLE_BLUE   PORTCbits.RC1
#define ROLE_RED    PORTCbits.RC2
#define ALARM       PORTCbits.RC3
#define INPUT_BLUE  PORTCbits.RC4

#define RED_INPUT   1
#define BLUE_INPUT  2

#define RESET_ACTIVE	1
#define BLUE_ACTIVE		2

void main(void)
{
	unsigned char is_red_input = 0;
	unsigned char is_blue_input = 0;

	TRISCbits.TRISC0 = 1;
	TRISCbits.TRISC1 = 0;
	TRISCbits.TRISC2 = 0;
	TRISCbits.TRISC3 = 0;
	TRISCbits.TRISC4 = 1;

#if PIC684 == 1
	ANSEL = 0x00;
	ANSELbits.ANS7 = 0;
	ANSELbits.ANS5 = 0;
	ANSELbits.ANS5 = 0;
	ANSELbits.ANS4 = 0;
	ANSELbits.ANS3 = 0;
	ANSELbits.ANS2 = 0;
	ANSELbits.ANS1 = 0;
	ANSELbits.ANS0 = 0;
	ADCON0bits.ADON = 0;
	ADCON0bits.GO = 0;

	CMCON0bits.CIS = 0;
	CMCON0bits.CM0 = 1;
	CMCON0bits.CM1 = 1;
	CMCON0bits.CM2 = 1;
#endif
	
	ALARM = 0;
	ROLE_RED = 0;
	ROLE_BLUE = 0;
	
	while (1)
	{
		if (!INPUT_RED)
		{
			is_red_input = 1;
			ROLE_RED = 1;
		}
		else
		{
			__delay_ms(10);
			is_red_input = 0;
			ROLE_RED = 0;
		}
		if (is_red_input)
		{
			ALARM = 1;
			__delay_ms(50);
			ALARM = 0;
			__delay_ms(50);
		}
		
		if (!INPUT_BLUE)
		{
			__delay_ms(100);
			ROLE_BLUE = 1;
			is_blue_input++;
			if(is_blue_input == 255)
				is_blue_input = 255;
		}
		else
		{
			__delay_ms(100);
			is_blue_input = 0;
			ROLE_BLUE = 0;
		}
		if(is_blue_input == 1)
		{
			ALARM = 1;
			__delay_ms(100);
			ALARM = 0;
			__delay_ms(100);
			ALARM = 1;
			__delay_ms(100);
			ALARM = 0;
			__delay_ms(200);
		}
	}
}
