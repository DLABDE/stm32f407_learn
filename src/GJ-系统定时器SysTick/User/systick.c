#include "systick.h"

//内核中，只能递减（24位）

__IO uint32_t timedata;

void systickinit()
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if (SysTick_Config(SystemCoreClock / 1000))//168000000=1s/1000=1ms一次中断
		/* Capture error */ 
		while (1);
	timedata=0;
}
