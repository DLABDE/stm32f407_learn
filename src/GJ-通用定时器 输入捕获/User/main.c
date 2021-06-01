#include "stm32f4xx.h"
#include "generaltim.h"
#include "led.h"
#include "timer.h"
#include "usart.h"
#include <stdio.h>

extern u8  TIM5CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ 

int main(void)
{
	long long temp=0;  
	gpiousart();
	usartinit();
	
	TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //��1Mhz��Ƶ�ʼ��� 
	while(1)
	{
		if(TIM_GetCapture1(TIM14)==300)TIM_SetCompare1(TIM14,0);			 
 		if(TIM5CH1_CAPTURE_STA&0X80)        //�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 		         //���ʱ���ܺ�
			temp+=TIM5CH1_CAPTURE_VAL;		   //�õ��ܵĸߵ�ƽʱ��
			printf("HIGH:%lld us\r\n",temp); //��ӡ�ܵĸߵ�ƽʱ��
			TIM5CH1_CAPTURE_STA=0;			     //������һ�β���
		}
	}
}
