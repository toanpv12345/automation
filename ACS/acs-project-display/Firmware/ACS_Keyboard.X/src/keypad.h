#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "board.h"

#define KEYPAD_TRIS TRISD
#define ROW   PORTD
#define COL   (PORTD & 0x70)

void keypad_init(void);
uint8_t keypad_get();

#endif
