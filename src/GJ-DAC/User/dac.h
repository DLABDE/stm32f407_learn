#ifndef _DAC_H_
#define _DAC_H_

#include "stm32f4xx.h"

void Dac2_Init(void);		//DAC通道2初始化	 	 
void Dac2_Set_Vol(u16 vol);	//设置通道2输出电压

#endif /* _DAC_H_ */
