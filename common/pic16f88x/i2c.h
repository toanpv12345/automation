/* 
 * File:   I2C.h
 * Author: Milinda(milindapro.blogspot.com)
 *
 * Created on December 11, 2012, 6:59 AM
 */

#ifndef I2C_H
#define	I2C_H

//#include <htc.h>
//#define _XTAL_FREQ 12000000

#include "board.h"

/**
 * Initialize the I2C module as a master mode<br/>
 * SCL - RC3
 * SDA - RC4
 * 100kHz I2C click at 4Mhz crystal
 */
void i2c_open(unsigned long clock_Hz);

/**
 * wait until I2C bus become idel
 */
void i2c_wait();

/**
 * Send start condition to I2C bus
 */
void i2c_start();

/**
 * Send stop condition to I2C bus
 */
void i2c_stop();

/**
 * Send restart condition to I2C bus
 */
void i2c_restart();

/**
 * Send 8 bit(1 byte) through I2C bus
 * @param data - 8 bit data t be sent
 */
void i2c_send(unsigned char data);

/**
 * read the I2C bus
 * @return Read data from I2C slave
 */
unsigned char i2c_read();

/**
 * send acknowledge condition
 */
void i2c_send_ack();

/**
 * send not acknowledge condition
 */
void i2c_send_nack();

#endif	/* I2C_H */

