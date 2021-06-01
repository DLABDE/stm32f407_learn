#include "usart.h"
#include <stdio.h>

///* �ж����� */
//static void nvicinit(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//�ж�Դ
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//}

/* 1,gpio��ʼ�� */
void gpiousart()
{
	/* �ṹ���ʼ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef gpio_init;

	gpio_init.GPIO_Speed=GPIO_Fast_Speed;
	gpio_init.GPIO_PuPd=GPIO_PuPd_UP;
	gpio_init.GPIO_Mode=GPIO_Mode_AF;//����ģʽ
	gpio_init.GPIO_OType=GPIO_OType_PP;//�������
	
	gpio_init.GPIO_Pin=GPIO_Pin_9;//TX
	GPIO_Init(GPIOA,&gpio_init);
	gpio_init.GPIO_Pin=GPIO_Pin_10;//RX
	GPIO_Init(GPIOA,&gpio_init);
	
	/* gpio���� */
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
}

/* 2,���ô��� */
void usartinit()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//��ʼ���ṹ��
  USART_InitTypeDef USART_InitStructure;
		
  USART_InitStructure.USART_BaudRate = 115200;//������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;//��У��
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����Ӳ����
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//usartģʽRX+TX
  USART_Init(USART1, &USART_InitStructure); //��ʼ��
	
//	nvicinit();
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ʹ�ܴ��ڽ����ж�
  USART_Cmd(USART1, ENABLE);//ʹ�ܴ���
}


/*����һ���ַ�*/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	USART_SendData(pUSARTx,ch);//����+����
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == 0);//�ȴ��������ݼĴ���Ϊ��
}

/*�����ַ���*/
void Usart_string( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==0);
}

/*����һ��16λ��*/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == 0);
	
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == 0);	
}


///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		//����һ���ֽ����ݵ�����
		USART_SendData(USART1, (uint8_t) ch);
		
		//�ȴ��������
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		//�ȴ�������������
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
