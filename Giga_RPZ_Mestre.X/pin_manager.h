#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#include <xc.h>
#include "main.h"

void PinManager_Initialize(void);

// Configuração dos entrada e saída
// Entradas
#define BOTAO_1 PORTBbits.RB0
#define BOTAO_2 PORTBbits.RB1
#define FIM_CURSO PORTBbits.RB2
#define CHAVE_TESTE_NF PORTBbits.RB3
#define CHAVE_TESTE_NA PORTBbits.RB4
#define CHAVE_TESTE_COPO PORTBbits.RB5
#define RX PORTCbits.RC7

// Saídas
#define LED_DEFEITO_TESTE PORTAbits.RA1
#define LED_EM_TESTE PORTAbits.RA2
#define LED_TESTE_OK PORTAbits.RA3
#define LED_TESTE_NF PORTCbits.RC0
#define LED_TESTE_NA PORTCbits.RC1
#define LAMPADA PORTCbits.RC2
#define RELE_ALIMENTACAO_PLACA PORTCbits.RC5
#define RELE_VALVULA PORTAbits.RA5
#define BUZZER PORTAbits.RA0
#define TX PORTCbits.RC6

#endif