#include "stm32f4xx.h"
#include "led.h"
#include "dma.h"

//M TO M �ڴ浽�ڴ�

int main(void)
{
	led_inint();//��ʼ��

	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	DMA_Config(); 
  
	if(konwyes()==0)//������
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	}
	
	while(1)
	{
	}
}
