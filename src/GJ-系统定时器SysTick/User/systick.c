#include "systick.h"

//�ں��У�ֻ�ܵݼ���24λ��

__IO uint32_t timedata;

void systickinit()
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	if (SysTick_Config(SystemCoreClock / 1000))//168000000=1s/1000=1msһ���ж�
		/* Capture error */ 
		while (1);
	timedata=0;
}
