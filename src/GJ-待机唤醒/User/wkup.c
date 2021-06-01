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

//系统进入待机模式
void Sys_Enter_Standby(void)
{		
  while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0);//等待WK_UP按键松开(在有RTC中断时,必须等WK_UP松开再进入待机)
 
	RCC_AHB1PeriphResetCmd(0X04FF,ENABLE);//复位所有IO口
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟		 
	PWR_BackupAccessCmd(ENABLE);//后备区域访问使能

	//关闭相关RTC中断
	RTC_ITConfig(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA,DISABLE);//关闭RTC相关中断
	RTC_ClearITPendingBit(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA);//清楚RTC相关中断标志位。
	
	PWR_ClearFlag(PWR_FLAG_WU);//清除Wake-up 标志
	PWR_WakeUpPinCmd(ENABLE);//设置WKUP用于唤醒
	PWR_EnterSTANDBYMode();	//进入待机模式
	/* Low Power modes configuration functions ************************************
void PWR_EnterSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry);
void PWR_EnterUnderDriveSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry);
void PWR_EnterSTANDBYMode(void);
	查看手册查询多种待机模式*/
	 
}
//检测WKUP脚的信号
//返回值1:连续按下3s以上
//      0:错误的触发	
u8 Check_WKUP(void) 
{
	u8 t=0;
	u8 tx=0;//记录松开的次数
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)//已经按下了
		{
			t++;
			tx=0;
		}
		else 
		{
			tx++; 
			if(tx>3)//超过90ms内没有WKUP信号
			{
				return 0;//错误的按键,按下次数不够
			}
		}
		
		delay_ms(30);
		if(t>=100)//按下超过秒钟
		{
			return 1; //按下3s以上了
		}
	}
}  
 

//PA0 WKUP唤醒初始化
void WKUP_Init(void)
{	  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //PA0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化	

	//(检查是否是正常开)机    		 
  if(Check_WKUP()==0)
	{
		Sys_Enter_Standby();	//不是开机,进入待机模式  
	}
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 连接到中断线0
	
	EXTI_InitTypeDef   EXTI_InitStructure;
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置NVIC
}
