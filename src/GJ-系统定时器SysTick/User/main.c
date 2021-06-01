#include "stm32f4xx.h"
#include "systick.h"
#include "led.h"

extern __IO uint32_t timedata;

int main(void)
{
	led_inint();
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	systickinit();
	while(1)
	{
		if (timedata==1000)
		{
			timedata=0;
			GPIO_ToggleBits(GPIOA,GPIO_Pin_6);
		}
	}
}
