#include "pwm.h"

#define TMR2_SCALER		16

void pwm_set_period(uint32_t period)
{
	PR2 = (uint8_t)((uint32_t)(_XTAL_FREQ >> 2)/(TMR2_SCALER*period) - 1);
}

void pwm1_set_duty(uint8_t duty)
{
	float ratio = (float)duty/100;
	uint16_t val = (uint16_t)(ratio*4*(PR2 + 1));

	CCPR1L = (val >> 2) & 0xFF;
	CCP1CONbits.DC1B0 = val & 0x01;
	CCP1CONbits.DC1B1 = val & 0x02;
}

void pwm2_set_duty(uint8_t duty)
{
	float ratio = (float)duty/100;
	uint16_t val = (uint16_t)(ratio*4*(PR2 + 1));

	CCPR2L = (val >> 2) & 0xFF;
	CCP2CONbits.DC2B0 = val & 0x01;
	CCP2CONbits.DC2B1 = val & 0x02;
}

void pwm1_set_mode(uint8_t mode)
{
	switch(mode)
	{
	case SingleMode:
		CCP1CONbits.CCP1M0 = 0;
		CCP1CONbits.CCP1M1 = 0;
		CCP1CONbits.CCP1M2 = 1;
		CCP1CONbits.CCP1M3 = 1;
		CCP1CONbits.P1M0 = 0;
		CCP1CONbits.P1M1 = 0;
		break;
	}
}

void pwm2_set_mode()
{
	CCP2CONbits.CCP2M0 = 1;
	CCP2CONbits.CCP2M1 = 1;
	CCP2CONbits.CCP2M2 = 1;
	CCP2CONbits.CCP2M3 = 1;
}

void pwm1_set_out_pin(bool bVal)
{
	if(bVal)
		TRISC2 = 0;
	else
		TRISC2 = 1;
}

void pwm2_set_out_pin(bool bVal)
{
	if(bVal)
		TRISC1 = 0;
	else
		TRISC1 = 1;
}

void pwm_init_timer2()
{
	TMR2ON = 0;
	T2CONbits.TOUTPS = 0x00;
	TMR2IF = 0;
	T2CKPS0 = 1;
	T2CKPS1 = 1;
	TMR2ON = 1;
}

void pwm1_start(uint8_t duty)
{
	pwm1_set_duty(duty);
	pwm1_set_out_pin(true);
}

void pwm2_start(uint8_t duty)
{
	pwm2_set_duty(duty);
	pwm2_set_out_pin(true);
}

void pwm1_init(uint32_t period_hz, uint8_t mode)
{
	pwm1_set_out_pin(false);
	pwm_set_period(period_hz);
	pwm1_set_mode(mode);
	pwm1_set_duty(0);
}

void pwm2_init(uint32_t period_hz)
{
	pwm2_set_out_pin(false);
	pwm_set_period(period_hz);
	pwm2_set_mode();
	pwm2_set_duty(0);
}

void pwm1_stop()
{
	CCP1CONbits.CCP1M = 0x00;
}

void pwm2_stop()
{
	CCP2CONbits.CCP2M = 0x00;
}