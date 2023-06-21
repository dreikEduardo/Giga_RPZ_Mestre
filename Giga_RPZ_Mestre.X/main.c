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
    PWM_Initialize();
    TMR0_Initialize();
    USART_Initialize();
    Interrupt_Enable();
    
    PR2 = 0xFF;             
    T2CON = 0b00000111;    
    
    estagio = INTERROMPIDO;
    estado.botao1 = 0;
    estado.botao2 = 0;
    estado.fimcurso = 0;
    estado.chaveNF = 0;
    estado.chaveNA = 0;
    estado.chavecopo = 0;
    timerbotao1 = 0;
    timerbotao2 = 0;
    timerfimcurso = 0;
    timerchaveNF = 0;
    timerchaveNA = 0;
    timerchavecopo = 0;
    timerpwm = 0;
    pwm_setado = PWM_LAMPADA_DESLIGADA;
              
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
        
        if(CHAVE_TESTE_COPO == 0)
        {
            if(estado.chavecopo == 0)
            {
                if(timerchavecopo > TEMPO_DEBOUNCE)
                    estado.chavecopo = 1;
            }
        }
        else
        {
            timerchavecopo = 0;
            estado.chavecopo = 0;
        }
        
        if(pwm_setado == PWM_LAMPADA_MAX)
        {
            if(pwm_atual == PWM_LAMPADA_DESLIGADA)
            {
                LAMPADA = 1;
                pwm_atual++;
            }
            else
            {
                if(pwm_atual == PWM_LAMPADA_MAX)
                {
                    CCP1CON = 0b00001111;
                    PWM_LoadDutyValue(PWM_LAMPADA_MAX);
                }
            }
        }
        
        if(pwm_setado == PWM_LAMPADA_MAX_COPO)
        {
            if(pwm_atual == PWM_LAMPADA_DESLIGADA)
            {
                LAMPADA = 1;
                pwm_atual++;
            }
            else
            {
                if(pwm_atual == PWM_LAMPADA_MAX_COPO)
                {
                    CCP1CON = 0b00001111;
                    PWM_LoadDutyValue(PWM_LAMPADA_MAX_COPO);
                }
            }
        }
        
        if(pwm_setado == PWM_LAMPADA_DESLIGADA)
        {
            CCP1CON = 0b00000000;
            LAMPADA = 0;
            pwm_atual = PWM_LAMPADA_DESLIGADA;
        }
        
        if(timerpwm > TEMPO_RAMPA_PWM)
        {
            timerpwm = 0;
            if(pwm_atual != pwm_setado)
            {
                if(pwm_atual < pwm_setado)
                {
                    if(pwm_atual < 1000)
                        pwm_atual++;
                    PWM_LoadDutyValue(pwm_atual);
                }
                else
                {
                    if(pwm_atual > 0)
                        pwm_atual--;  
                    PWM_LoadDutyValue(pwm_atual);
                }
            }
        }
       
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
                pwm_setado = PWM_LAMPADA_DESLIGADA;
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
                if(estado.falha == 1)
                {
                    if(addressFail > 0)
                    {
                        if(timer < TEMPO_PISCADA)
                            LED_DEFEITO_TESTE = 1;
                        else
                            LED_DEFEITO_TESTE = 0;    
                        if(timer > (TEMPO_PISCADA*2))
                        {
                            addressFail--;
                            timer = 0;                                      
                        }
                    }
                    else
                        LED_DEFEITO_TESTE = 1;    
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
                escravo.byte = 0;
                timer = 0;
                naoliga = 0;
                naodesliga = 0;
                sincronismo = 0;
                pecaOK = 0;
                addressFail = 0;
                estagio = TESTE_TAMPA_FECHADA;
            }
            break;
            
            case TESTE_TAMPA_FECHADA:
            {
                if(timer > TEMPO_FECHA_TAMPA)
                    estagio = FALHA_FIM_CURSO;
                else
                {
                    if(estado.fimcurso == 1)
                    {
                        timer = 0;
                        estagio = ALIMENTA_PLACA;
                    }
                }
            }
            break;
            
            case ALIMENTA_PLACA:
            {
                estado.testando = 1;
                if(estado.chaveNA == 1)
                    pwm_setado = PWM_LAMPADA_DESLIGADA;
                else
                {
                    if(estado.chavecopo == 1)
                        pwm_setado = PWM_LAMPADA_MAX_COPO;
                    else
                        pwm_setado = PWM_LAMPADA_MAX; 
                }
                if(timer > 2000)
                {
                    RELE_ALIMENTACAO_PLACA = 1;
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
            }
            break;
             
            case ESTABILIZACAO:
            {
                if(timer > TEMPO_ESTABILIZACAO)
                    estagio = TESTE_LIGA;
                
                if(USART_RX_Ready() == 0)
                    timeoutRx = 0;
                else
                {
                    if(timeoutRx > TEMPO_TIMEOUT_RX)
                    {
                        USART_RX_Clear_Buffer();
                        timeoutRx = 0;
                    }
                }
        
                if(USART_RX_Ready() == 1)
                {
                    comunicacao.byte = USART_Read();
                    if(comunicacao.endereco == 0x00)
                    {
                        if(comunicacao.resposta == 0x0F)
                        {
                            estagio = FALHA;
                            estado.testando = 0;
                            comunicacao.endereco = 0x0F;
                            comunicacao.resposta = 0x00;   
                            USART_Write(comunicacao.byte);
                        }
                    }
                }
            }
            break;
            
            case TESTE_LIGA:
            {
                if(estado.chaveNA == 1)
                    pwm_setado = PWM_LAMPADA_MAX;
                else
                    pwm_setado = PWM_LAMPADA_MIN;
                timer = 0;
                estagio = AGUARDA_TEMPO_LIGA;
            }
            break;
            
            case AGUARDA_TEMPO_LIGA:
            {
                if((timer > TEMPO_LIGA) || (escravo.byte == 0x03FF))
                {
                    if(timer > TEMPO_LIGADO)
                    {
                        escravo.byte = 0;
                        endereco = 0;
                        estagio = TESTE_DESLIGA;
                    }
                }
                else
                    estagio = VERIFICA_PLACAS_LIGA;
            }
            break;
            
            case VERIFICA_PLACAS_LIGA:
            {
                endereco++;
                if(endereco <= 10)
                {
                    timeoutRx = 0;
                    USART_RX_Clear_Buffer();
                    comunicacao.endereco = endereco;
                    comunicacao.resposta = 0x0F;
                    USART_Write(comunicacao.byte);
                    estagio = MENSAGEM_RETORNO_PLACAS_LIGA;  
                }
                else
                    endereco = 0;
            }
            break;
            
            case MENSAGEM_RETORNO_PLACAS_LIGA:
            {
                if(timeoutRx > TEMPO_TIMEOUT_RX)
                    estagio = AGUARDA_TEMPO_LIGA;
                else
                {
                    if(USART_RX_Ready() == 1)
                    {
                        comunicacao.byte = USART_Read();
                        if(comunicacao.endereco == endereco)
                        {
                            if(comunicacao.resposta != 0x00)
                            {
                                switch(endereco)
                                {
                                    case 1:
                                        escravo.endereco1 = 1;
                                    break; 
                                    case 2:
                                        escravo.endereco2 = 1;
                                    break;
                                    case 3:
                                        escravo.endereco3 = 1;
                                    break; 
                                    case 4:
                                        escravo.endereco4 = 1;
                                    break; 
                                    case 5:
                                        escravo.endereco5 = 1;
                                    break; 
                                    case 6:
                                        escravo.endereco6 = 1;
                                    break;
                                    case 7:
                                        escravo.endereco7 = 1;
                                    break; 
                                    case 8:
                                        escravo.endereco8 = 1;
                                    break; 
                                    case 9:
                                        escravo.endereco9 = 1;
                                    break; 
                                    case 10:
                                        escravo.endereco10 = 1;
                                    break; 
                                }      
                            }
                        }
                        estagio = AGUARDA_TEMPO_LIGA;
                    }
                }
            }
            break;
            
            case TESTE_DESLIGA:
            {
                if(estado.chaveNA == 1)
                    pwm_setado = PWM_LAMPADA_MIN;
                else
                {
                    if(estado.chavecopo == 1)
                        pwm_setado = PWM_LAMPADA_MAX_COPO;
                    else
                        pwm_setado = PWM_LAMPADA_MAX; 
                } 
                timer = 0;
                estagio = AGUARDA_TEMPO_DESLIGA;
                comunicacao.endereco = 0x0F;
                comunicacao.resposta = 0x07;
                USART_Write(comunicacao.byte);
            }
            break;
            
            case AGUARDA_TEMPO_DESLIGA:
            {
                if((timer > TEMPO_DESLIGA) || (escravo.byte == 0x03FF))
                {
                    if((timer > TEMPO_DESLIGADO) || (estado.reteste == 1))
                    {
                        escravo.byte = 0;
                        endereco = 0;
                        if(estado.reteste == 1)
                            estagio = VERIFICA_PLACAS;
                        else
                            estagio = RETESTE;
                    }
                }
                else
                    estagio = VERIFICA_PLACAS_DESLIGA;
            }
            break;
            
            case VERIFICA_PLACAS_DESLIGA:
            {
                endereco++;
                if(endereco <= 10)
                {
                    timeoutRx = 0;
                    USART_RX_Clear_Buffer();
                    comunicacao.endereco = endereco;
                    comunicacao.resposta = 0x0F;
                    USART_Write(comunicacao.byte);
                    estagio = MENSAGEM_RETORNO_PLACAS_DESLIGA;  
                }
                else
                    endereco = 0;
            }
            break;
            
            case MENSAGEM_RETORNO_PLACAS_DESLIGA:
            {
                if(timeoutRx > TEMPO_TIMEOUT_RX)
                    estagio = AGUARDA_TEMPO_DESLIGA;
                else
                {
                    if(USART_RX_Ready() == 1)
                    {
                        comunicacao.byte = USART_Read();
                        if(comunicacao.endereco == endereco)
                        {
                            if(comunicacao.resposta != 0x00)
                            {
                                switch(endereco)
                                {
                                    case 1:
                                        escravo.endereco1 = 1;
                                    break; 
                                    case 2:
                                        escravo.endereco2 = 1;
                                    break;
                                    case 3:
                                        escravo.endereco3 = 1;
                                    break; 
                                    case 4:
                                        escravo.endereco4 = 1;
                                    break; 
                                    case 5:
                                        escravo.endereco5 = 1;
                                    break; 
                                    case 6:
                                        escravo.endereco6 = 1;
                                    break;
                                    case 7:
                                        escravo.endereco7 = 1;
                                    break; 
                                    case 8:
                                        escravo.endereco8 = 1;
                                    break; 
                                    case 9:
                                        escravo.endereco9 = 1;
                                    break; 
                                    case 10:
                                        escravo.endereco10 = 1;
                                    break; 
                                }      
                            }
                        }
                        estagio = AGUARDA_TEMPO_DESLIGA;
                    }
                }
            }
            break;
            
            case RETESTE:
            {
                estado.reteste = 1;
                estagio = TESTE_LIGA;
                comunicacao.endereco = 0x0F;
                comunicacao.resposta = 0x0C;
                USART_Write(comunicacao.byte);
            }
            break;
            
            case VERIFICA_PLACAS:
            {
                endereco++;
                if(endereco <= 10)
                {
                    countFailRx = 0;
                    timeoutRx = 0;
                    USART_RX_Clear_Buffer();
                    comunicacao.endereco = endereco;
                    comunicacao.resposta = 0x0F;
                    USART_Write(comunicacao.byte);
                    estagio = MENSAGEM_RETORNO_PLACAS;  
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

            case MENSAGEM_RETORNO_PLACAS:
            {
                if(timeoutRx > TEMPO_TIMEOUT_RX)
                {
                    if(countFailRx < 3)
                    {
                        countFailRx++;
                        timeoutRx = 0;
                        USART_RX_Clear_Buffer();
                        comunicacao.endereco = endereco;
                        comunicacao.resposta = 0x0F;
                        USART_Write(comunicacao.byte);
                        estagio = MENSAGEM_RETORNO_PLACAS;     
                    }
                    else
                    {
                        estado.falha = 1;
                        addressFail = endereco;
                        estagio = VERIFICA_PLACAS;
                    }
                }
                else
                {
                    if(USART_RX_Ready() == 1)
                    {
                        comunicacao.byte = USART_Read();
                        if(comunicacao.endereco == endereco)
                        {
                            if(comunicacao.resposta == 0x02)
                                naodesliga++;
                            if(comunicacao.resposta == 0x03)
                                naoliga++;
                            if(comunicacao.resposta == 0x04)
                                sincronismo++;
                            if(comunicacao.resposta == 0x08)
                                pecaOK++;
                            if(comunicacao.resposta == 0x0F)
                                sincronismo++;
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
            
            case FALHA_FIM_CURSO:
            {
                comunicacao.endereco = 0x0F;
                comunicacao.resposta = 0x00;   
                USART_Write(comunicacao.byte);
                pwm_setado = PWM_LAMPADA_DESLIGADA;
                LED_DEFEITO_TESTE = 1;
                BUZZER = 1;
                timer = 0;
                estagio = TESTE_FINALIZADO;
            }
            break;
 
            case FALHA:
            {
                pwm_setado = PWM_LAMPADA_DESLIGADA;
                LED_DEFEITO_TESTE = 1;
                BUZZER = 1;
                timer = 0;
                estagio = TESTE_FINALIZADO;
            }
            break;
            
            case OK:
            {
                pwm_setado = PWM_LAMPADA_DESLIGADA;
                LED_TESTE_OK = 1;
                estagio = TESTE_FINALIZADO;
            }
            break;
            
            case TESTE_FINALIZADO:
            {
                if(timer > TEMPO_BUZZER)
                {
                    BUZZER = 0;
                    comunicacao.endereco = 0x0B;
                    comunicacao.resposta = naoliga;
                    USART_Write(comunicacao.byte);
                    comunicacao.endereco = 0x0C;
                    comunicacao.resposta = naodesliga;
                    USART_Write(comunicacao.byte);
                    comunicacao.endereco = 0x0D;
                    comunicacao.resposta = sincronismo;
                    USART_Write(comunicacao.byte);
                    comunicacao.endereco = 0x0E;
                    comunicacao.resposta = pecaOK;
                    USART_Write(comunicacao.byte);
                    comunicacao.endereco = 0x00;
                    if(estado.chavecopo == 1)
                        comunicacao.resposta = 1;
                    else
                        comunicacao.resposta = 0;
                    if(estado.chaveNF == 1)
                        comunicacao.resposta += 0x02;
                    else
                    {
                        if(estado.chaveNA == 1)
                            comunicacao.resposta += 0x08;
                        else
                            comunicacao.resposta += 0x04;
                    }
                    USART_Write(comunicacao.byte);
                    timer = 0;
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