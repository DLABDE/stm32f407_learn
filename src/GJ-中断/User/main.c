/* EXTI外部中断事件控制 */
#include "stm32f4xx.h"
#include "led.h"
#include "exti.h"

int main(void)
{
	led_inint();
	initexti();	/* 时钟线、按键、EXTI初始化 */
	initnvic();/* NVIC初始化 */
	while(1);
}
