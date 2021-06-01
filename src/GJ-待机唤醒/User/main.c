#include "stm32f4xx.h"
#include "wkup.h"
#include "led.h"

/*����Ĭ�Ͻ������������3S�л�
����ģʽ�²�����¼*/

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
	
	WKUP_Init();				//�������ѳ�ʼ��
	
	while(1)
	{
		GPIO_ToggleBits(GPIOA,GPIO_Pin_6);
		delay_ms(100);
	}
}
