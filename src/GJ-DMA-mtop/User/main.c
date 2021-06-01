#include "stm32f4xx.h"
#include "led.h"
#include "dma.h"
#include "usart.h"
#include <stdio.h>

//M TO P 内存到外设

extern uint8_t SendBuff[SENDBUFF_SIZE];

static void Delay(__IO uint32_t nCount)	 
{
	for(; nCount != 0; nCount--);
}

int main(void)
{
	uint16_t i;
	
	led_inint();
	GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_SET);
	
	gpiousart();
	usartinit();
	DMA_Config(); 
  
	for(i=0;i<SENDBUFF_SIZE;i++)
  {
    SendBuff[i]	 = '1';
  }

  /* USART1 向 DMA发出TX请求 */
  USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	//while( DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) == RESET); //等待传输完成
	//DMA_Cmd(DMA_STREAM, DISABLE);              //关闭DMA        
  //DMA_ClearFlag(DMA_STREAM,DMA_CHANNEL);
	
  /* 此时CPU是空闲的，可以干其他的事情 */  
  while(1)
  {
    GPIO_ToggleBits(GPIOA,GPIO_Pin_7);
    Delay(0xFFFFF);
  }
}
