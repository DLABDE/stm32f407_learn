#ifndef _IIC_H_
#define _IIC_H_

#include "stm32f4xx.h"
#include <inttypes.h>


#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */


/* ����I2C�������ӵ�GPIO�˿� */
#define I2C_GPIO_PORT				GPIOB			/* GPIO�˿� */
#define I2C_GPIO_CLK			 	RCC_AHB1Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define I2C_SCL_PIN					GPIO_Pin_8			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN					GPIO_Pin_9			/* ���ӵ�SDA�����ߵ�GPIO */


/* �����дSCL��SDA�ĺ� */
#if 1	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define I2C_SCL_1()  GPIO_SetBits(I2C_GPIO_PORT, I2C_SCL_PIN)		/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_ResetBits(I2C_GPIO_PORT, I2C_SCL_PIN)		/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_SetBits(I2C_GPIO_PORT, I2C_SDA_PIN)		/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_ResetBits(I2C_GPIO_PORT, I2C_SDA_PIN)		/* SDA = 0 */
	
	#define I2C_SDA_READ()  GPIO_ReadInputDataBit(I2C_GPIO_PORT, I2C_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define I2C_SCL_1()  I2C_GPIO_PORT->BSRRL = I2C_SCL_PIN				/* SCL = 1 */
	#define I2C_SCL_0()  I2C_GPIO_PORT->BSRRH = I2C_SCL_PIN				/* SCL = 0 */
	
	#define I2C_SDA_1()  I2C_GPIO_PORT->BSRRL = I2C_SDA_PIN				/* SDA = 1 */
	#define I2C_SDA_0()  I2C_GPIO_PORT->BSRRH = I2C_SDA_PIN				/* SDA = 0 */
	
	#define I2C_SDA_READ()  ((I2C_GPIO_PORT->IDR & I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#endif


void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);

#endif /*_IIC_H_*/
