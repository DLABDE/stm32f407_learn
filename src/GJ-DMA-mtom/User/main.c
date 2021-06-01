#include "stm32f4xx.h"
#include "led.h"
#include "dma.h"

//M TO M 内存到内存

int main(void)
{
	led_inint();//初始化

	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	DMA_Config(); 
  
	if(konwyes()==0)//无问题
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	}
	
	while(1)
	{
	}
}
