/*�ԼĴ���ӳ��Ĳ��ַ�װ*/
#ifndef _STM32F4XX_H
#define _STM32F4XX_H

typedef unsigned int uint32;
typedef unsigned short int uint16;

/*�ڲ����ߵĵ�ַ*/
#define AHB1 (0x40020000)

/*����������ĵ�ַ*/
//AHB1������������
#define RCC_BASE (AHB1+0x3800)//RCCʱ�ӿ��Ƶ�ַ

#define GPIOA_BASE (AHB1+0x00)//GPIOX_BASE��ַ
#define GPIOB_BASE (AHB1+0x400)


/*RCC�Ĵ���*/
#define RCC_AHB1ENR (*( uint32 *)(RCC_BASE+0x30))//AHB1����ʱ��ʹ�ܼĴ���,ƫ�Ƶ�ַ0x30

/*GPIO�Ĵ���*/
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
