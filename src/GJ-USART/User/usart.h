#ifndef _USART_H
#define _USART_H


#include "stm32f4xx.h"

void gpiousart(void);
void usartinit(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_string( USART_TypeDef * pUSARTx, char *str);

#endif /*_USART_H*/
