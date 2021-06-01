#include "basictim.h"

/*TIM_TimeBaseInitTypeDef�ṹ��
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
//TOUT=(TIM_Period+1)*(TIM_Prescaler+1)/(TIMxCLK)
//5000*8400/84000000=0.5s

//������ʱ�����ã�6��7��
void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
  TIM_TimeBaseStructure.TIM_Period = 5000-1;//�Զ�װ��ֵ���ۼ� TIM_Period�������һ�����»����ж�*/	
  TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;	//��Ƶϵ��

	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);// ��ʼ����ʱ��
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);// �����ʱ�������жϱ�־λ
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);// ������ʱ�������ж�
	TIM_Cmd(TIM6, ENABLE);	// ʹ�ܶ�ʱ��
}

//�ж�����
void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
