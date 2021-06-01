/*��ƣ���D2��PA6����D3��PA7�͵�ƽ���*/
#include "stm32f4xx.h"
void delay(uint16);

int main(void)
{
	RCC_AHB1ENR|=(1<<0);//��GPIOA��ʱ��ʹ��
	
	GPIOA->MODER&=~((0x03)<<(2*6));//����PA6Ϊ�˿ڸ�λ״̬
	GPIOA->MODER|=(1<<(2*6));//����PA6Ϊ�˿����״̬
	GPIOA->BSRRR=(1<<6);//����PA6Ϊ��λ���ɶ�ODR��Ч����
	
	GPIOA->MODER&=~((0x30)<<(2*7));
	GPIOA->MODER|=(1<<(2*7));
	GPIOA->BSRRR=(1<<7);
	while(1)
	{
		GPIOA->ODR&=~(1<<6);//PA6�˿ڵ͵�ƽ
		GPIOA->ODR|=(1<<7);//PA7�˿ڸߵ�ƽ
		delay(100);
		GPIOA->ODR|=(1<<6);//PA6�˿ڸߵ�ƽ
		GPIOA->ODR&=~(1<<7);//PA7�˿ڵ͵�ƽ
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
	//��ƭ
}
