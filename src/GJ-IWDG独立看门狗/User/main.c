#include "stm32f4xx.h"
#include "led.h"
#include "iwdg.h"

void delay(uint32_t time)
{
	for(;time>0;time--);
}

int main(void)
{
	led_inint();//≥ı ºªØ
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);
	
	IWDG_Config(IWDG_Prescaler_64 ,625);
	delay(0xffffff);
	while(1)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_SET);
		delay(0xffffff);
		GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_RESET);
		delay(0xffffff);
		//IWDG_Feed();
	}
}
