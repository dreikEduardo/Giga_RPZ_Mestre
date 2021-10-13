#include "pwm.h"

void PWM_Initialize(void)
{
	CCP1CON = 0b00001111;
    
    //Frequência PWM: 19.53kHz
    PR2 = 0xFF;             
    T2CON = 0b00000100;
    
    PWM_LoadDutyValue(0);
}

void PWM_LoadDutyValue(unsigned int dutyValue)
{
    CCPR1L = ((dutyValue & 0x03FC)>>2);
    CCP1CON = ((unsigned char)(CCP1CON & 0xCF) | ((dutyValue & 0x0003)<<4));
}
