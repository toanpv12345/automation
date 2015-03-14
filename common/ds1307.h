/* 
 * File:   DS1307.h
 * Author: Milinda(milindapro.blogspot.com)
 *
 * Description: This file contains functions for communicate with the DS1307 RTC IC through
 *              hardware I2C protocol
 *
 * Created on December 11, 2012, 7:18 AM
 */

#ifndef DS1307_H
#define	DS1307_H

#define     MODE_12H    1
#define     MODE_24H    0

#define     PM          1
#define     AM          0

#include "i2c.h"

/**
 * Function will read a byte from the @address location of the memory
 * @param address - address that you need to read
 * @return received data byte from DS1307
 */
unsigned char ds1307_read(unsigned char address);

/**
 * Write to DS1307's eeprom
 * @param address - address to write data (0 - 7)
 * @param data - 8bit data to be written
 */
void ds1307_write(unsigned char address, unsigned char data);

/**
 * get seconds from DS1307's eeprom
 * @return seconds on DS1307
 */
unsigned char ds1307_get_second();

/**
 * get minutes from DS1307's eeprom
 * @return minutes on DS1307
 */
unsigned char ds1307_get_minutes();

/**
 * This function will return the hours in 24 hour format by reading from DS1307
 * @return hours from DS1307 in 24 hours format
 */
unsigned char ds1307_get_hours(unsigned char* mode, unsigned char* pm_nAm);

/**
 * reads the Day value from DS1307 memory
 * @return the number for the specific day (1=SUN, 2=MON,... 7=SAT)
 */
void ds1307_enable_out();

unsigned char ds1307_get_day();
unsigned char ds1307_get_date();
unsigned char ds1307_get_month();
unsigned char ds1307_get_year();
void ds1307_get_times(uint8_t* hour, uint8_t* mode_hour, uint8_t* pm_nAm, uint8_t* minute, uint8_t* second);
void ds1307_get_dates(uint8_t* day, uint8_t* date, uint8_t* month, uint8_t* year);

void ds1307_set_date(unsigned char newDate);
void ds1307_set_month(unsigned char newMonth);
void ds1307_set_year(unsigned char newYear);
/**
 * set seconds to DS1307 with CH bit
 * @param newSec - seconds to be set (0 - 79)
 * @param chBit - Clock enable bit (0 = clock enable, 1 = clock disable)
 */
void ds1307_set_second(unsigned char newSec, unsigned char chBit);

/**
 * Set DS1307 minutes
 * @param newMin - minutes to be set(0 - 127)
 */
void ds1307_set_minutes(unsigned char newMin);

/**
 * Set DS1307 hours in 12 hour mode
 * @param newHour - hours in 12 hour mode (0 - 19)
 * @param pm_nAm - AM/PM bit <b>(1 for PM, 0 for AM)</b>
 */
void ds1307_set_hours12(unsigned char newHour, unsigned char pm_nAm);

/**
 * Set DS1307 hours in 24 hour mode
 * @param newHour - hours in 24 hour mode (0 - 29)
 */
void ds1307_set_hours24(unsigned char newHour);

/**
 * Set day to DS1307 (range 0-7)
 * @param newDay - day to be set (1 = SUN, 2 = MON,...., 7 = SAT)
 */
void ds1307_set_day(unsigned char newDay);
void ds1307_set_time12h(unsigned char h, unsigned char min, unsigned char s, unsigned char pm_nAm);
void ds1307_set_time24h(unsigned char h, unsigned char min, unsigned char s);
void ds1307_set_dates(unsigned char day, unsigned char date, unsigned char month, unsigned char year);

#endif	/* DS1307_H */

