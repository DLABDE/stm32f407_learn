#include "stm32f4xx.h"
#include "led.h"
#include "adc.h"
#include "oled.h"
#include "usart.h"
#include <stdio.h>

__IO uint16_t ADC_Value;


static void Delay(__IO uint32_t nCount)	
{
	for(; nCount != 0; nCount--);
}

int main(void)
{
	float ADC_Vol,tem;
	char Buffer[15] = "CPU TEM TEST";
	char Bufferr[15] = "THE TEM:";
	char data[7];
	
	GPIO_OLED_InitConfig();
	OLED_Clear();
	OLED_ShowString(24,0, Buffer);
	OLED_ShowString(0,4, Bufferr);
	
	THEADC_Init();
		
	gpiousart();
	usartinit();
	printf("This is the CPU TEM\n");
	
	while(1)
	{
		ADC_Vol =(float) ADC_Value/4096*(float)3.3; // 读取转换的AD值
    printf("\r\n The current AD value = %f V \r\n",ADC_Vol);
		tem=((ADC_Vol-0.76f)/0.0025f + 25.00f);
		printf("\r\n The CPU TEM value = %f\r\n",tem); 
		
		sprintf(data, "%0.2f", tem);  //浮点转换字符串
		OLED_ShowString(65,4, data);
		Delay(0xAfffff); 
	}
}
