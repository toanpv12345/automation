#ifndef TIMER_H
#define TIMER_H
#include "board.h"

void timer_init(void);
void timer_on(void);
void timer_reset(void);
bool timer_is_timeout(void);

#endif // TIMER_H
