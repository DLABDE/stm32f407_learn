#include "nkey.h"

void delay(uint32_t time)
{
	for(;time>0;time--);
}

void key_inint(void)//��ʼ��
{
	//1��������ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE,ENABLE);//��ANB1��������ʱ��ʹ��
	//2�������ʼ���ṹ��
	GPIO_InitTypeDef gpio_init;
	//3�����ýṹ���Ա
	gpio_init.GPIO_Pin=GPIO_Pin_0;
	gpio_init.GPIO_Mode=GPIO_Mode_IN;
	gpio_init.GPIO_PuPd=GPIO_PuPd_DOWN;
	//4�����ó�ʼ������������Ա����д��Ĵ���
	GPIO_Init(GPIOA,&gpio_init);
	
	gpio_init.GPIO_Pin=GPIO_Pin_4;
	gpio_init.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOE,&gpio_init);
}

uint8_t key_scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_pin,uint8_t fn)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_pin)==fn)
	{
		delay(0xfff);
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_pin)==fn)
		{
			while(GPIO_ReadInputDataBit(GPIOx,GPIO_pin)==fn);
			return 1;
		}
	}
	return 0;
}
