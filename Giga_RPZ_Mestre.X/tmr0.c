#include "tmr0.h"
#include "main.h"
#include "pin_manager.h"

void TMR0_Initialize(void)
{
    //Estouro Timer0: 1ms
    OPTION_REG = 0b00000100;
    TMR0 = 0x64;
    
    INTCONbits.T0IE = 1;
}

void TMR0_ISR(void)
{
    timerbotao1++;
    if(timerbotao1 > 1000)
        timerbotao1 = 1000;
    timerbotao2++;
    if(timerbotao2 > 1000)
        timerbotao2 = 1000;
    timerfimcurso++;
    timerchaveNF++;
    timerchaveNA++;
    timerchavecopo++;
    timer++;
    timerpwm++;
    timeoutRx++;
       
    TMR0 = 0x64;
    INTCONbits.T0IF = 0;
}