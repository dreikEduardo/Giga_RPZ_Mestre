#include "pin_manager.h"

void PinManager_Initialize(void)
{
    PORTA = 0b00000000; 
    PORTB = 0b00000000;
    PORTC = 0b00000000;
    
    TRISA = 0b11010000;
    TRISB = 0b11111111;
    TRISC = 0b10011000;  

    ADCON1 = 0b00000110;

    OPTION_REG = 0b00000100;
}



  
