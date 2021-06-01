#include "stm32f4xx.h"
#include "generaltim.h"
#include "pwm.h"
#include "led.h"

void delay(uint32_t time)
{
	for(;time>0;time--);
}

int main(void)
{
	u16 led0pwmval=0;    
	u8 dir=1;
	
	TIM14_PWM_Init(500-1,84-1);//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ1000������PWMƵ��Ϊ 1M/1000=1Khz.     
	while(1)
	{ 
		delay(0xffff);
		if(dir)led0pwmval++;//dir==1 led0pwmval����
		else led0pwmval--;	//dir==0 led0pwmval�ݼ� 
 		if(led0pwmval>300-1)dir=0;//led0pwmval����xxx�󣬷���Ϊ�ݼ�
		if(led0pwmval==0)dir=1;	//led0pwmval�ݼ���0�󣬷����Ϊ����
 
		TIM_SetCompare1(TIM14,led0pwmval);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	}
}
