// Nome do projeto: Giga Teste RPZ - Placa Mestre
// Descrição:
// Microcontrolador: PIC16F876A
// Compilador: XC8

// Bibliotecas 
#include <xc.h>
#include "main.h"
#include "device_config.h"
#include "pin_manager.h"
#include "interrupt_manager.h"
#include "tmr0.h"
#include "pwm.h"
#include "usart.h"

// Vetor de início
void main(void)
{ 
    PinManager_Initialize();
    TMR0_Initialize();
    //PWM_Initialize();
    USART_Initialize();
    Interrupt_Enable();
    
    estagio = INTERROMPIDO;
    estado.botao1 = 0;
    estado.botao2 = 0;
    estado.fimcurso = 0;
    estado.chaveNF = 0;
    estado.chaveNA = 0;
    timerbotao1 = 0;
    timerbotao2 = 0;
    timerfimcurso = 0;
    timerchaveNF = 0;
    timerchaveNA = 0;
    timerpwm = 0;
    //pwm_setado = PWM_LAMPADA_DESLIGADA;
             
    while(1)
    {
        CLRWDT();
        
        if(BOTAO_1 == 0)
        {
            if(estado.botao1 == 0)
            {
                if(timerbotao1 > TEMPO_DEBOUNCE)
                    estado.botao1 = 1; 
            }
        }
        else
        {
            timerbotao1 = 0;
            estado.botao1 = 0;
        }
                
        if(BOTAO_2 == 0)
        {
            if(estado.botao2 == 0)
            {
                if(timerbotao2 > TEMPO_DEBOUNCE)
                    estado.botao2 = 1; 
            }
        }
        else
        {
            timerbotao2 = 0;
            estado.botao2 = 0;
        }
                
        if(FIM_CURSO == 0)
        {
            if(estado.fimcurso == 0)
            {
                if(timerfimcurso > TEMPO_DEBOUNCE)
                    estado.fimcurso = 1; 
            }
        }
        else
        {
            timerfimcurso = 0;
            estado.fimcurso = 0;
            if(estado.testando == 1)
            {
                estagio = INTERROMPIDO;
                estado.testando = 0;
                comunicacao.endereco = 0x0F;
                comunicacao.resposta = 0x00;   
                USART_Write(comunicacao.byte);
            }
        }
        
        if(CHAVE_TESTE_NF == 0)
        {
            if(estado.chaveNF == 0)
            {
                if(timerchaveNF > TEMPO_DEBOUNCE)
                {
                    estado.chaveNF = 1;
                    LED_TESTE_NF = 1;
                }
            }
        }
        else
        {
            timerchaveNF = 0;
            estado.chaveNF = 0;
            LED_TESTE_NF = 0;
        }

        if(CHAVE_TESTE_NA == 0)
        {
            if(estado.chaveNA == 0)
            {
                if(timerchaveNA > TEMPO_DEBOUNCE)
                {
                    estado.chaveNA = 1; 
                    LED_TESTE_NA = 1;
                }
            }
        }
        else
        {
            timerchaveNA = 0;
            estado.chaveNA = 0;
            LED_TESTE_NA = 0;
        }
        
        /*
        if(pwm_setado == PWM_LAMPADA_LIGADA)
        {
            PWM_LoadDutyValue(PWM_LAMPADA_LIGADA);
            pwm_atual = PWM_LAMPADA_LIGADA;
        }
        if(pwm_setado == PWM_LAMPADA_DESLIGADA)
        {
            PWM_LoadDutyValue(PWM_LAMPADA_DESLIGADA);
            pwm_atual = PWM_LAMPADA_DESLIGADA;
        }
        if(timerpwm > TEMPO_RAMPA_PWM)
        {
            timerpwm = 0;
            if(pwm_atual != pwm_setado)
            {
                if(pwm_atual < pwm_setado)
                {
                    pwm_atual++;
                    PWM_LoadDutyValue(pwm_atual);
                }
                else
                {
                    pwm_atual--;
                    PWM_LoadDutyValue(pwm_atual);
                }
            }
        }
        */
                
        switch(estagio)
        {
            case INTERROMPIDO:
            {
                if(timer < TEMPO_PISCADA)
                {
                    LED_DEFEITO_TESTE = 1;
                    LED_TESTE_OK = 1; 
                    LED_EM_TESTE = 1;
                }
                else
                {
                    LED_DEFEITO_TESTE = 0;
                    LED_TESTE_OK = 0; 
                    LED_EM_TESTE = 0;
                    if(timer > (TEMPO_PISCADA*2))
                        timer = 0;
                }
                //pwm_setado = PWM_LAMPADA_DESLIGADA;
                LAMPADA = 0;
                RELE_ALIMENTACAO_PLACA = 0;
                RELE_VALVULA = 0;
                if((estado.botao1 == 1) && (estado.botao2 == 1))
                    estagio = TESTE_BOTOES;
            }
            break;
            
            case TESTE_BOTOES:
            {
                if((estado.botao1 == 1) && (estado.botao2 == 1))
                {
                    if((timerbotao1 < 500) && (timerbotao2 < 500))
                        estagio = INICIA_TESTE;
                }       
            }
            break;
            
            case INICIA_TESTE:
            {
                LED_DEFEITO_TESTE = 0;
                LED_TESTE_OK = 0;
                LED_EM_TESTE = 1;
                BUZZER = 0;
                RELE_VALVULA = 1;
                estado.reteste = 0;
                estado.falha = 0;
                endereco = 0;
                timer = 0;
                estagio = TESTE_TAMPA_FECHADA;
            }
            break;
            
            case TESTE_TAMPA_FECHADA:
            {
                if(timer > TEMPO_FECHA_TAMPA)
                    estagio = FALHA;
                else
                {
                    if(estado.fimcurso == 1)
                        estagio = ALIMENTA_PLACA;
                }
            }
            break;
            
            case ALIMENTA_PLACA:
            {
                RELE_ALIMENTACAO_PLACA = 1;
                if(estado.chaveNA == 1)
                    //pwm_setado = PWM_LAMPADA_DESLIGADA;
                    LAMPADA = 0;
                else
                    //pwm_setado = PWM_LAMPADA_LIGADA;
                    LAMPADA = 1;    
                estado.testando = 1;
                comunicacao.endereco = 0x0F;
                if(estado.chaveNF == 1)
                    comunicacao.resposta = 0x09;
                else
                {
                    if(estado.chaveNA == 1)
                        comunicacao.resposta = 0x05;
                    else
                        comunicacao.resposta = 0x03;   
                }
                USART_Write(comunicacao.byte);
                timer = 0;
                estagio = ESTABILIZACAO;
            }
            break;
             
            case ESTABILIZACAO:
            {
                if(timer > TEMPO_ESTABILIZACAO)
                    estagio = TESTE_LIGA;
            }
            break;
            
            case TESTE_LIGA:
            {
                if(estado.chaveNA == 1)
                    //pwm_setado = PWM_LAMPADA_MAX;
                    LAMPADA = 1;
                else
                    //pwm_setado = PWM_LAMPADA_MIN;
                    LAMPADA = 0;  
                timer = 0;
                estagio = AGUARDA_TEMPO_LIGA;
            }
            break;
            
            case AGUARDA_TEMPO_LIGA:
            {
                if(timer > TEMPO_LIGA)
                    estagio = TESTE_DESLIGA;
            }
            break;
            
            case TESTE_DESLIGA:
            {
                if(estado.chaveNA == 1)
                    //pwm_setado = PWM_LAMPADA_MIN;
                    LAMPADA = 0;
                else
                    //pwm_setado = PWM_LAMPADA_MAX;
                    LAMPADA = 1;  
                timer = 0;
                estagio = AGUARDA_TEMPO_DESLIGA;
            }
            break;
            
            case AGUARDA_TEMPO_DESLIGA:
            {
                if(timer > TEMPO_DESLIGA)
                {
                    if(estado.reteste == 1)
                        estagio = VERIFICA_PLACAS;
                    else
                        estagio = RETESTE;
                }
            }
            break;
            
            case RETESTE:
            {
                estado.reteste = 1;
                estagio = TESTE_LIGA;
            }
            break;
            
            case VERIFICA_PLACAS:
            {
                endereco++;
                if(endereco <= 10)
                {
                    timer = 0;
                    USART_RX_Clear_Buffer();
                    comunicacao.endereco = endereco;
                    comunicacao.resposta = 0x0F;
                    USART_Write(comunicacao.byte);
                    estagio = MENSAGEM_RETORNO;  
                }
                else
                {
                    if(estado.falha == 1)
                        estagio = FALHA;
                    else
                        estagio = OK;
                }
            }
            break;            

            case MENSAGEM_RETORNO:
            {
                if(timer > TEMPO_TIMEOUT_RX)
                {
                    estado.falha = 1;
                    estagio = VERIFICA_PLACAS;
                }
                else
                {
                    if(USART_RX_Ready() == 1)
                    {
                        comunicacao.byte = USART_Read();
                        if(comunicacao.endereco == endereco)
                        {
                            if(comunicacao.resposta != 0x08)
                                estado.falha = 1;
                        }
                        else
                            estado.falha = 1;
                        estagio = VERIFICA_PLACAS;
                    }
                }
            }
            break;               
 
            case FALHA:
            {
                //pwm_setado = PWM_LAMPADA_DESLIGADA;
                LAMPADA = 0;
                LED_DEFEITO_TESTE = 1;
                BUZZER = 1;
                timer = 0;
                estagio = TESTE_FINALIZADO;
            }
            break;
            
            case OK:
            {
                //pwm_setado = PWM_LAMPADA_DESLIGADA;
                LAMPADA = 0;
                LED_TESTE_OK = 1;
                estagio = TESTE_FINALIZADO;
            }
            break;
            
            case TESTE_FINALIZADO:
            {
                if(timer > TEMPO_BUZZER)
                {
                    BUZZER = 0;
                    estagio = TESTE_BOTOES;
                }
                LED_EM_TESTE = 0;
                RELE_ALIMENTACAO_PLACA = 0;
                RELE_VALVULA = 0;
                estado.testando = 0;
            }
            break;

            default:
            break;
        }  
    }
}