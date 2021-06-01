#include "stm32f4xx.h"
#include "led.h"

void delay(uint32_t time)
{
	for(;time>0;time--);
}

int main(void)
{
	led_inint();//≥ı ºªØ
	//GPIO_ResetBits(GPIOA,GPIO_Pin_6);//∏¥Œª
	while(1)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);
		delay(0xfffff);
		//GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_RESET);
		delay(0xfffff);
	}
}
