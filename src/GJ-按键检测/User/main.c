#include "stm32f4xx.h"
#include "nkey.h"
#include "led.h"

int main(void)
{
	key_inint();
	led_inint();
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	while(1)
	{
		if(key_scan(GPIOA,GPIO_Pin_0,1))
		{
			GPIO_ToggleBits(GPIOA,GPIO_Pin_6);
		}
		if(key_scan(GPIOE,GPIO_Pin_4,0))
		{
			GPIO_ToggleBits(GPIOA,GPIO_Pin_7);
		}
	}
}
