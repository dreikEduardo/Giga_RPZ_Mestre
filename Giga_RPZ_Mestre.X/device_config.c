#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include "device_config.h"

// Configuração de gravação
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = ON        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config CP = ALL         // FLASH Program Memory Code Protection bits (0000h to 1FFFh code protected)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low Voltage In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = ON         // Data EE Memory Code Protection (Data EEPROM memory code-protected)
#pragma config WRT = ON         // FLASH Program Memory Write Enable (Unprotected program memory may be written to by EECON control)

#endif