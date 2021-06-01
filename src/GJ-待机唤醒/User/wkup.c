#include "wkup.h"

static void delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=0xafff;  
      while(i--) ;    
   }
}

//ϵͳ�������ģʽ
void Sys_Enter_Standby(void)
{		
  while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0);//�ȴ�WK_UP�����ɿ�(����RTC�ж�ʱ,�����WK_UP�ɿ��ٽ������)
 
	RCC_AHB1PeriphResetCmd(0X04FF,ENABLE);//��λ����IO��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ��PWRʱ��		 
	PWR_BackupAccessCmd(ENABLE);//���������ʹ��

	//�ر����RTC�ж�
	RTC_ITConfig(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA,DISABLE);//�ر�RTC����ж�
	RTC_ClearITPendingBit(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA);//���RTC����жϱ�־λ��
	
	PWR_ClearFlag(PWR_FLAG_WU);//���Wake-up ��־
	PWR_WakeUpPinCmd(ENABLE);//����WKUP���ڻ���
	PWR_EnterSTANDBYMode();	//�������ģʽ
	/* Low Power modes configuration functions ************************************
void PWR_EnterSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry);
void PWR_EnterUnderDriveSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry);
void PWR_EnterSTANDBYMode(void);
	�鿴�ֲ��ѯ���ִ���ģʽ*/
	 
}
//���WKUP�ŵ��ź�
//����ֵ1:��������3s����
//      0:����Ĵ���	
u8 Check_WKUP(void) 
{
	u8 t=0;
	u8 tx=0;//��¼�ɿ��Ĵ���
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)//�Ѿ�������
		{
			t++;
			tx=0;
		}
		else 
		{
			tx++; 
			if(tx>3)//����90ms��û��WKUP�ź�
			{
				return 0;//����İ���,���´�������
			}
		}
		
		delay_ms(30);
		if(t>=100)//���³�������
		{
			return 1; //����3s������
		}
	}
}  
 

//PA0 WKUP���ѳ�ʼ��
void WKUP_Init(void)
{	  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
	GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //PA0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��	

	//(����Ƿ���������)��    		 
  if(Check_WKUP()==0)
	{
		Sys_Enter_Standby();	//���ǿ���,�������ģʽ  
	}
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 ���ӵ��ж���0
	
	EXTI_InitTypeDef   EXTI_InitStructure;
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
  EXTI_Init(&EXTI_InitStructure);//����
	
	NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����NVIC
}
