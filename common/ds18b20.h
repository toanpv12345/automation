/* 
 * File:   ds1820.h
 * Author: Ken
 *
 * Created on May 27, 2013, 10:50 PM
 */

#ifndef DS1820_H
#define	DS1820_H

#include "board.h"

#define	DQ		RC7
#define DQ_read()	TRISC7=1
#define DQ_write()	TRISC7=0

// ROM COMMANDS FOR DS18B20
#define SEARCH_ROM      0xF0
#define READ_ROM        0x33
#define MATCH_ROM       0x55
#define SKIP_ROM        0xCC
#define ALARM_SEARCH    0xEC

// FUNCTION COMMANDS FOR DS18B20
#define CONVERT_T           0x44
#define WRITE_SCRATCHPAD    0x4E
#define READ_SCRATCHPAD     0xBE
#define COPY_SCRATCHPAD     0x48
#define RECALL_E            0xB8
#define READ_POWER_SUPPLY   0xB4

// Resolution
#define RESOLUTION_9BITS     0x1F
#define RESOLUTION_10BITS    0x3F
#define RESOLUTION_11BITS    0x5F
#define RESOLUTION_12BITS    0x7F

#ifdef	__cplusplus
extern "C" {
#endif

    void ds18b20_reset(void);
    uint8_t ds18b20_read(void);
    void ds18b20_write(uint8_t dat);
    uint16_t ds18b20_read_temp(void);
    void ds18b20_init(uint8_t resolution);

#ifdef	__cplusplus
}
#endif

#endif	/* DS1820_H */

