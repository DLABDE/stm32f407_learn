#ifndef _DMA_H
#define _DMA_H

#define DMA_STREAM               DMA2_Stream5//5������
#define DMA_CHANNEL              DMA_Channel_4//4ͨ��
#define DMA_STREAM_CLOCK         RCC_AHB1Periph_DMA2 //������AHB1

#define SENDBUFF_SIZE            32
#define TIMEOUT_MAX              1000 /* Maximum timeout value */

#include "stm32f4xx.h"

void DMA_Config(void);

#endif /*_DMA_H*/
