#include "generaltim.h"

//ͨ�ö�ʱ�� TIMx,x[1,8]�ж����ȼ�����
static void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);// �����ж���Ϊ0
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQn; // �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	// ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;// ���������ȼ�	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
TIM_TimeBaseInitTypeDef�ṹ��
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û��
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û��
 * TIM_RepetitionCounter ��TIMx,x[1,8]��
 *-----------------------------------------------------------------------------
*/
//TOUT=(TIM_Period+1)*(TIM_Prescaler+1)/(TIMxCLK)
//10000*8400/84000000=1s
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	// �˴�Ĭ��ͨ�ö�ʱ��ʱ��ԴTIMxCLK = HCLK/2=84MHz 
  RCC_APB1PeriphClockCmd(GENERAL_TIM_CLK, ENABLE);//�򿪶�ʱ��ʱ��Դ

  TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;	//����Ԥ��Ƶϵ������ʱ��Ƶ��TIMxCLK/(TIM_Prescaler+1)=10000Hz
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;// ���ϼ�����ʽ
  TIM_TimeBaseStructure.TIM_Period = 10000-1; //�����Զ�װ��ֵ���ۼ� TIM_Period�������һ�����»����ж� 	
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;// ���벶��
  
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);//��ʼ����ʱ��
	
	TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);// �����ʱ�������жϱ�־λ
	TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);// ������ʱ�������ж�
	TIM_Cmd(GENERAL_TIM, ENABLE);	// ʹ�ܶ�ʱ��
}

void TIMx_Configuration(void)
{
	TIMx_NVIC_Configuration();	
  TIM_Mode_Config();
}
