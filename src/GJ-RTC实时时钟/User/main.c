#include "stm32f4xx.h"
#include "led.h"
#include "usart.h"
#include "rtc.h"
#include "oled.h"
#include <stdio.h>

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
	led_inint();
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	gpiousart();
	usartinit();
	GPIO_OLED_InitConfig();
	OLED_Clear();
	My_RTC_Init();
	printf("this is stm32f4\nTHE RTC TEST!\n");
	
	char Buffer[15] = "RTC TIME";
	char tbuf[40];
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);//配置WAKE UP中断,1秒钟中断一次（0到0一个周期）
	OLED_ShowString(35, 0, Buffer);
	while(1)
	{
		RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
		sprintf((char*)tbuf,"Time:%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
		OLED_ShowString(0, 2,tbuf);
		
		RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
		sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
		OLED_ShowString(0, 4,tbuf);
		
		sprintf((char*)tbuf,"Week:%d",RTC_DateStruct.RTC_WeekDay); 
		OLED_ShowString(0, 6,tbuf);
		
		Delay_ms(200);
	}
}

//解析下达数据并存入结构体
void eat_msg(uint8_t *res,uint8_t max)
{

}
