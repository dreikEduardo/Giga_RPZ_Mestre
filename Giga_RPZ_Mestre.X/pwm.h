#ifndef PWM_H
#define PWM_H

#include <xc.h>

void PWM_Initialize(void);
void PWM_LoadDutyValue(unsigned int dutyValue);

#endif