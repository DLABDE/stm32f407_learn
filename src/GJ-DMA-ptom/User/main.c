#include "stm32f4xx.h"
#include "led.h"
#include "dma.h"
#include "usart.h"
#include <stdio.h>

//2021.4.3£¬ÓÐ´ý²âÊÔ

//p-m

extern uint8_t SendBuff[SENDBUFF_SIZE];

static void Delay(__IO uint32_t nCount)	 
{
	for(; nCount != 0; nCount--);
}


int main(void)
{
	led_inint();
	GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_SET);
	
	gpiousart();
	usartinit();
	DMA_Config(); 
	
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	
	while(1)
  {
    GPIO_ToggleBits(GPIOA,GPIO_Pin_6);
    Delay(0xFFFAFF);
		for (uint8_t i=0;i<SENDBUFF_SIZE;i++)
			USART_SendData(USART1, (uint8_t) SendBuff[i]);
  }
}
