#ifndef P74LS573_H
#define P74LS573_H
#include "pin_config.h"

void chip_select(uint8_t cs);
void output(uint8_t out_no, uint8_t status);
void control_relay(uint8_t relay_no, uint8_t status);
void reset_74ls573(void);
void all_relay(uint8_t status);

#endif
