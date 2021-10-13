#include "interrupt_manager.h"
#include "tmr0.h"
#include "usart.h"

// Vetor de interrupção
void __interrupt() InterruptManager (void)
{
    if(INTCONbits.T0IE == 1 && INTCONbits.T0IF == 1)
        TMR0_ISR();
        
    if(PIE1bits.RCIE == 1 && PIR1bits.RCIF == 1)
        USART_RX_ISR();
}

void Interrupt_Enable(void)
{
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}

void Interrupt_Disable(void)
{
    INTCONbits.GIE = 0;
    INTCONbits.PEIE = 0;
}
