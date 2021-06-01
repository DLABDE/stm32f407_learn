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
	
	TIM14_PWM_Init(500-1,84-1);//84M/84=1Mhz的计数频率,重装载值1000，所以PWM频率为 1M/1000=1Khz.     
	while(1)
	{ 
		delay(0xffff);
		if(dir)led0pwmval++;//dir==1 led0pwmval递增
		else led0pwmval--;	//dir==0 led0pwmval递减 
 		if(led0pwmval>300-1)dir=0;//led0pwmval到达xxx后，方向为递减
		if(led0pwmval==0)dir=1;	//led0pwmval递减到0后，方向改为递增
 
		TIM_SetCompare1(TIM14,led0pwmval);	//修改比较值，修改占空比
	}
}
