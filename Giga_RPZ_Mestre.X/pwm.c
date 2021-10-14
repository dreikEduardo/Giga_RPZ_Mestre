#include "pwm.h"

void PWM_Initialize(void)
{
	CCP1CON = 0b00001111;
    
    //Frequência PWM: 19.53kHz
    PR2 = 0xFF;             
    T2CON = 0b00000100;
    
    PWM_LoadDutyValue(0);
}

void PWM_LoadDutyValue(unsigned char dutyValue)
{
    CCPR1L = dutyValue;
}
