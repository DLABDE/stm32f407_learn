#include "stm32f4xx.h"
#include "generaltim.h"
#include "led.h"
#include "timer.h"
#include "usart.h"
#include <stdio.h>

extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值 

int main(void)
{
	long long temp=0;  
	gpiousart();
	usartinit();
	
	TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //以1Mhz的频率计数 
	while(1)
	{
		if(TIM_GetCapture1(TIM14)==300)TIM_SetCompare1(TIM14,0);			 
 		if(TIM5CH1_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 		         //溢出时间总和
			temp+=TIM5CH1_CAPTURE_VAL;		   //得到总的高电平时间
			printf("HIGH:%lld us\r\n",temp); //打印总的高点平时间
			TIM5CH1_CAPTURE_STA=0;			     //开启下一次捕获
		}
	}
}
