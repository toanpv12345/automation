/* 
 * File:   led7seg.h
 * Author: Ken
 *
 * Created on June 1, 2013, 10:08 PM
 */

#ifndef LED7SEG_H
#define	LED7SEG_H


#ifdef	__cplusplus
extern "C" {
#endif

    void led7seg_init();
    void led7seg_display(uint8_t* data);
    void led7seg_clear();

#ifdef	__cplusplus
}
#endif

#endif	/* LED7SEG_H */

