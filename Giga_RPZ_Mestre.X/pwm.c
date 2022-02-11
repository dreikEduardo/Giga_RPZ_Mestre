#include "pwm.h"

void PWM_Initialize(void)
{
	CCP1CON = 0b00001111;
    
    //Frequência PWM: 1.22kHz
    PR2 = 0xFF;             
    T2CON = 0b00000111;
    
    PWM_LoadDutyValue(0);
}

void PWM_LoadDutyValue(unsigned int dutyValue)
{
    CCPR1L = (unsigned char)(dutyValue >> 2);
    CCP1CON = (unsigned char)(CCP1CON | (dutyValue << 4));
}