#include "stm32f4xx.h"
#include "wkup.h"
#include "led.h"

/*开机默认进入待机，按键3S切换
待机模式下不能烧录*/

static void delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=0xafff;  
      while(i--) ;    
   }
}

int main(void)
{
	led_inint();
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
	GPIO_SetBits(GPIOA, GPIO_Pin_6);
	
	WKUP_Init();				//待机唤醒初始化
	
	while(1)
	{
		GPIO_ToggleBits(GPIOA,GPIO_Pin_6);
		delay_ms(100);
	}
}
