#include "stm32f4xx.h"
#include "led.h"
#include "basictim.h"

int main(void)
{
	led_inint();
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);
	
	TIM_Mode_Config();
	TIMx_NVIC_Configuration();
	while(1)
	{
		
	}
}
