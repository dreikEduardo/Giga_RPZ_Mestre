#ifndef USART_H
#define USART_H

#include <xc.h>

void USART_Initialize(void);
void USART_RX_ISR(void);
unsigned char USART_RX_Ready(void);
unsigned char USART_Read(void);
void USART_Write(unsigned char data);
void USART_RX_Clear_Buffer(void);

#endif