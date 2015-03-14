/* 
 * File:   timer.h
 * Author: Ken
 *
 * Created on Ngày 29 tháng 9 năm 2013, 00:06
 */

#ifndef TIMER_H
#define	TIMER_H

#include "board.h"

#define T1_DIV_1    0x00
#define T1_DIV_2    0x01
#define T1_DIV_4    0x02
#define T1_DIV_8    0x03

#ifdef	__cplusplus
extern "C" {
#endif

    void timer1_init(uint8_t prescale, bool is_isr_enable);
    void timer1_start(uint16_t cycle);
    void timer1_start_ms(uint8_t ms);
    bool timer1_is_timeout();
    void timer1_stop();
    void timer1_reset_irs_flag();
    uint16_t timer1_get();

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

