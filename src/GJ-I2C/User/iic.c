#include "iic.h"

static void i2c_CfgGpio(void);


//I2C总线位延迟，最快400KHz
static void i2c_Delay(void)
{
	uint8_t i;
	/*　
		可用逻辑分析仪测量I2C通讯时的频率
    工作条件：CPU主频168MHz ，MDK编译环境，1级优化
		经测试，循环次数为20~250时都能通讯正常
	*/
	for (i = 0; i < 40; i++);
}


//启动信号
void i2c_Start(void)
{
	//当SCL高电平时，SDA出现一个下跳沿
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

//停止信号
void i2c_Stop(void)
{
	//当SCL高电平时，SDA出现一个上跳沿
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}

//主机发送8bit数据
//_ucByte ： 等待发送的字节
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	//先发送字节的高位bit7
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();	
		I2C_SCL_0();
		if (i == 7)
		{
			 I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	//左移一个bit
		i2c_Delay();
	}
}

//I2C总线设备读取8bit数据
//返回读到的数据
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	//读到第1个bit为数据的bit7
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}

//读取器件的ACK应答信号
//返回0表示正确应答，1表示无器件响应
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPU释放SDA总线 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	i2c_Delay();
	return re;
}

//CPU产生一个ACK
void i2c_Ack(void)
{
	I2C_SDA_0();	//SDA = 0
	i2c_Delay();
	I2C_SCL_1();	//CPU产生1个时钟
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	//放SDA总线
}


//CPU产生1个NACK信号
void i2c_NAck(void)
{
	I2C_SDA_1();	//SDA = 1
	i2c_Delay();
	I2C_SCL_1();	//CPU产生1个时钟
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();	
}

//配置I2C总线的GPIO，采用模拟IO的方式实现
static void i2c_CfgGpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(I2C_GPIO_CLK, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  	//开漏输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStructure);

	//停止信号, 复位I2C总线上设备到待机模式
	i2c_Stop();
}


//检测设备，CPU发送设备地址，取设备应答来判断
//_Address：设备的I2C总线地址
//返回值 0 表示正确， 返回1表示未探测到
uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	i2c_CfgGpio();

	i2c_Start();		
	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(_Address | I2C_WR);
	ucAck = i2c_WaitAck();	//检测设备的ACK应答
	i2c_Stop();	

	return ucAck;
}
