#ifndef _IIC_H_
#define _IIC_H_

#include "stm32f4xx.h"
#include <inttypes.h>


#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */


/* 定义I2C总线连接的GPIO端口 */
#define I2C_GPIO_PORT				GPIOB			/* GPIO端口 */
#define I2C_GPIO_CLK			 	RCC_AHB1Periph_GPIOB		/* GPIO端口时钟 */
#define I2C_SCL_PIN					GPIO_Pin_8			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN					GPIO_Pin_9			/* 连接到SDA数据线的GPIO */


/* 定义读写SCL和SDA的宏 */
#if 1	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
	#define I2C_SCL_1()  GPIO_SetBits(I2C_GPIO_PORT, I2C_SCL_PIN)		/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_ResetBits(I2C_GPIO_PORT, I2C_SCL_PIN)		/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_SetBits(I2C_GPIO_PORT, I2C_SDA_PIN)		/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_ResetBits(I2C_GPIO_PORT, I2C_SDA_PIN)		/* SDA = 0 */
	
	#define I2C_SDA_READ()  GPIO_ReadInputDataBit(I2C_GPIO_PORT, I2C_SDA_PIN)	/* 读SDA口线状态 */
#else	/* 这个分支选择直接寄存器操作实现IO读写 */
    /*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
	#define I2C_SCL_1()  I2C_GPIO_PORT->BSRRL = I2C_SCL_PIN				/* SCL = 1 */
	#define I2C_SCL_0()  I2C_GPIO_PORT->BSRRH = I2C_SCL_PIN				/* SCL = 0 */
	
	#define I2C_SDA_1()  I2C_GPIO_PORT->BSRRL = I2C_SDA_PIN				/* SDA = 1 */
	#define I2C_SDA_0()  I2C_GPIO_PORT->BSRRH = I2C_SDA_PIN				/* SDA = 0 */
	
	#define I2C_SDA_READ()  ((I2C_GPIO_PORT->IDR & I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
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
