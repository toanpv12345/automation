#ifndef BOARD_H
#define BOARD_H

#if defined(__XC8)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdbool.h>
#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ  12000000
#endif

#define delay_ms(ms)    __delay_ms(ms)
#define delay_us(us)    __delay_us(us)

#endif /* BOARD_H_ */
