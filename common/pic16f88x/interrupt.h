/* 
 * File:   interrupt.h
 * Author: Ken
 *
 * Created on Ngày 14 tháng 12 năm 2013, 08:35
 */

#ifndef INTERRUPT_H
#define	INTERRUPT_H

#include "board.h"

#ifdef	__cplusplus
extern "C" {
#endif

    void isr_global_enable(bool is_enable);
    void isr_external_enable(bool is_enable);
    void isr_portb_change_enable(bool is_enable);
    void isr_pinb_change_enable(bool is_enable, int8_t pin);
    void isr_pinb_change_enable_all(bool is_enable);
    
#ifdef	__cplusplus
}
#endif

#endif	/* INTERRUPT_H */

