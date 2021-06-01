#include "stm32f4xx.h"
#include "led.h"
#include "adc.h"
#include "dac.h"
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
	float ADC_Vol;
	char Buffer[15] = "THE ADDA TEST";
	char Bufferr[15] = "THE DA:";
	char data[7];
	
	gpiousart();
	usartinit();
	printf("This is the ADC-DAC\n");
	
	GPIO_OLED_InitConfig();
	OLED_Clear();
	OLED_ShowString(16,0, Buffer);
	OLED_ShowString(0,4, Bufferr);
	
	THEADC_Init();
	Dac2_Init();
		
	Dac2_Set_Vol(2500);//2.5V
	
	while(1)
	{
		//DAC_GetDataOutputValue(DAC_Channel_2);//读取前面设置DAC的值
		ADC_Vol =(float) ADC_Value/4096*(float)3.3; // 读取转换的AD值
    printf("\r\n The current AD value = 0x%04X \r\n", ADC_Value); 
    printf("\r\n The current AD value = %f V \r\n",ADC_Vol);
		
		sprintf(data, "%0.2f", ADC_Vol);  //浮点转换字符串
		OLED_ShowString(60,4, data);
		//OLED_ShowNum(0, 2,6, 4, 16);
		Delay(0x1ff1ff); 
		printf("back=%d;target=%d\n",(int)(ADC_Vol*1000),2500);
	}
}
