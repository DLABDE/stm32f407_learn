/* EXTI�ⲿ�ж��¼����� */
#include "stm32f4xx.h"
#include "led.h"
#include "exti.h"

int main(void)
{
	led_inint();
	initexti();	/* ʱ���ߡ�������EXTI��ʼ�� */
	initnvic();/* NVIC��ʼ�� */
	while(1);
}
