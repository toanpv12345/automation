/* 
 * File:   pwm.h
 * Author: Toanpv
 *
 * Created on April 20, 2014, 11:29 AM
 */
#include "board.h"

enum PWMMode
{
    SingleMode,
    FullBridgeForwardMode,
    HalfBridgeMode,
    FullBridgeReversedMode
};

enum PWMChanel
{
    PWMChanel1,
    PWMChanel2
};

#ifndef PWM_H
#define	PWM_H

void pwm_set_period(uint32_t period_hz);
void pwm1_set_duty(uint8_t duty);
void pwm2_set_duty(uint8_t duty);
void pwm1_set_mode(uint8_t mode);
void pwm2_set_mode();
void pwm1_set_out_pin(bool bVal);
void pwm2_set_out_pin(bool bVal);
void pwm_init_timer2();
void pwm1_init(uint32_t period_hz, uint8_t mode);
void pwm2_init(uint32_t period_hz);
void pwm1_start(uint8_t duty);
void pwm2_start(uint8_t duty);
void pwm1_stop();
void pwm2_stop();

#endif	/* PWM_H */

