#include "wwdg.h"

static uint8_t wwdg_cnt ;//��¼������ֵ

// WWDG �ж����ȼ���ʼ��
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

//tr��ʱ����ֵ(0x7f~0x40)
//wr����ֵ(0x7f~0x40)
/*prvԤ��Ƶ��ֵ,ȡֵ
	 @arg WWDG_Prescaler_1: WWDG counter clock = (PCLK1(42MHz)/4096)/1  Լ10253Hz 97us
	 @arg WWDG_Prescaler_2: WWDG counter clock = (PCLK1(42MHz)/4096)/2	 Լ5126Hz	195us
	 @arg WWDG_Prescaler_4: WWDG counter clock = (PCLK1(42MHz)/4096)/4	 Լ2563Hz	390us
	 @arg WWDG_Prescaler_8: WWDG counter clock = (PCLK1(42MHz)/4096)/8  Լ1281Hz	780us

����
	��������Ϊ��
		tr = 127(0x7f��tr�����ֵ)
		wr = 80��0x50, 0x40Ϊ��Сwr��Сֵ
		prv=WWDG_Prescaler_8
780us*(127-80)=36.6ms<ˢ�´���<780us*��127-0x40+1��=49.9ms
(��36.6msǰι��,��λ;��49.9ms��û��ι��,��λ;�ο������������ʱ����в�������)	
 */
void WWDG_Config(uint8_t tr, uint8_t wr, uint32_t prv)
{	
	wwdg_cnt = tr;//����CNT���ã�����ι������
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);// ���� WWDG ʱ��
	WWDG_SetPrescaler( prv );// ����Ԥ��Ƶ����ֵ
	WWDG_SetWindowValue( wr );// �����ϴ���ֵ
	WWDG_Enable(tr);	// ���ü�������ֵ��ʹ��WWDG
	WWDG_ClearFlag();	// �����ǰ�����жϱ�־λ
	WWDG_NVIC_Config();	// ����WWDG�ж����ȼ�
	WWDG_EnableIT();// ��WWDG �ж�
}


void WWDG_Feed(void)// ι��
{
	//ˢ�¼�����
	WWDG_SetCounter( wwdg_cnt );
}
