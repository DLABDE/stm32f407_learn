#include "iic.h"

static void i2c_CfgGpio(void);


//I2C����λ�ӳ٣����400KHz
static void i2c_Delay(void)
{
	uint8_t i;
	/*��
		�����߼������ǲ���I2CͨѶʱ��Ƶ��
    ����������CPU��Ƶ168MHz ��MDK���뻷����1���Ż�
		�����ԣ�ѭ������Ϊ20~250ʱ����ͨѶ����
	*/
	for (i = 0; i < 40; i++);
}


//�����ź�
void i2c_Start(void)
{
	//��SCL�ߵ�ƽʱ��SDA����һ��������
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

//ֹͣ�ź�
void i2c_Stop(void)
{
	//��SCL�ߵ�ƽʱ��SDA����һ��������
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}

//��������8bit����
//_ucByte �� �ȴ����͵��ֽ�
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	//�ȷ����ֽڵĸ�λbit7
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
			 I2C_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1;	//����һ��bit
		i2c_Delay();
	}
}

//I2C�����豸��ȡ8bit����
//���ض���������
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	//������1��bitΪ���ݵ�bit7
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

//��ȡ������ACKӦ���ź�
//����0��ʾ��ȷӦ��1��ʾ��������Ӧ
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
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

//CPU����һ��ACK
void i2c_Ack(void)
{
	I2C_SDA_0();	//SDA = 0
	i2c_Delay();
	I2C_SCL_1();	//CPU����1��ʱ��
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	//��SDA����
}


//CPU����1��NACK�ź�
void i2c_NAck(void)
{
	I2C_SDA_1();	//SDA = 1
	i2c_Delay();
	I2C_SCL_1();	//CPU����1��ʱ��
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();	
}

//����I2C���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
static void i2c_CfgGpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(I2C_GPIO_CLK, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  	//��©���
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStructure);

	//ֹͣ�ź�, ��λI2C�������豸������ģʽ
	i2c_Stop();
}


//����豸��CPU�����豸��ַ��ȡ�豸Ӧ�����ж�
//_Address���豸��I2C���ߵ�ַ
//����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	i2c_CfgGpio();

	i2c_Start();		
	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte(_Address | I2C_WR);
	ucAck = i2c_WaitAck();	//����豸��ACKӦ��
	i2c_Stop();	

	return ucAck;
}
