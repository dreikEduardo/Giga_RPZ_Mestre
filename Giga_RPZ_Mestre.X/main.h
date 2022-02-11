#ifndef MAIN_H
#define MAIN_H

#include <xc.h>

// Declaração das variáveis globais
unsigned int timerbotao1 = 0;
unsigned int timerbotao2 = 0;
unsigned char timerfimcurso = 0;
unsigned char timerchaveNF = 0;
unsigned char timerchaveNA = 0;
unsigned char timerchavecopo = 0;
unsigned int timer = 0;
unsigned int timerpwm = 0;
unsigned char timeoutRx = 0;
unsigned int pwm_setado = 0;
unsigned int pwm_atual = 0;
unsigned char endereco = 0;
union
{
    unsigned char byte;
    struct
    {
        unsigned endereco:4;
        unsigned resposta:4;
    };
}comunicacao;
union
{
    unsigned int byte;
    struct
    {
        unsigned endereco1:1;
        unsigned endereco2:1;
        unsigned endereco3:1;
        unsigned endereco4:1;
        unsigned endereco5:1;
        unsigned endereco6:1;
        unsigned endereco7:1;
        unsigned endereco8:1;
        unsigned endereco9:1;
        unsigned endereco10:1;
        unsigned :6;
    };
}escravo;

// Declaração das flags
struct
{
    unsigned botao1:1;
    unsigned botao2:1;
    unsigned fimcurso:1;
    unsigned chaveNF:1;
    unsigned chaveNA:1;
    unsigned chavecopo:1;
    unsigned testando:1;
    unsigned reteste:1;
    unsigned falha:1;
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
    VERIFICA_PLACAS_LIGA,
    MENSAGEM_RETORNO_PLACAS_LIGA,
    TESTE_DESLIGA,
    AGUARDA_TEMPO_DESLIGA,
    VERIFICA_PLACAS_DESLIGA,
    MENSAGEM_RETORNO_PLACAS_DESLIGA,
    RETESTE,
    VERIFICA_PLACAS,
    MENSAGEM_RETORNO_PLACAS,
    FALHA_FIM_CURSO,
    FALHA,
    OK,
    TESTE_FINALIZADO
} estagio = INTERROMPIDO;

// Declaração das constantes
#define ENDERECO 0
#define TEMPO_TIMEOUT_RX 80         //80ms -> base de tempo 1ms
#define TEMPO_DEBOUNCE 100          //100ms -> base de tempo 1ms
#define TEMPO_FECHA_TAMPA 5000      //5s -> base de tempo 1ms
#define TEMPO_ESTABILIZACAO 5000    //5s -> base de tempo 1ms
#define TEMPO_LIGA 12000            //12s -> base de tempo 1ms
#define TEMPO_LIGADO 3000           //3s -> base de tempo 1ms
#define TEMPO_DESLIGA 10000         //10s -> base de tempo 1ms
#define TEMPO_DESLIGADO 8000        //8s -> base de tempo 1ms
#define TEMPO_PISCADA 500           //500ms -> base de tempo 1ms
#define TEMPO_BUZZER 2000           //2s -> base de tempo 1ms
#define TEMPO_RAMPA_PWM 10
#define PWM_LAMPADA_LIGADA 1000
#define PWM_LAMPADA_MAX 105
#define PWM_LAMPADA_MAX_COPO 170
#define PWM_LAMPADA_MIN 10
#define PWM_LAMPADA_DESLIGADA 0 

#endif