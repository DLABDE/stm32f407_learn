/*点灯：（D2）PA6，（D3）PA7低电平点灯*/
#include "stm32f4xx.h"
void delay(uint16);

int main(void)
{
	RCC_AHB1ENR|=(1<<0);//打开GPIOA的时钟使能
	
	GPIOA->MODER&=~((0x03)<<(2*6));//配置PA6为端口复位状态
	GPIOA->MODER|=(1<<(2*6));//配置PA6为端口输出状态
	GPIOA->BSRRR=(1<<6);//配置PA6为复位，可对ODR有效操作
	
	GPIOA->MODER&=~((0x30)<<(2*7));
	GPIOA->MODER|=(1<<(2*7));
	GPIOA->BSRRR=(1<<7);
	while(1)
	{
		GPIOA->ODR&=~(1<<6);//PA6端口低电平
		GPIOA->ODR|=(1<<7);//PA7端口高电平
		delay(100);
		GPIOA->ODR|=(1<<6);//PA6端口高电平
		GPIOA->ODR&=~(1<<7);//PA7端口低电平
		delay(100);
	}
}

void delay(uint16 time)
{    
   uint16 i=0;  
   while(time--)
   {
      i=12000;
      while(i--) ;    
   }
}

void SystemInit(void)
{
	//欺骗
}
