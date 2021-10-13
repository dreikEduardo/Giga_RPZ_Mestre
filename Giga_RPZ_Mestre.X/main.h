#ifndef MAIN_H
#define MAIN_H

#include <xc.h>

// Declaração das variáveis globais
unsigned int timerbotao1 = 0;
unsigned int timerbotao2 = 0;
unsigned char timerfimcurso = 0;
unsigned char timerchaveNF = 0;
unsigned char timerchaveNA = 0;
unsigned int timer = 0;
unsigned char timerpwm = 0;
unsigned int pwm_setado = 0;
unsigned int pwm_atual = 0;
unsigned char endereco = 0;

union
{
    unsigned char byte;
    struct
    {
        unsigned endereco:4;
        unsigned tipo:1;
        unsigned resposta:3;
    };
}comunicacao;

// Declaração das flags
struct
{
    unsigned botao1:1;
    unsigned botao2:1;
    unsigned fimcurso:1;
    unsigned chaveNF:1;
    unsigned chaveNA:1;
    unsigned tipo:1;
    unsigned reteste:1;
    unsigned falha:1;
    unsigned dadorecebido:1;
    unsigned :7;
}estado;

static enum _estagio
{
    INTERROMPIDO = 0,
    TESTE_BOTOES,
    INICIA_TESTE,
    TESTE_TAMPA_FECHADA,
	ALIMENTA_PLACA,
    ESTABILIZACAO,
    TESTE_LIGA,
    AGUARDA_TEMPO_LIGA,
    TESTE_DESLIGA,
    AGUARDA_TEMPO_DESLIGA,
    RETESTE,
    VERIFICA_PLACAS,
    MENSAGEM_RETORNO,
    FALHA,
    OK,
    TESTE_FINALIZADO
} estagio = INTERROMPIDO;

// Declaração das constantes
#define ENDERECO 0
#define TEMPO_TIMEOUT_RX 80         //80ms -> base de tempo 1ms
#define TEMPO_DEBOUNCE 100          //100ms -> base de tempo 1ms
#define TEMPO_FECHA_TAMPA 5000      //5s -> base de tempo 1ms
#define TEMPO_ESTABILIZACAO 1000    //1s -> base de tempo 1ms
#define TEMPO_LIGA 5000             //5s -> base de tempo 1ms
#define TEMPO_DESLIGA 6000          //6s -> base de tempo 1ms
#define TEMPO_PISCADA 500           //500ms -> base de tempo 1ms
#define TEMPO_BUZZER 2000           //2s -> base de tempo 1ms
#define TEMPO_RAMPA_PWM 10
#define PWM_LAMPADA_LIGADA 150 
#define PWM_LAMPADA_MAX 120      
#define PWM_LAMPADA_MIN 80
#define PWM_LAMPADA_DESLIGADA 0 

#endif