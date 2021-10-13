#include "usart.h"
#include "main.h"

#define RX_BUFFER_SIZE 3

volatile unsigned char rxBuffer[RX_BUFFER_SIZE];
volatile unsigned char rxCount;

void USART_Initialize(void)
{
    RCSTA = 0b10010000;
    TXSTA = 0b00100000;
    
    //Baud Rate: 2.400
    SPBRG = 0x81;
    
    rxCount = 0;
    
    PIE1bits.RCIE = 1;
}

void USART_RX_ISR(void)
{
    rxBuffer[rxCount] = RCREG;
    rxCount++;
    if(rxCount >= RX_BUFFER_SIZE)
        rxCount = RX_BUFFER_SIZE - 1;
    if(RCSTAbits.OERR == 1)
    {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
        rxCount = 0;
    }
    PIR1bits.RCIF = 0;
}

unsigned char USART_RX_Ready(void)
{
    return rxCount;
}

unsigned char USART_Read(void)
{
    unsigned char data;
    
    rxCount--;
    data = rxBuffer[rxCount];
    return data;
}

void USART_Write(unsigned char data)
{
    while(PIR1bits.TXIF == 0)
    {
    }
    TXREG = data;
}

void USART_RX_Clear_Buffer(void)
{
    rxCount = 0;
}