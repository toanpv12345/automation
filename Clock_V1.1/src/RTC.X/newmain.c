/*
 * File:   rtc_main.c
 * Author: KEN
 *
 * Created on 28 March 2017, 18:56
 */


#include "defines.h"


// PIC16F886 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//uint8_t digit[10] = {0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6}; // ULN2803 buffer, a-->RB7
uint8_t digit[16] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x39, 0x63, 0x76, 0x73, 0x40, 0x5c}; // ULN2803 buffer, a-->RB0
//uint8_t characters[] = {0x39, 0x63, 0x76, 0x73, 0x40, 0x5c}; // C, oC, H, P, -,%
uint8_t buffer[6] = {0};
uint16_t timeout_count = 0;
uint8_t count_mode = 0;
uint8_t mode_display = 0;
uint8_t setup_type = 0;

void led7seg_init();
void led7seg_clear();
void led7seg_display();
void led7seg_display_for_setup(int setup_type);
void led7seg_display_for_setup_4digit(int setup_type);
void led7seg_display_temp(uint8_t sign, uint8_t temperature);
void led7seg_display_temp_humi();
void port_init();
void setup_time();
void setup_time_4digit();

void display_time();
void display_date();
void display_temperature();
void get_temperature_humi();
uint8_t get_day(uint8_t date, uint8_t month, uint8_t year);
void get_date();
void get_time();

void main(void)
{
	int i;
	count_mode = 0;
	timeout_count = 0;
	port_init();
	led7seg_init();
	i2c_open(100000);
	delay_ms(50);
	ds1307_enable_out();
	mode_display = DISPLAY_TIME_MODE;

	while (1)
	{
		if (!BUTTON_MODE_OK)
		{
			if (mode_display == DISPLAY_TIME_MODE) // displaying time
			{
				get_time();
				for(i = 0; i < TIMES_LOOP_KEYBOARD; i++)
				{
					led7seg_display();
				}
			}
			else if (mode_display == DISPLAY_DATE_MODE) // displaying date
			{
				get_date();
				for(i = 0; i < TIMES_LOOP_KEYBOARD; i++)
				{
					led7seg_display();
				}
			}
#ifdef VERSION_6_DIGIT
			else if (mode_display == DISPLAY_TEMP_HUMI_MODE) // displaying temperature and humi
			{
				get_temperature_humi();
				for(i = 0; i < TIMES_LOOP_KEYBOARD; i++)
				{
					led7seg_display_temp_humi();
				}
			}
#endif
			count_mode++;
#ifndef VERSION_6_DIGIT
			if(count_mode >= COUNT_MODE_DATE)
				count_mode = COUNT_MODE_FINISH;
#endif
		}

		switch (count_mode)
		{
			case COUNT_MODE_TIME:
			{
				mode_display = DISPLAY_TIME_MODE;
				display_time();
			}
			break;
#ifdef VERSION_6_DIGIT
			case COUNT_MODE_DATE:
			{
				mode_display = DISPLAY_DATE_MODE;
				display_date();
			}
			break;

			case COUNT_MODE_TEMPERATURE:
			{
				mode_display = DISPLAY_TEMP_HUMI_MODE;
				get_temperature_humi();
				for(i = 0; i < TIMES_LOOP; i++)
				{
					led7seg_display_temp_humi();
				}
			}
			break;
#endif
			case COUNT_MODE_FINISH:
				count_mode = COUNT_MODE_TIME;
			break;
		}
		if (!BUTTON_MENU)
		{
			timeout_count = 0;
			if (mode_display == DISPLAY_TIME_MODE) // displaying time
			{
				get_time();
				for(i = 0; i < TIMES_LOOP_KEYBOARD; i++)
				{
					led7seg_display();
				}
			}
			else if (mode_display == DISPLAY_DATE_MODE) // displaying date
			{
				get_date();
				for(i = 0; i < TIMES_LOOP_KEYBOARD; i++)
				{
					led7seg_display();
				}
			}
#ifdef VERSION_6_DIGIT
			else if (mode_display == DISPLAY_TEMP_HUMI_MODE) // displaying temperature and humi
			{
				get_temperature_humi();
				for(i = 0; i < TIMES_LOOP_KEYBOARD; i++)
				{
					led7seg_display_temp_humi();
				}
			}
			setup_time();
#else
			setup_time_4digit();
#endif
		}
	}
}

void port_init()
{
	TRISC0 = 1;
	TRISC1 = 1;
	TRISC2 = 1;
	TRISC5 = 1;

	ANSEL = 0x00;
	ANSELH = 0x00;
	TRISA = 0x00;
	TRISB = 0x00;
	ADON = 0;
	CCP1CONbits.CCP1M = 0x00;
}

void led7seg_init()
{
	DATA_DIR = 0x00;
	ANODE_PIN_DIR1 = 0;
	ANODE_PIN_DIR2 = 0;
	ANODE_PIN_DIR3 = 0;
	ANODE_PIN_DIR4 = 0;
	ANODE_PIN_DIR5 = 0;
	ANODE_PIN_DIR6 = 0;
	led7seg_clear();
}

void led7seg_clear()
{
	DATA_PORT = 0;
	ANODE_PORT = 0x00;
}

void led7seg_display()
{
	led7seg_clear();
	DATA_PORT = digit[buffer[0]];
	ANODE_PORT = 0x01;
	DELAY_LED();

	led7seg_clear();
	DATA_PORT = digit[buffer[1]];
	ANODE_PORT = 0x02;
	DELAY_LED();

	led7seg_clear();
	DATA_PORT = digit[buffer[2]];
	ANODE_PORT = 0x04;
	DELAY_LED();

	led7seg_clear();
	DATA_PORT = digit[buffer[3]];
	ANODE_PORT = 0x08;
	DELAY_LED();

	led7seg_clear();
	DATA_PORT = digit[buffer[4]];
	ANODE_PORT = 0x10;
	DELAY_LED();

	led7seg_clear();
	DATA_PORT = digit[buffer[5]];
	ANODE_PORT = 0x20;
	DELAY_LED();
	led7seg_clear();
}

//void led7seg_display_temp_humi()
//{
//	led7seg_clear();
//	DATA_PORT = digit[buffer[0]];
//	ANODE_PORT = 0x01;
//	DELAY_LED();
//
//	led7seg_clear();
//	DATA_PORT = digit[buffer[1]];
//	ANODE_PORT = 0x02;
//	DELAY_LED();
//
//	led7seg_clear();
//	DATA_PORT = digit[buffer[2]];
//	ANODE_PORT = 0x04;
//	DELAY_LED();
//
//	led7seg_clear();
//	DATA_PORT = digit[buffer[3]];
//	ANODE_PORT = 0x08;
//	DELAY_LED();
//
//	led7seg_clear();
//	DATA_PORT = digit[buffer[4]];
//	ANODE_PORT = 0x10;
//	DELAY_LED();
//
//	led7seg_clear();
//	DATA_PORT = digit[buffer[5]];
//	ANODE_PORT = 0x20;
//	DELAY_LED();
//	led7seg_clear();
//}
//
//void led7seg_display_for_setup(int setup_type)
//{
//	switch(setup_type)
//	{
//		case SETUP_HOUR:
//		{
//			led7seg_clear();
//			DATA_PORT = digit[buffer[NUM1_H]];
//			ANODE_PORT = 0x01;
//			DELAY_LED_SETUP();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[NUM1_L]];
//			ANODE_PORT = 0x02;
//			DELAY_LED_SETUP();
//			led7seg_clear();
//		}
//		break;
//
//		case SETUP_MINUTE:
//		{
//			led7seg_clear();
//			DATA_PORT = digit[buffer[NUM2_H]];
//			ANODE_PORT = 0x04;
//			DELAY_LED_SETUP();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[NUM2_L]];
//			ANODE_PORT = 0x08;
//			DELAY_LED_SETUP();
//			led7seg_clear();
//		}
//		break;
//
//		case SETUP_DATE:
//		{
//			led7seg_clear();
//			DATA_PORT = digit[buffer[NUM1_H]];
//			ANODE_PORT = 0x01;
//			DELAY_LED_SETUP();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[NUM1_L]];
//			ANODE_PORT = 0x02;
//			DELAY_LED_SETUP();
//			led7seg_clear();
//		}
//		break;
//
//		case SETUP_MONTH:
//		{
//			led7seg_clear();
//			DATA_PORT = digit[buffer[NUM2_H]];
//			ANODE_PORT = 0x04;
//			DELAY_LED_SETUP();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[NUM2_L]];
//			ANODE_PORT = 0x08;
//			DELAY_LED_SETUP();
//			led7seg_clear();
//		}
//		break;
//
//		case SETUP_YEAR:
//		{
//			led7seg_clear();
//			DATA_PORT = digit[2];
//			ANODE_PORT = 0x04;
//			DELAY_LED_SETUP();
//
//			led7seg_clear();
//			DATA_PORT = digit[0];
//			ANODE_PORT = 0x08;
//			DELAY_LED_SETUP();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[NUM3_H]];
//			ANODE_PORT = 0x10;
//			DELAY_LED_SETUP();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[NUM3_L]];
//			ANODE_PORT = 0x20;
//			DELAY_LED_SETUP();
//			led7seg_clear();
//		}
//		break;
//	}
//}

void led7seg_display_for_setup_4digit(int setup_type)
{
	switch(setup_type)
	{
		case SETUP_HOUR:
		{
			led7seg_clear();
			DATA_PORT = digit[buffer[NUM1_H]];
			ANODE_PORT = 0x01;
			DELAY_LED_SETUP();

			led7seg_clear();
			DATA_PORT = digit[buffer[NUM1_L]];
			ANODE_PORT = 0x02;
			DELAY_LED_SETUP();
			led7seg_clear();

			DATA_PORT = digit[CHU_H];
			ANODE_PORT = 0x04;
			DELAY_LED_SETUP();
			led7seg_clear();
		}
		break;

		case SETUP_MINUTE:
		{
			led7seg_clear();
			DATA_PORT = digit[buffer[NUM2_H]];
			ANODE_PORT = 0x01;
			DELAY_LED_SETUP();

			led7seg_clear();
			DATA_PORT = digit[buffer[NUM2_L]];
			ANODE_PORT = 0x02;
			DELAY_LED_SETUP();
			led7seg_clear();

			DATA_PORT = digit[CHU_P];
			ANODE_PORT = 0x04;
			DELAY_LED_SETUP();
			led7seg_clear();
		}
		break;

//		case SETUP_DATE:
//		{
//			led7seg_clear();
//			DATA_PORT = digit[buffer[NUM1_H]];
//			ANODE_PORT = 0x01;
//			DELAY_LED_SETUP();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[NUM1_L]];
//			ANODE_PORT = 0x02;
//			DELAY_LED_SETUP();
//			led7seg_clear();
//		}
//		break;
//
//		case SETUP_MONTH:
//		{
//			led7seg_clear();
//			DATA_PORT = digit[buffer[NUM2_H]];
//			ANODE_PORT = 0x04;
//			DELAY_LED_SETUP();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[NUM2_L]];
//			ANODE_PORT = 0x08;
//			DELAY_LED_SETUP();
//			led7seg_clear();
//		}
//		break;
//
//		case SETUP_YEAR:
//		{
//			led7seg_clear();
//			DATA_PORT = digit[2];
//			ANODE_PORT = 0x04;
//			DELAY_LED_SETUP();
//
//			led7seg_clear();
//			DATA_PORT = digit[0];
//			ANODE_PORT = 0x08;
//			DELAY_LED_SETUP();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[NUM3_H]];
//			ANODE_PORT = 0x10;
//			DELAY_LED_SETUP();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[NUM3_L]];
//			ANODE_PORT = 0x20;
//			DELAY_LED_SETUP();
//			led7seg_clear();
//		}
//		break;
	}
}

//void led7seg_display_temp(uint8_t sign, uint8_t temperature)
//{
//	if (sign == 0) // temp > 0C
//	{
//		if ((temperature >= 10) && (temperature < 100))
//		{
//			led7seg_clear();
//			DATA_PORT = digit[buffer[2]];
//			ANODE_PORT = 0x04;
//			DELAY_LED();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[3]];
//			ANODE_PORT = 0x08;
//			DELAY_LED();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[4]];
//			ANODE_PORT = 0x10;
//			DELAY_LED();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[5]];
//			ANODE_PORT = 0x20;
//			DELAY_LED();
//			led7seg_clear();
//		}
//		else if (temperature < 10)
//		{
//			led7seg_clear();
//			DATA_PORT = digit[buffer[3]];
//			ANODE_PORT = 0x08;
//			DELAY_LED();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[4]];
//			ANODE_PORT = 0x10;
//			DELAY_LED();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[5]];
//			ANODE_PORT = 0x20;
//			DELAY_LED();
//			led7seg_clear();
//		}
//		else
//		{
//			led7seg_clear();
//			DATA_PORT = digit[buffer[1]];
//			ANODE_PORT = 0x02;
//			DELAY_LED();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[2]];
//			ANODE_PORT = 0x04;
//			DELAY_LED();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[3]];
//			ANODE_PORT = 0x08;
//			DELAY_LED();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[4]];
//			ANODE_PORT = 0x10;
//			DELAY_LED();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[5]];
//			ANODE_PORT = 0x20;
//			DELAY_LED();
//			led7seg_clear();
//		}
//	}
//	else
//	{
//		if ((temperature >= 10) && (temperature < 100))
//		{
//			led7seg_clear();
//			DATA_PORT = digit[buffer[1]];
//			ANODE_PORT = 0x02;
//			DELAY_LED();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[2]];
//			ANODE_PORT = 0x04;
//			DELAY_LED();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[3]];
//			ANODE_PORT = 0x08;
//			DELAY_LED();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[4]];
//			ANODE_PORT = 0x10;
//			DELAY_LED();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[5]];
//			ANODE_PORT = 0x20;
//			DELAY_LED();
//			led7seg_clear();
//		}
//		else
//		{
//			led7seg_clear();
//			DATA_PORT = digit[buffer[2]];
//			ANODE_PORT = 0x04;
//			DELAY_LED();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[3]];
//			ANODE_PORT = 0x08;
//			DELAY_LED();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[4]];
//			ANODE_PORT = 0x10;
//			DELAY_LED();
//
//			led7seg_clear();
//			DATA_PORT = digit[buffer[5]];
//			ANODE_PORT = 0x20;
//			DELAY_LED();
//			led7seg_clear();
//		}
//	}
//}
//
//void setup_time()
//{
//	bool is_set_time = false;
//	bool is_set_date = true;
//	uint8_t mode = 0;
//	uint8_t i = 0;
//	int8_t hour = 0, minute = 0;
//	int8_t day = 0, date = 0, month = 0, year = 0;
//	uint8_t hour_set, minute_set, second_set;
//
//	get_time();
//	
//	hour = buffer[NUM1_H] * 10 + buffer[NUM1_L];
//	minute = buffer[NUM2_H] * 10 + buffer[NUM2_L];
//
//	while (1)
//	{
//		led7seg_display_for_setup(mode);
//		if (!BUTTON_MENU)
//		{
//			for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
//			{
//				led7seg_display_for_setup(mode);
//			}
//			mode++;
//		}
//
//		switch (mode)
//		{
//			case SETUP_HOUR: // set hour mode
//			{
//				if(is_set_time)
//				{
//					is_set_time = false;
//					get_time();
//					hour = buffer[NUM1_H] * 10 + buffer[NUM1_L];
//					minute = buffer[NUM2_H] * 10 + buffer[NUM2_L];
//				}
//				if (!BUTTON_UP)
//				{
//					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
//					{
//						led7seg_display_for_setup(mode);
//					}
//					hour++;
//					if (hour > 23)
//						hour = 0;
//				}
//				if (!BUTTON_DOWN)
//				{
//					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
//					{
//						led7seg_display_for_setup(mode);
//					}
//					hour--;
//					if (hour < 0)
//						hour = 23;
//				}
//				buffer[NUM1_H] = hour / 10;
//				buffer[NUM1_L] = hour % 10;
//			}
//			break;
//
//			case SETUP_MINUTE: // set minute mode
//			{
//				if (!BUTTON_UP)
//				{
//					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
//					{
//						led7seg_display_for_setup(mode);
//					}
//					minute++;
//					if (minute > 59)
//						minute = 0;
//				}
//				if (!BUTTON_DOWN)
//				{
//					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
//					{
//						led7seg_display_for_setup(mode);
//					}
//					minute--;
//					if (minute < 0)
//						minute = 59;
//				}
//				buffer[NUM2_H] = minute / 10;
//				buffer[NUM2_L] = minute % 10;
//			}
//			break;
//
//			case SETUP_DATE:
//			{
//				if(is_set_date)
//				{
//					is_set_date = false;
//					get_date();
//					date = buffer[NUM1_H] * 10 + buffer[NUM1_L];
//					month = buffer[NUM2_H] * 10 + buffer[NUM2_L];
//					year = buffer[NUM3_H] * 10 + buffer[NUM3_L];
//				}
//				if (!BUTTON_UP)
//				{
//					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
//					{
//						led7seg_display_for_setup(mode);
//					}
//					date++;
//					if (month == 2)
//					{
//						// nam nhuan
//						if (year / 4 == 0)
//						{
//							if (date > 29)
//								date = 1;
//						}
//						else
//						{
//							if (date > 28)
//								date = 1;
//						}
//					}
//					else if ((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 10) || (month == 12))
//					{
//						if (date > 31)
//							date = 1;
//					}
//					else
//					{
//						if (date > 30)
//							date = 1;
//					}
//				}
//				if (!BUTTON_DOWN)
//				{
//					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
//					{
//						led7seg_display_for_setup(mode);
//					}
//					date--;
//					if (date < 0)
//					{
//						if (month == 2)
//						{
//							if (year / 4 == 0)
//								date = 29;
//							else
//								date = 28;
//						}
//						else if ((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 10) || (month == 12))
//							date = 31;
//						else
//							date = 30;
//					}
//				}
//				buffer[NUM1_H] = date / 10;
//				buffer[NUM1_L] = date % 10;
//			}
//			break;
//			case SETUP_MONTH:
//			{
//				if (!BUTTON_UP)
//				{
//					for (i = 0; i < 30; i++)
//					{
//						led7seg_display_for_setup(mode);
//					}
//					month++;
//					if (month > 12)
//						month = 1;
//				}
//				if (!BUTTON_DOWN)
//				{
//					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
//					{
//						led7seg_display_for_setup(mode);
//					}
//					month--;
//					if (month < 0)
//						month = 12;
//				}
//				buffer[NUM2_H] = month / 10;
//				buffer[NUM2_L] = month % 10;
//			}
//			break;
//			case SETUP_YEAR:
//			{
//				if (!BUTTON_UP)
//				{
//					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
//					{
//						led7seg_display_for_setup(mode);
//					}
//					year++;
//					if (year > 99)
//						year = 14;
//				}
//				if (!BUTTON_DOWN)
//				{
//					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
//					{
//						led7seg_display_for_setup(mode);
//					}
//					year--;
//					if (year < 0)
//						year = 14;
//				}
//				buffer[NUM3_H] = year / 10;
//				buffer[NUM3_L] = year % 10;
//			}
//			break;
//		case SETUP_FINISH:
//			mode = SETUP_HOUR;
//			is_set_time = true;
//			is_set_date = true;
//			mode_display = DISPLAY_TIME_MODE;
//			count_mode = 0;
//			return;
//		}
//
//		if (!BUTTON_MODE_OK)
//		{
//			for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
//			{
//				led7seg_display_for_setup(mode);
//			}
//			
//			day = get_day(date, month, year);
//			hour_set = ((hour/10) << 4) | (hour%10);
//			minute_set = ((minute/10) << 4) | (minute%10);
//			second_set = 0;//((second/10) << 4) | (second%10);
//			ds1307_set_time24h(hour_set, minute_set, second_set);
//			
//			hour_set = ((date/10) << 4) | (date%10);
//			minute_set = ((month/10) << 4) | (month%10);
//			second_set = ((year/10) << 4) | (year%10);
//			ds1307_set_dates(day, hour_set, minute_set, second_set);
//
//			mode_display = DISPLAY_TIME_MODE;
//			count_mode = 0;
//
//			for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
//			{
//				led7seg_display_for_setup(mode);
//			}
//			return;
//		}
//	}
//}

void setup_time_4digit()
{
	bool is_set_time = false;
	bool is_set_date = true;
	uint8_t mode = 0;
	uint8_t i = 0;
	int8_t hour = 0, minute = 0;
	int8_t day = 0, date = 0, month = 0, year = 0;
	uint8_t hour_set, minute_set, second_set;

	get_time();

	hour = buffer[NUM1_H] * 10 + buffer[NUM1_L];
	minute = buffer[NUM2_H] * 10 + buffer[NUM2_L];

	while (1)
	{
		led7seg_display_for_setup_4digit(mode);
		if (!BUTTON_MENU)
		{
			for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
			{
				led7seg_display_for_setup_4digit(mode);
			}
			mode++;
			if(mode >= SETUP_DATE)
				mode = SETUP_FINISH;
		}

		switch (mode)
		{
			case SETUP_HOUR: // set hour mode
			{
				if(is_set_time)
				{
					is_set_time = false;
					get_time();
					hour = buffer[NUM1_H] * 10 + buffer[NUM1_L];
					minute = buffer[NUM2_H] * 10 + buffer[NUM2_L];
				}
				if (!BUTTON_UP)
				{
					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
					{
						led7seg_display_for_setup_4digit(mode);
					}
					hour++;
					if (hour > 23)
						hour = 0;
				}
				if (!BUTTON_DOWN)
				{
					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
					{
						led7seg_display_for_setup_4digit(mode);
					}
					hour--;
					if (hour < 0)
						hour = 23;
				}
				buffer[NUM1_H] = hour / 10;
				buffer[NUM1_L] = hour % 10;
			}
			break;

			case SETUP_MINUTE: // set minute mode
			{
				if (!BUTTON_UP)
				{
					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
					{
						led7seg_display_for_setup_4digit(mode);
					}
					minute++;
					if (minute > 59)
						minute = 0;
				}
				if (!BUTTON_DOWN)
				{
					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
					{
						led7seg_display_for_setup_4digit(mode);
					}
					minute--;
					if (minute < 0)
						minute = 59;
				}
				buffer[NUM2_H] = minute / 10;
				buffer[NUM2_L] = minute % 10;
			}
			break;

			case SETUP_DATE:
			{
				if(is_set_date)
				{
					is_set_date = false;
					get_date();
					date = buffer[NUM1_H] * 10 + buffer[NUM1_L];
					month = buffer[NUM2_H] * 10 + buffer[NUM2_L];
					year = buffer[NUM3_H] * 10 + buffer[NUM3_L];
				}
				if (!BUTTON_UP)
				{
					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
					{
						led7seg_display_for_setup_4digit(mode);
					}
					date++;
					if (month == 2)
					{
						// nam nhuan
						if (year / 4 == 0)
						{
							if (date > 29)
								date = 1;
						}
						else
						{
							if (date > 28)
								date = 1;
						}
					}
					else if ((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 10) || (month == 12))
					{
						if (date > 31)
							date = 1;
					}
					else
					{
						if (date > 30)
							date = 1;
					}
				}
				if (!BUTTON_DOWN)
				{
					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
					{
						led7seg_display_for_setup_4digit(mode);
					}
					date--;
					if (date < 0)
					{
						if (month == 2)
						{
							if (year / 4 == 0)
								date = 29;
							else
								date = 28;
						}
						else if ((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 10) || (month == 12))
							date = 31;
						else
							date = 30;
					}
				}
				buffer[NUM1_H] = date / 10;
				buffer[NUM1_L] = date % 10;
			}
			break;
			case SETUP_MONTH:
			{
				if (!BUTTON_UP)
				{
					for (i = 0; i < 30; i++)
					{
						led7seg_display_for_setup_4digit(mode);
					}
					month++;
					if (month > 12)
						month = 1;
				}
				if (!BUTTON_DOWN)
				{
					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
					{
						led7seg_display_for_setup_4digit(mode);
					}
					month--;
					if (month < 0)
						month = 12;
				}
				buffer[NUM2_H] = month / 10;
				buffer[NUM2_L] = month % 10;
			}
			break;
			case SETUP_YEAR:
			{
				if (!BUTTON_UP)
				{
					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
					{
						led7seg_display_for_setup_4digit(mode);
					}
					year++;
					if (year > 99)
						year = 14;
				}
				if (!BUTTON_DOWN)
				{
					for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
					{
						led7seg_display_for_setup_4digit(mode);
					}
					year--;
					if (year < 0)
						year = 14;
				}
				buffer[NUM3_H] = year / 10;
				buffer[NUM3_L] = year % 10;
			}
			break;
		case SETUP_FINISH:
			mode = SETUP_HOUR;
			is_set_time = true;
			is_set_date = true;
			mode_display = DISPLAY_TIME_MODE;
			count_mode = 0;
			return;
		}

		if (!BUTTON_MODE_OK)
		{
			for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
			{
				led7seg_display_for_setup_4digit(mode);
			}

			//day = get_day(date, month, year);
			hour_set = ((hour/10) << 4) | (hour%10);
			minute_set = ((minute/10) << 4) | (minute%10);
			second_set = 0;//((second/10) << 4) | (second%10);
			ds1307_set_time24h(hour_set, minute_set, second_set);

//			hour_set = ((date/10) << 4) | (date%10);
//			minute_set = ((month/10) << 4) | (month%10);
//			second_set = ((year/10) << 4) | (year%10);
//			ds1307_set_dates(day, hour_set, minute_set, second_set);

			mode_display = DISPLAY_TIME_MODE;
			count_mode = 0;

			for (i = 0; i < TIMES_LOOP_KEYBOARD; i++)
			{
				led7seg_display_for_setup_4digit(mode);
			}
			return;
		}
	}
}

void display_time()
{
	int i = 0;
	get_time();
	for(i = 0; i < TIMES_LOOP; i++)
	{
		led7seg_display();
	}
}

//void display_date()
//{
//	int i = 0;
//	get_date();
//	for(i = 0; i < TIMES_LOOP; i++)
//	{
//		led7seg_display();
//	}
//}

//void display_temperature()
//{
//	uint16_t sensor_data = 0, temperature = 0;
//	uint8_t sign = 0;
//	sensor_data = ds18b20_read_temp();
//	temperature = (sensor_data & 0x00FF) >> 4;
//	temperature |= (sensor_data >> 8) << 4;
//
//	sign = temperature & 0x80;
//	temperature &= 0x7f;
//
//	buffer[4] = characters[DO_C];
//	buffer[5] = characters[CHU_C];
//	if (sign == 0) // temperature >= 0C
//	{
//		buffer[0] = 0;
//		buffer[1] = temperature / 100;
//		buffer[2] = (temperature / 10) % 10;
//		buffer[3] = temperature % 10;
//	}
//	else
//	{
//		if ((temperature >= 10) && (temperature < 100))
//		{
//			buffer[0] = 0;
//			buffer[1] = characters[DAU_AM];
//			buffer[2] = temperature / 10;
//			buffer[3] = temperature % 10;
//		}
//		else
//		{
//			buffer[0] = 0;
//			buffer[1] = 0;
//			buffer[2] = characters[DAU_AM];
//			buffer[3] = temperature;
//		}
//	}
//	led7seg_display_temp(sign, temperature);
//}

//void get_temperature_humi()
//{
//	uint16_t humi = 0, temp = 0;
//	int ret = 0;
//	uint8_t i;
//	ret = dht_get_tem_humi(&temp, &humi);
//	if(ret == DHT_OK)
//	{
//		buffer[0] = (temp / 10);
//		buffer[1] = temp % 10;
//		buffer[2] = DO_C;
//		buffer[3] = CHU_C;
//		buffer[4] = (humi / 10);
//		buffer[5] = humi % 10;
//	}
//}
//
//uint8_t get_day(uint8_t date, uint8_t month, uint8_t year)
//{
//	if (month < 3)
//	{
//		month = month + 12;
//		year = year - 1;
//	}
//	return (date + 2*month + 3*(month + 1)/5 + year + year/4)%7;
//}

void get_date()
{
	uint8_t day = 0;
	uint8_t date = 0;
	uint8_t month = 0;
	uint8_t year = 0;
	
	ds1307_get_dates(&day, &date, &month, &year);

	buffer[NUM1_L] = (date & 0x0F);
	buffer[NUM1_H] = (date >> 4);
	buffer[NUM2_L] = (month & 0x0F);
	buffer[NUM2_H] = (month >> 4);
	buffer[NUM3_L] = (year & 0x0F);
	buffer[NUM3_H] = (year >> 4);
}

void get_time()
{
	uint8_t hours = 0;
	uint8_t min = 0;
	uint8_t second = 0;
	uint8_t mode_hour = 0, pm_nAm = 0;

	ds1307_get_times(&hours, &mode_hour, &pm_nAm, &min, &second);

	if (mode_hour == MODE_12H)
	{
		if (pm_nAm == PM)
		{
			hours += 0x12;
		}
	}
	buffer[NUM1_L] = (hours & 0x0F);
	buffer[NUM1_H] = (hours >> 4);
	buffer[NUM2_L] = (min & 0x0F);
	buffer[NUM2_H] = (min >> 4);
	buffer[NUM3_L] = (second & 0x0F);
	buffer[NUM3_H] = (second >> 4);
}
