#include "stm32f4xx.h"
#include "pwm.h"
#include "adc.h"
#include "oled.h"
#include "usart.h"
#include <stdio.h>

__IO uint16_t ADC_Value;

void delay(uint32_t time)
{
	for(;time>0;time--);
}

int main(void)
{
	u16 led0pwmval=0;    
	u8 dir=1;
	
	float ADC_Vol;
	char Buffer[15] = "THE PWM TEST";
	char Bufferr[15] = "THE V:";
	char data[7];
	
	gpiousart();
	usartinit();
	printf("This is the PWM-DAC\n");
	
	GPIO_OLED_InitConfig();
	OLED_Clear();
	OLED_ShowString(16,0, Buffer);
	OLED_ShowString(0,4, Bufferr);
	
	THEADC_Init();
	
	TIM14_PWM_Init(1000-1,84-1);//84M/84=1Mhz的计数频率,重装载值1000，所以PWM频率为 1M/1000=1Khz.     
	while(1)
	{ 
		delay(0xfffff);
		if(dir)led0pwmval++;//dir==1 led0pwmval递增
		else led0pwmval--;	//dir==0 led0pwmval递减 
 		if(led0pwmval>50-1)dir=0;//led0pwmval到达xxx后，方向为递减
		if(led0pwmval==0)dir=1;	//led0pwmval递减到0后，方向改为递增
		
		//DAC_GetDataOutputValue(DAC_Channel_2);//读取前面设置DAC的值
		ADC_Vol =(float) ADC_Value/4096*(float)3.3; // 读取转换的AD值
    printf("\r\n The current AD value = 0x%04X \r\n", ADC_Value); 
    printf("\r\n The current AD value = %f V \r\n",ADC_Vol);
		printf("back=%d;target=%d\n",(int)(ADC_Vol*1000),2500);
		sprintf(data, "%0.2f", ADC_Vol);  //浮点转换字符串
		OLED_ShowString(60,4, data);
		//OLED_ShowNum(0, 2,6, 4, 16);
		
		TIM_SetCompare1(TIM14,led0pwmval);	//修改比较值，修改占空比
	}
}
