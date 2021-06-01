#ifndef _GENERALTIM_H_
#define _GENERALTIM_H_

#include "stm32f4xx.h"

#define GENERAL_TIM           		TIM2
#define GENERAL_TIM_CLK       		RCC_APB1Periph_TIM2

#define GENERAL_TIM_IRQn					TIM2_IRQn
#define GENERAL_TIM_IRQHandler    TIM2_IRQHandler


void TIMx_Configuration(void);

#endif /*_GENERALTIM_H_*/
