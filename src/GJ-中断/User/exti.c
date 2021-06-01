#include "exti.h"


/* 时钟线、按键、EXTI初始化 */
void initexti()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE,ENABLE);
	/* 配置中断按键 */
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Pin=GPIO_Pin_0;
	gpio_init.GPIO_PuPd=GPIO_PuPd_DOWN;
	gpio_init.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(GPIOA,&gpio_init);
	
	gpio_init.GPIO_Pin=GPIO_Pin_4;
	gpio_init.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOE,&gpio_init);
	
	/* 初始化EXTI */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//SYSCFG使能
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);//EXTI中断源链接到PA0
	
	EXTI_InitTypeDef EXTI_InitStructure;//初始化结构体
	//PA0
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//挂载PX0线
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	//PE4
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);//EXTI中断源链接到PE4
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;//挂载PX4线
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发,改一下可以在按键按下或弹开时中断
  EXTI_Init(&EXTI_InitStructure);
}

/* NVIC初始化 */
void initnvic()
{
	NVIC_InitTypeDef NVIC_InitStructure;  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//优先级第一组
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//0线中断源
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//子优先级1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
	//不同是按下，所以就不区别具体的中断优先级了
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//4线中断源
	//例如EXTI9_IRQn则错，看startup_stm32f40xx.s
  NVIC_Init(&NVIC_InitStructure);
}
