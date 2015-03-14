#include "uart.h"

// PIC16F886 Configuration Bit Settings
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & IESO_OFF & FCMEN_OFF & LVP_OFF);
__CONFIG(BOR4V_BOR40V & WRT_OFF);

#define BLED_PIN      RC4
#define RYLED_PIN     RC3
#define BUZZER_PIN    RC2
#define RELAY_PIN     RC1
#define OUT_EN        RC5

// Position for bytes
#define START_P         0
#define LENGTH_P        1
#define CMD_P           2
#define ITEM_P          3
#define STATUS_P        4
#define SHORT_TIME_P    5
#define LONG_TIME_P     6
#define CRC_P           7
#define END_P           8

// Value for bytes
#define START_V     0xA0
#define LENGTH_V    0x09
#define CMD_V       0x0B
#define END_V       0xB0

// Control Items
#define BUZZER_CTR      0x00
#define BLED_CTR        0x01
#define RYLED_CTR       0x02
#define RELAY_CTR       0x03

// Control Status
#define NC              0
#define NO              1
#define DESIGN_TIME     0x02

#define FRAME_LENGTH    9

#define RS485_RE()      OUT_EN = 0
#define RS485_TE()      OUT_EN = 1

#define OPEN  1
#define CLOSE 0

uint32_t getTime(uint8_t longTime, uint8_t shortTime);
void timerInit();
void onTimer();
bit isTimeout();
void resetTimer();
bool isMessageValid(uint8_t *message);
uint8_t getCRC(uint8_t *frame, uint8_t len);

bool timerOn1 = false;
bool timerOn2 = false;
bool timerOn3 = false;
bool timerOn4 = false;

void main()
{

    uint8_t data_frame[FRAME_LENGTH] = {0};
    uint8_t items = 0;
    uint8_t status = 0;
    uint8_t count = 0;
    uint8_t crc = 0;
    uint32_t time = 0;
    uint32_t timeOut1 = 0;
    uint32_t timeOut2 = 0;
    uint32_t timeOut3 = 0;
    uint32_t timeOut4 = 0;


    TRISB = 0;
    ADCON1 = 0x07;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;
    PIE1bits.RCIE = 0;
    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 0;

    uartOpen(9600);
    timerInit();
    __delay_ms(100);

    while(1)
    {
        RS485_RE();
        if(RX_READY())
        {
            data_frame[count] = RCREG;
            count++;
            onTimer();
        }
        if(isTimeout())
        {
            uartDisable(true);
            resetTimer();
            count = 0;

            if(isMessageValid(data_frame))
            {
                items = data_frame[ITEM_P];
                status = data_frame[STATUS_P];

                if(items == BUZZER_CTR)
                {
                    if(status != DESIGN_TIME)
                    {
                        BUZZER_PIN = status;
                    }
                    else
                    {
                        time = getTime(data_frame[LONG_TIME_P], data_frame[SHORT_TIME_P]);
                        timerOn1 = true;
                        timeOut1 = 0;
                        BUZZER_PIN = OPEN;
                    }
                }
                else if(items == BLED_CTR)
                {
                    if(status != DESIGN_TIME)
                    {
                        BLED_PIN = status;
                    }
                    else
                    {
                        time = getTime(data_frame[LONG_TIME_P], data_frame[SHORT_TIME_P]);
                        timerOn2 = true;
                        timeOut2 = 0;
                        BLED_PIN = OPEN;
                    }
                }
                else if(items == RYLED_CTR)
                {
                    if(status != DESIGN_TIME)
                    {
                        RYLED_PIN = status;
                    }
                    else
                    {
                        time = getTime(data_frame[LONG_TIME_P], data_frame[SHORT_TIME_P]);
                        timerOn3 = true;
                        timeOut3 = 0;
                        RYLED_PIN = OPEN;
                    }
                }
                else if(items == RELAY_CTR)
                {
                    if(status != DESIGN_TIME)
                    {
                        RELAY_PIN = status;
                    }
                    else
                    {
                        time = getTime(data_frame[LONG_TIME_P], data_frame[SHORT_TIME_P]);
                        timerOn4 = true;
                        timeOut4 = 0;
                        RELAY_PIN = OPEN;
                    }
                }
            }
            crc = getCRC(data_frame, 5);
            data_frame[LENGTH_P] = 0x07;
            data_frame[5] = crc;
            data_frame[6] = END_V;
            
            uartDisable(false);
            RS485_TE();
            uartWriteFrame(data_frame, 7);
        }

        if(timerOn1 || timerOn2 || timerOn3 || timerOn4)
        {
            if(timerOn1)
            {
                timeOut1++;                
                if(timeOut1 == time)
                {
                    BUZZER_PIN = CLOSE;
                    timeOut1 = 0;
                    timerOn1 = false;
                }
            }

            if(timerOn2)
            {
                timeOut2++;               
                if(timeOut2 == time)
                {
                    BLED_PIN = CLOSE;
                    timeOut2 = 0;
                    timerOn2 = false;
                }
            }
            if(timerOn3)
            {
                timeOut3++;               
                if(timeOut3 == time)
                {
                    RYLED_PIN = CLOSE;
                    timeOut3 = 0;
                    timerOn3 = false;
                }
            }
            if(timerOn4)
            {
                timeOut4++;               
                if(timeOut4 == time)
                {
                    RELAY_PIN = CLOSE;
                    timeOut4 = 0;
                    timerOn4 = false;
                }
            }
            __delay_ms(1);
        }        
    }
}

uint32_t getTime(uint8_t longTime, uint8_t shortTime)
{
    uint16_t time = ((0xffff & longTime) << 8) | shortTime;
    return (time * 20);
}

void timerInit()
{
    T1CON = 0;
    T1CKPS0 = 1;
    T1CKPS1 = 1; // Prescale = 1:8
    TMR1CS = 0; // Fosc/4
    TMR1GE = 0;
    TMR1ON = 0;
}

void onTimer()
{
    TMR1ON = 0;
    TMR1H = ((0xFFFF - 1250) >> 8) & 0xFF; // timeout for 5ms
    TMR1L = (0xFFFF - 1250) & 0xFF;
    TMR1IF = 0;
    TMR1ON = 1;
}

bit isTimeout()
{
    return TMR1IF;
}

bool isMessageValid(uint8_t* message)
{
    uint8_t crc = 0;

    if(message[CMD_P] != CMD_V)
        return false;

    if(message[LENGTH_P] != LENGTH_V)
        return false;

    if(message[START_P] != START_V)
        return false;

    if(message[END_P] != END_V)
        return false;

    crc = getCRC(message, 7);
    if(crc != message[CRC_P])
        return false;

    return true;
}

void resetTimer()
{
    TMR1IF = 0;
}

uint8_t getCRC(uint8_t *frame, uint8_t len)
{
    uint8_t tmp = 0;
    while(len)
    {
        len--;
        tmp |= frame[len];        
    }
    return tmp;
}