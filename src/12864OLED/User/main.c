#include "stm32f4xx.h"
#include "usart.h"
#include "oled.h"
#include <stdio.h>
#include "bmp.h"

/*126*64*/

static void Delay_ms(u16 time)
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
	/*
	char Buffer[15] = "Hello, World!";
	char Bufferr[15];
	int Num=1208;
	float Data=3.14;
	*/
	char Bufferr[15];
	int Num=1208;
	float Data=3.14;
	
	gpiousart();
	usartinit();
	printf("this is stm32f4\nI am OLED-CODE-TEXT\n");
	
	GPIO_OLED_InitConfig();
	OLED_Clear();
	while(1)
	{
		//打印字符串
//		OLED_Clear();
//		OLED_ShowString(20, 2, Buffer);
//		Delay_ms(600);
		
		
		//打印数字
		OLED_Clear();
		OLED_ShowNum(0, 2, Num, 8, 16);
		sprintf(Bufferr, "%0.2f", Data);  //浮点转换字符串
		OLED_ShowString(0,0, Bufferr);
		OLED_ShowNum(0, 0, Data, 8, 16);
		Delay_ms(600);

		
		//打印自定义字符
		OLED_Clear();
		OLED_ShowCHinese(0, 2, 0);
		OLED_ShowCHinese(14, 2, 1);
		OLED_ShowCHinese(28, 2, 2);
		OLED_ShowCHinese(42, 2, 3);
		Delay_ms(600);
		
		//打印BMP图片
		OLED_Clear();
		OLED_DrawBMP(0,0,126,8,BMP);
		Delay_ms(500);
	}
}

//解析下达数据并存入结构体
void eat_msg(uint8_t *res,uint8_t max)
{

}
