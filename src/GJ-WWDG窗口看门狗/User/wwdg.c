#include "wwdg.h"

static uint8_t wwdg_cnt ;//记录计数器值

// WWDG 中断优先级初始化
static void WWDG_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

//tr计时器的值(0x7f~0x40)
//wr窗口值(0x7f~0x40)
/*prv预分频器值,取值
	 @arg WWDG_Prescaler_1: WWDG counter clock = (PCLK1(42MHz)/4096)/1  约10253Hz 97us
	 @arg WWDG_Prescaler_2: WWDG counter clock = (PCLK1(42MHz)/4096)/2	 约5126Hz	195us
	 @arg WWDG_Prescaler_4: WWDG counter clock = (PCLK1(42MHz)/4096)/4	 约2563Hz	390us
	 @arg WWDG_Prescaler_8: WWDG counter clock = (PCLK1(42MHz)/4096)/8  约1281Hz	780us

例：
	参数设置为：
		tr = 127(0x7f，tr的最大值)
		wr = 80（0x50, 0x40为最小wr最小值
		prv=WWDG_Prescaler_8
780us*(127-80)=36.6ms<刷新窗口<780us*（127-0x40+1）=49.9ms
(在36.6ms前喂狗,复位;在49.9ms后没有喂狗,复位;参考自身程序运行时间进行参数设置)	
 */
void WWDG_Config(uint8_t tr, uint8_t wr, uint32_t prv)
{	
	wwdg_cnt = tr;//保存CNT配置，用在喂狗函数
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);// 开启 WWDG 时钟
	WWDG_SetPrescaler( prv );// 设置预分频器的值
	WWDG_SetWindowValue( wr );// 设置上窗口值
	WWDG_Enable(tr);	// 设置计数器的值，使能WWDG
	WWDG_ClearFlag();	// 清除提前唤醒中断标志位
	WWDG_NVIC_Config();	// 配置WWDG中断优先级
	WWDG_EnableIT();// 开WWDG 中断
}


void WWDG_Feed(void)// 喂狗
{
	//刷新计数器
	WWDG_SetCounter( wwdg_cnt );
}
