#ifndef _NKEY_H
#define _NKEY_H

#include "stm32f4xx.h"

void key_inint(void);
uint8_t key_scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_pin,uint8_t fn);
void delay(uint32_t time);

#endif /*_NKEY_H*/
