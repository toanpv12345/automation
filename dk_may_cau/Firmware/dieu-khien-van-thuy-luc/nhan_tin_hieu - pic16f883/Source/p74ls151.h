#ifndef P74LS151_H
#define P74LS151_H
#include "pin_config.h"

void chip_select(uint8_t cs);
void input_select(uint8_t input_no);
uint8_t scan_input(uint8_t cs, uint8_t input_no);

#endif
