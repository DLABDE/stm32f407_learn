#include "basictim.h"

/*TIM_TimeBaseInitTypeDef结构体
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
//TOUT=(TIM_Period+1)*(TIM_Prescaler+1)/(TIMxCLK)
//5000*8400/84000000=0.5s

//基本定时器设置（6或7）
void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
  TIM_TimeBaseStructure.TIM_Period = 5000-1;//自动装载值，累计 TIM_Period个后产生一个更新或者中断*/	
  TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;	//分频系数

	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);// 初始化定时器
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);// 清除定时器更新中断标志位
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);// 开启定时器更新中断
	TIM_Cmd(TIM6, ENABLE);	// 使能定时器
}

//中断设置
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
