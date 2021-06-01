#include "exti.h"


/* ʱ���ߡ�������EXTI��ʼ�� */
void initexti()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE,ENABLE);
	/* �����жϰ��� */
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Pin=GPIO_Pin_0;
	gpio_init.GPIO_PuPd=GPIO_PuPd_DOWN;
	gpio_init.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(GPIOA,&gpio_init);
	
	gpio_init.GPIO_Pin=GPIO_Pin_4;
	gpio_init.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOE,&gpio_init);
	
	/* ��ʼ��EXTI */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//SYSCFGʹ��
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);//EXTI�ж�Դ���ӵ�PA0
	
	EXTI_InitTypeDef EXTI_InitStructure;//��ʼ���ṹ��
	//PA0
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//����PX0��
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	//PE4
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);//EXTI�ж�Դ���ӵ�PE4
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;//����PX4��
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���,��һ�¿����ڰ������»򵯿�ʱ�ж�
  EXTI_Init(&EXTI_InitStructure);
}

/* NVIC��ʼ�� */
void initnvic()
{
	NVIC_InitTypeDef NVIC_InitStructure;  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//���ȼ���һ��
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//0���ж�Դ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//�����ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
	//��ͬ�ǰ��£����ԾͲ����������ж����ȼ���
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//4���ж�Դ
	//����EXTI9_IRQn�����startup_stm32f40xx.s
  NVIC_Init(&NVIC_InitStructure);
}
