#include "tmr1.h"

void TMR1_Initialize(void)
{
    T1CON = 0b00110001;
    PIE1bits.TMR1IE = 0;
}