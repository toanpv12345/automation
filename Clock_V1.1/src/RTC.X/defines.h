/* 
 * File:   defines.h
 * Author: Toanpv
 *
 * Created on May 13, 2014, 9:59 PM
 */

#include "../../../common/ds18b20.h"
#include "../../../common/ds1307.h"
#include "../../../common/pic16f88x/timer.h"
#include "../../../common/pic16f88x/interrupt.h"
#include "../../../common/dht11.h"

#define DATA_PORT   PORTB
#define DATA_DIR    TRISB

#define ANODE_PORT  PORTA
#define ANODE_TRIS  TRISA

#define ANODE_PIN_DIR1  TRISA0
#define ANODE_PIN_DIR2  TRISA1
#define ANODE_PIN_DIR3  TRISA2
#define ANODE_PIN_DIR4  TRISA3
#define ANODE_PIN_DIR5  TRISA4
#define ANODE_PIN_DIR6  TRISA5

#define ANODE_PIN1      RA0
#define ANODE_PIN2      RA1
#define ANODE_PIN3      RA2
#define ANODE_PIN4      RA3
#define ANODE_PIN5      RA4
#define ANODE_PIN6      RA5

#define BUTTON_MENU	RD0
#define BUTTON_UP	RD1
#define BUTTON_DOWN	RD2
#define BUTTON_MODE_OK	RD3

#define ROLE1		RE1
#define ROLE2		RE0

#define NUM_OF_DIGIT    6

#define DELAY_LED()           delay_ms(3)
#define DELAY_LED_SETUP()     delay_ms(3)
#define DELAY_LED_TEM_HUMI()  delay_ms(3)

#define TIME_DISPLAY_TIME   100
#define TIME_DISPLAY_DATE   50
#define TIME_DISPLAY_TEMP   30

#define TIME_DISPLAY_TIME2   1000
#define TIME_DISPLAY_DATE2   500
#define TIME_DISPLAY_TEMP2   300

#define DISPLAY_TIME_MODE           0
#define DISPLAY_DATE_MODE           1
#define DISPLAY_TEMP_HUMI_MODE    2
#define DISPLAY_HUMI_MODE           3
#define DISPLAY_UNKNOWN_MODE        4

#define TIMES_LOOP               10
#define TIMES_LOOP_KEYBOARD      30

#define CHU_C   10
#define DO_C    11
#define CHU_H   12
#define CHU_P   13
#define DAU_AM  14
#define PERCEN1 15

//#define VERSION_6_DIGIT

#define DHT11       0
#define DS18B20     1

#define SENSOR      DHT11

#define DISPLAY_TEMP    0
#define DISPLAY_HUMI    1

enum {
    NUM1_H,
    NUM1_L,
    NUM2_H,
    NUM2_L,
    NUM3_H,
    NUM3_L
};

enum SetupType {
    SETUP_HOUR,
    SETUP_MINUTE,
    SETUP_DATE,
    SETUP_MONTH,
    SETUP_YEAR,
    SETUP_FINISH
};

enum CountModeType
{
    COUNT_MODE_TIME,
    COUNT_MODE_DATE,
    COUNT_MODE_TEMPERATURE,
    COUNT_MODE_FINISH
};