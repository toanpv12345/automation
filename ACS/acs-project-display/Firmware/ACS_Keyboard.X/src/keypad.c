#include "keypad.h"
#include "delays.h"

#define Col1    PORTDbits.RD4
#define Col2    PORTDbits.RD5
#define Col3    PORTDbits.RD6

#define D1      PORTCbits.RC3
#define D2      PORTCbits.RC2
#define D3      PORTCbits.RC1
#define D4      PORTCbits.RC0
#define DK1     PORTAbits.RA3
#define DK2     PORTAbits.RA4

void keypad_init() {
    TRISD = 0b11110000; // 4 bit cao la cac cot(input), 4 bit thap la cac hang(output)
    PORTD = 0b00001111;
}

uint8_t keypad_get() {

    // Scan row 1
    PORTD = 0b00001110;
    if(!Col1){
        while(!Col1){
            D1 = D2 = D3 = D4 = DK1 = DK2 = 1;
        }
        delay_us(120);
        return 0x38;
    }
    if(!Col2){
        while(!Col2){
            D1 = D2 = D3 = D4 = DK1 = DK2 = 1;
        }
        delay_us(120);
        return 0x34;
    }
    if(!Col3){
        while(!Col3){
            D1 = D2 = D3 = D4 = DK1 = DK2 = 1;
        }
        delay_us(120);
        return 0x30;
    }

    // Scan row 2
    PORTD = 0b00001101;
    if(!Col1){
        while(!Col1){
            D1 = D2 = D3 = D4 = DK1 = DK2 = 1;
        }
        delay_us(120);
        return 0x39;
    }
    if(!Col2){
        while(!Col2){
            D1 = D2 = D3 = D4 = DK1 = DK2 = 1;
        }
        delay_us(120);
        return 0x35;
    }
    if(!Col3){
        while(!Col3){
            D1 = D2 = D3 = D4 = DK1 = DK2 = 1;
        }
        delay_us(120);
        return 0x31;
    }

    // Scan row 3
    PORTD = 0b00001011;
    if(!Col1){
        while(!Col1){
            D1 = D2 = D3 = D4 = DK1 = DK2 = 1;
        }
        delay_us(120);
        return 'E';
    }
    if(!Col2){
        while(!Col2){
            D1 = D2 = D3 = D4 = DK1 = DK2 = 1;
        }
        delay_us(120);
        return 0x36;
    }
    if(!Col3){
        while(!Col3){
            D1 = D2 = D3 = D4 = DK1 = DK2 = 1;
        }
        delay_us(120);
        return 0x32;
    }

    // Scan row 4
    PORTD = 0b00000111;
    if(!Col1){
        while(!Col1){
            D1 = D2 = D3 = D4 = DK1 = DK2 = 1;
        }
        delay_us(120);
        return 'C';
    }
    if(!Col2){
        while(!Col2){
            D1 = D2 = D3 = D4 = DK1 = DK2 = 1;
        }
        delay_us(120);
        return 0x37;
    }
    if(!Col3){
        while(!Col3){
            D1 = D2 = D3 = D4 = DK1 = DK2 = 1;
        }
        delay_us(120);
        return 0x33;
    }

    return 0;
}
