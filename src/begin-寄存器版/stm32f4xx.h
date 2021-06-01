/*对寄存器映射的部分封装*/
#ifndef _STM32F4XX_H
#define _STM32F4XX_H

typedef unsigned int uint32;
typedef unsigned short int uint16;

/*内部总线的地址*/
#define AHB1 (0x40020000)

/*总线上外设的地址*/
//AHB1高速总线外设
#define RCC_BASE (AHB1+0x3800)//RCC时钟控制地址

#define GPIOA_BASE (AHB1+0x00)//GPIOX_BASE地址
#define GPIOB_BASE (AHB1+0x400)


/*RCC寄存器*/
#define RCC_AHB1ENR (*( uint32 *)(RCC_BASE+0x30))//AHB1外设时钟使能寄存器,偏移地址0x30

/*GPIO寄存器*/
typedef struct{
	uint32 MODER;//
	uint32 OTYPER;
	uint32 OSPEEDR;
	uint32 PUPDR;
	uint32 IDR;
	uint32 ODR;
	uint16 BSRRS;
	uint16 BSRRR;
	uint32 LCKR;
	uint32 AFRL;
	uint32 AFRH;
}GPIO_TypeDef;
#define GPIOA ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef *)GPIOB_BASE)


#endif /*_STM32F4XX_H*/
