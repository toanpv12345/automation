#include "DS1307.h"

/**
 * 
 * File:   DS1307.h
 * Author: Milinda(milindapro.blogspot.com)
 *
 * Description: This file contains functions for communicate with the DS1307 RTC IC through
 *              hardware I2C protocol 
 *
 * Created on December 11, 2012, 7:52 AM
 */

/**
 * Function will read a byte from the @address location of the memory
 * @param address - address that you need to read
 * @return received data byte from DS1307
 */
unsigned char ds1307_read(unsigned char address)
{
    unsigned char data;

    i2c_start(); //send start signal on I2C bus
    i2c_send(208); //send DS1307 dummy write command to get the pointer to our location
    i2c_send(address); //set the address pointer
    i2c_restart(); //send stop+start signal to bus
    i2c_send(209); //send read commad to DS1307
    data = i2c_read(); //read the buffer for received data
    i2c_stop(); //close the i2c bus
    return data;
}

/**
 * Write to DS1307's eeprom
 * @param address - address to write data (0 - 7)
 * @param data - 8bit data to be written
 */
void ds1307_write(unsigned char address, unsigned char data)
{
    i2c_start(); //start the i2c bus
    i2c_send(208); //send DS1307 write command, DS1307 address + direction bit(R/nW) 1101000 + 0 = 11010000 = 208
    i2c_send(address); //send the address that you need to write data on.
    i2c_send(data); //write data on the address
    i2c_stop(); //close the I2C bus
}

/**
 * get seconds from DS1307's eeprom
 * @return seconds on DS1307(0x00 - 0x59)
 */
unsigned char ds1307_get_second()
{
    unsigned char bcdSec = ds1307_read(0); //get the 0th location data(CH + 10Sec + 1Sec)[in BCD format]
    bcdSec = bcdSec & 0b01111111; //ignore CH bit (ignore MSB)
    return bcdSec;
}

/**
 * get minutes from DS1307's eeprom
 * @return minutes on DS1307 (0x00 - 0x59)
 */
unsigned char ds1307_get_minutes()
{
    return ds1307_read(1); //get the 1st loaction's data(0 + 10Min + 1Min)[in BCD format]
}

/**
 * This function will return the hours in 24 hour format by reading from DS1307
 * @return hours from DS1307 in 24 hours format(0x00 - 0x23)
 */
unsigned char ds1307_get_hours(unsigned char* mode, unsigned char* pm_nAm)
{
    unsigned char hour;
    hour = ds1307_read(2); //get the 1st loaction's data(0 + 12 or 24 hour mode + 10hour(first bit) or PM/AM + 10hour(second bit) + 1hour)[in BCD format]

    if((hour & 0x40) == 0x40) // mode = 12h
    {
        *mode = MODE_12H;
        if((hour & 0x20) == 0x20)
            *pm_nAm = PM;
        else
            *pm_nAm = AM;

        hour &= 0x1F;
    }
    else // mode 24h
    {
        *mode = MODE_24H;
        hour &= 0x3F;
    }

    return hour;
}

/**
 * reads the Day value from DS1307 memory
 * @return the number for the specific day (1=SUN, 2=MON,... 7=SAT)
 */
unsigned char ds1307_get_day()
{
    return ds1307_read(3); //read value on 3rd location of DS1307 and return it
}

/**
 * reads the Date value from DS1307 memory
 * @return the number for the specific date (0x01 - 0x31)
 */
unsigned char ds1307_get_date()
{
    return ds1307_read(4); //read value on 3rd location of DS1307 and return it
}

/**
 * reads the Month value from DS1307 memory
 * @return the number for the specific month (0x01 - 0x12)
 */
unsigned char ds1307_get_month()
{
    return ds1307_read(5); //read value on 3rd location of DS1307 and return it
}

/**
 * reads the Year value from DS1307 memory
 * @return the number for the specific year (0x00 - 0x99)
 */
unsigned char ds1307_get_year()
{
    return ds1307_read(6); //read value on 3rd location of DS1307 and return it
}

void ds1307_get_times(uint8_t* hour, uint8_t* mode_hour, uint8_t* pm_nAm, uint8_t* minute, uint8_t* second)
{
    *hour = ds1307_get_hours(mode_hour, pm_nAm);
    *minute = ds1307_get_minutes();
    *second = ds1307_get_second();
}

void ds1307_get_dates(uint8_t* day, uint8_t* date, uint8_t* month, uint8_t* year)
{
    *day = ds1307_get_day();
    *date = ds1307_get_date();
    *month = ds1307_get_month();
    *year = ds1307_get_year();
}

/**
 * set seconds to DS1307 with CH bit
 * @param newSec - seconds to be set (0 - 0x59)
 * @param chBit - Clock enable bit (0 = clock enable, 1 = clock disable)
 */
void ds1307_set_second(unsigned char newSec, unsigned char chBit)
{
    unsigned char bcdNum;
    if (newSec > 0x59)
        return; //to avoid writing to CH when writing to second feild
    //add CH bit
    if (chBit == 1)
        bcdNum = newSec | 0b10000000;
    else
        bcdNum = newSec & 0x7F;

    ds1307_write(0, bcdNum); //write to sec
}

/**
 * Set DS1307 minutes
 * @param newMin - minutes to be set(0x00 - 0x59)
 */
void ds1307_set_minutes(unsigned char newMin)
{
    //unsigned char bcdNum;
    if (newMin > 0x59)
        return; //to avoid overlimit

    ds1307_write(1, newMin); //write to min
}

/**
 * Set DS1307 hours in 12 hour mode
 * @param newHour - hours in 12 hour mode (0x01 - 0x12)
 * @param pm_nAm - AM/PM bit <b>(1 for PM, 0 for AM)</b>
 */
void ds1307_set_hours12(unsigned char newHour, unsigned char pm_nAm)
{
    unsigned char bcdNum;
    if (newHour > 0x12)
        return; //avoid overlimit

    //bcdNum = 0b01000000;//set 12 hour mode
    if (pm_nAm)//PM?
        bcdNum = newHour | 0b01100000; //set PM
    else
        bcdNum = newHour | 0b01000000; //set AM

    ds1307_write(2, bcdNum); //write to hours
}

/**
 * Set DS1307 hours in 24 hour mode
 * @param newHour - hours in 24 hour mode (0 - 0x23)
 */
void ds1307_set_hours24(unsigned char newHour)
{
    // unsigned char bcdNum;
    if (newHour > 0x23)
        return; //avoid overlimit

    ds1307_write(2, newHour); //write to hours
}

/**
 * Set day to DS1307 (range 0-7)
 * @param newDay - day to be set (1 = SUN, 2 = MON,...., 7 = SAT)
 */
void ds1307_set_day(unsigned char newDay)
{
    if (newDay > 7)
        return; //avoid overlimit

    ds1307_write(3, newDay); //write to day
}

/**
 * Set date to DS1307 (range 0-0x31)
 * @param newDay - day to be set (0-0x31)
 */
void ds1307_set_date(unsigned char newDate)
{
    if (newDate > 0x31)
        return; //avoid overlimit

    ds1307_write(4, newDate); //write to day
}

/**
 * Set month to DS1307 (range 1-0x12)
 * @param newMonth - month to be set (1-0x12)
 */
void ds1307_set_month(unsigned char newMonth)
{
    if (newMonth > 0x12)
        return; //avoid overlimit

    ds1307_write(5, newMonth); //write to day
}

/**
 * Set year to DS1307 (range 0-0x99)
 * @param newYear - year to be set (00-0x99)
 */
void ds1307_set_year(unsigned char newYear)
{
    if (newYear > 0x99)
        return; //avoid overlimit

    ds1307_write(6, newYear); //write to day
}

void ds1307_set_time12h(unsigned char h, unsigned char min, unsigned char s, unsigned char pm_nAm)
{
    ds1307_set_hours12(h, pm_nAm);
    ds1307_set_minutes(min);
    ds1307_set_second(s, 0);
}

void ds1307_set_time24h(unsigned char h, unsigned char min, unsigned char s)
{
    ds1307_set_hours24(h);
    ds1307_set_minutes(min);
    ds1307_set_second(s, 0);
}

void ds1307_set_dates(unsigned char day, unsigned char date, unsigned char month, unsigned char year)
{
    ds1307_set_day(day);
    ds1307_set_date(date);
    ds1307_set_month(month);
    ds1307_set_year(year);
}

void ds1307_enable_out()
{
	uint8_t second = ds1307_get_second();
	second &= 0x7F;
	delay_ms(10);
	ds1307_set_second(second, 0);
	ds1307_write(0x07, 0x10);
}