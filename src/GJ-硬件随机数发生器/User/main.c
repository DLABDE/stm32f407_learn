#include "stm32f4xx.h"
#include "usart.h"
#include "oled.h"
#include "rng.h"
#include <stdio.h>

static void delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=0xafff;  
      while(i--) ;    
   }
}

int main(void)
{
	char Buffer[15] = "THE RNG ERROR!";
	char Bufferr[15] = "THE RNG TEST";
	char BBuffer[15] = "THE RANDOM:";
	u32 random;
	
	gpiousart();
	usartinit();
	printf("this is stm32f4\nI am RNG-CODE-TEXT\n");
	
	GPIO_OLED_InitConfig();
	OLED_Clear();
	
	OLED_ShowString(0, 0, Bufferr);
	OLED_ShowString(0, 4, BBuffer);
	
	while(RNG_Init())	 		//初始化随机数发生器
	{ 
		OLED_ShowString(0, 0, Buffer);//ERROR
	} 
	
	while(1)
	{
		random=RNG_Get_RandomRange(0,100);
		printf("%s%d\n",BBuffer,random);
		OLED_ShowNum(90, 4, random, 3, 16);
		delay_ms(700);
	}
}

//解析下达数据并存入结构体
void eat_msg(uint8_t *res,uint8_t max)
{

}
