#include "dma.h"

//m to m ֻ��DMA2
#define DMA_STREAM               DMA2_Stream0//0������
#define DMA_CHANNEL              DMA_Channel_0//0ͨ��
#define DMA_STREAM_CLOCK         RCC_AHB1Periph_DMA2 //������AHB1
#define DMA_FLAG_TCIF            DMA_FLAG_TCIF0

#define BUFFER_SIZE              32
#define TIMEOUT_MAX              10000 /* Maximum timeout value */

/* ����DMA��������Դ
  const�ؼ��ֽ��䶨��Ϊ�����������flash��ֻ���� 4*8=32λ*/
const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};
/* ����DMA����Ŀ��洢��������,sram */
uint32_t aDST_Buffer[BUFFER_SIZE];
                                    
void delay(uint32_t time)
{
	for(;time>0;time--);
}

/* ��ʼ��DMA */
void DMA_Config(void)
{
  DMA_InitTypeDef  DMA_InitStructure;
  __IO uint32_t    Timeout = TIMEOUT_MAX;
    
  RCC_AHB1PeriphClockCmd(DMA_STREAM_CLOCK, ENABLE);//ʹ��DMAʱ��
  DMA_DeInit(DMA_STREAM);//��λ��ʼ��DMA������
  while (DMA_GetCmdStatus(DMA_STREAM) != DISABLE);//ȷ��DMA��������λ���
  
  DMA_InitStructure.DMA_Channel = DMA_CHANNEL;  // DMA������ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)aSRC_Const_Buffer;//Դ���ݵ�ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)aDST_Buffer;//Ŀ���ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;//�洢�����洢��ģʽ
  DMA_InitStructure.DMA_BufferSize = (uint32_t)BUFFER_SIZE;//������Ŀ
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;//ʹ��flash��ַ�Զ���������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//ʹ��sram��ַ�Զ���������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//Դ�����ִ�С(32λ)
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//Ŀ�������ִ�С(32λ)
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//һ�δ���ģʽ,m to m ֻ��һ��
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//���������ȼ�Ϊ��
	//FIFO
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;//m to m �Զ�ʹ��FIFO��������ɿ���    
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//��ֵ,��
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ��ģʽ,����ģʽ
	
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ��ģʽ,����ģʽ
  DMA_Init(DMA_STREAM, &DMA_InitStructure);
  //DMA_ClearFlag(DMA_STREAM,DMA_FLAG_TCIF);//���DMA������������ɱ�־λ
  DMA_Cmd(DMA_STREAM, ENABLE);//ʹ��DMA����������ʼDMA���ݴ���

  /* ���DMA�������Ƿ���Ч�����г�ʱ��⹦��,���ENʹ��λ */
  Timeout = TIMEOUT_MAX;
  while ((DMA_GetCmdStatus(DMA_STREAM) != ENABLE) && (Timeout-- > 0));
  if (Timeout == 0)//��ʱ����ʾ
  {
    while (1)
    {      
			GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);
			delay(0xfffff);
			GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_RESET);
			delay(0xfffff);
    }
  } 
}

/**
  * �ж�ָ�����ȵ���������Դ�Ƿ���ȫ��ȣ�
  * �����ȫ��ȷ���1��ֻҪ����һ�����ݲ���ȷ���0
  */
uint8_t Buffercmp(const uint32_t* pBuffer, 
                  uint32_t* pBuffer1, uint16_t BufferLength)
{
  /* ���ݳ��ȵݼ� */
  while(BufferLength--)
  {
    /* �ж���������Դ�Ƿ��Ӧ��� */
    if(*pBuffer != *pBuffer1)
    {
      /* ��Ӧ����Դ����������˳�������������0 */
      return 0;
    }
    /* ������������Դ�ĵ�ַָ�� */
    pBuffer++;
    pBuffer1++;
  }
  /* ����жϲ��Ҷ�Ӧ������� */
  return 1;  
}

uint8_t konwyes(void)
{
  while(DMA_GetFlagStatus(DMA_STREAM,DMA_FLAG_TCIF)==DISABLE);//�ȴ�DMA�������
  if(Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE)==0)//�Ƚ�Դ�����봫�������,�ж�
  {
    return 1;//��ͬ
  }
  else
  { 
    return 0;//��ͬ
  }
}

