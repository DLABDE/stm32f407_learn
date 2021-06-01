#include "stm32f4xx.h"
#include "led.h"
#include "usart.h"
#include "wwdg.h"
#include <stdio.h>

static void Delay(__IO uint32_t nCount)	 //��ʱ����
{
	for(; nCount != 0; nCount--);
}

int main(void)
{
	uint8_t wwdg_tr, wwdg_wr;
	
	led_inint();
	GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);//��
	GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_SET);
	
	gpiousart();
	usartinit();
	
	if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)//�ǹ���λ
  {
		RCC_ClearFlag();//�����־
		GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_RESET);
		printf("DOG RESET!!\n");
		Delay(0XFFFFFF);	
		GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);
  }
  else//�ǹ���λ���縴λ�ȣ�
  {
		GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_RESET);
		printf("NO DOG RESET!!\n");
		Delay(0XFFFFFF);	
		GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_SET);
  }	
	
	
	WWDG_Config(127,80,WWDG_Prescaler_8);	//���ù�
	wwdg_wr = WWDG->CFR & 0X7F;// ����ֵ�����ڳ�ʼ����ʱ�����ó�0X5F�����ֵ����ı�
	
	while(1)
	{
		//GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_RESET);
		//GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_RESET);
		
		wwdg_tr = WWDG->CR & 0X7F;
		if( wwdg_tr < wwdg_wr )
		{
			WWDG_Feed();// ι�����������ü�������ֵΪ���0X7F
		}
	}
}
