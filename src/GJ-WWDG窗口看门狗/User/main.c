#include "stm32f4xx.h"
#include "led.h"
#include "usart.h"
#include "wwdg.h"
#include <stdio.h>

static void Delay(__IO uint32_t nCount)	 //延时函数
{
	for(; nCount != 0; nCount--);
}

int main(void)
{
	uint8_t wwdg_tr, wwdg_wr;
	
	led_inint();
	GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);//灭
	GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_SET);
	
	gpiousart();
	usartinit();
	
	if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)//是狗复位
  {
		RCC_ClearFlag();//清除标志
		GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_RESET);
		printf("DOG RESET!!\n");
		Delay(0XFFFFFF);	
		GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);
  }
  else//非狗复位（电复位等）
  {
		GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_RESET);
		printf("NO DOG RESET!!\n");
		Delay(0XFFFFFF);	
		GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_SET);
  }	
	
	
	WWDG_Config(127,80,WWDG_Prescaler_8);	//配置狗
	wwdg_wr = WWDG->CFR & 0X7F;// 窗口值我们在初始化的时候设置成0X5F，这个值不会改变
	
	while(1)
	{
		//GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_RESET);
		//GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_RESET);
		
		wwdg_tr = WWDG->CR & 0X7F;
		if( wwdg_tr < wwdg_wr )
		{
			WWDG_Feed();// 喂狗，重新设置计数器的值为最大0X7F
		}
	}
}
