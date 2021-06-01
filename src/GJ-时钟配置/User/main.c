#include "stm32f4xx.h"
#include "clock.h"
#include "led.h"

/* ʹ���ڲ�����HSIʱ��
Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
        ;IMPORT  SystemInit
        IMPORT  __main

                 ;LDR     R0, =SystemInit
                 ;BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP
*/
void delay(uint32_t time)
{
	for(;time>0;time--);
}

int main(void)
{
	//����ʹ�ø���HSEʱ�ӣ�User_SetSysClock(void)������8M������SetSysClock()25M
	//User_SetSysClock();
	HSE_SetSysClock();
	//HSI_SetSysClock();
	
	led_inint();//��ʼ��
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);//��λ
	
	while(1)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);
		delay(0xfffff);
		GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_RESET);
		delay(0xfffff);
	}
}
