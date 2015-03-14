#ifndef BUFF_H
#define BUFF_H
#include "types.h"

void buff_init(void);
bool buff_push(uint8_t ch);
bool buff_pop(uint8_t *ch);

#endif
