#include "stm32f4xx.h"
#include "main.h"
#include "generaltim.h"
#include "led.h"
int main(void)
{
	led_inint();
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	
	TIMx_Configuration();
	while(1)
	{
		
	}
}
