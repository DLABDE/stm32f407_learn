#include "led.h"

void led_inint(void)//��ʼ��
{
	//1��������ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//��ANB1��������ʱ��ʹ��
	//2�������ʼ���ṹ��
	GPIO_InitTypeDef gpio_init;
	//3�����ýṹ���Ա
	gpio_init.GPIO_Pin=GPIO_Pin_6;//6��
	gpio_init.GPIO_Speed=GPIO_Low_Speed;//����
	gpio_init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio_init.GPIO_Mode=GPIO_Mode_OUT;//���ģʽ
	gpio_init.GPIO_OType=GPIO_OType_PP;//�������
	//4�����ó�ʼ������������Ա����д��Ĵ���
	GPIO_Init(GPIOA,&gpio_init);
	
	gpio_init.GPIO_Pin=GPIO_Pin_7;
	GPIO_Init(GPIOA,&gpio_init);
}
