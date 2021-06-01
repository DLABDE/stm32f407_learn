#include "stm32f4xx.h"
#include "main.h"
#include "led.h"
#include "usart.h"
#include <stdio.h>

static void Delay_ms(u16 time)
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
	led_inint();//初始化
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);//复位
	
	gpiousart();
	usartinit();
	Usart_string(USART1,"i am stm32f407\n");
	Usart_SendByte(USART1,0x12);
	printf("hahaha\n");
	int a=0,x=200;
	while(1)
	{

	}
}

//解析下达数据并存入结构体
void eat_msg(uint8_t *res,uint8_t max)
{

}
