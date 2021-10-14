#include "tmr0.h"
#include "main.h"

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
    timerpwm++;
    timer++;
    
    TMR0 = 0x64;
    INTCONbits.T0IF = 0;
}

    /*
    dutyCycle++;
    if(dutyCycle > 9)
        dutyCycle = 0;
    if(pwm_atual == 0)
        LAMPADA = 0;
    else
    {
        if(dutyCycle > pwm_atual)
            LAMPADA = 0;
        else
            LAMPADA = 1;   
    }
    */