#include "stm32f4xx.h"
//MCO1�����ʱ���ź�,PA8
//MCO2�����ʱ���ź�,PA9

void mcoinit()//��ʼ��mco��
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Pin=GPIO_Pin_8;
	gpio_init.GPIO_Speed=GPIO_High_Speed;
	gpio_init.GPIO_PuPd=GPIO_PuPd_UP;
	gpio_init.GPIO_Mode=GPIO_Mode_AF;//����ģʽ
	gpio_init.GPIO_OType=GPIO_OType_PP;//�������
	GPIO_Init(GPIOA,&gpio_init);
	
	gpio_init.GPIO_Pin=GPIO_Pin_9;
	GPIO_Init(GPIOA,&gpio_init);
}


int main(void)
{
	mcoinit();
	RCC_MCO1Config(RCC_MCO1Source_PLLCLK,RCC_MCO1Div_1);//ȷ��ʱ����Դ�ͷ�Ƶ����
	RCC_MCO2Config(RCC_MCO2Source_SYSCLK,RCC_MCO2Div_1);
}
