#include "dma.h"

/* ����DMA����Ŀ��洢�������� */
uint8_t SendBuff[SENDBUFF_SIZE];

/* ��ʼ��DMA */
void DMA_Config(void)
{
  DMA_InitTypeDef  DMA_InitStructure;
  __IO uint32_t    Timeout = TIMEOUT_MAX;
    
  RCC_AHB1PeriphClockCmd(DMA_STREAM_CLOCK, ENABLE);//ʹ��DMAʱ��
  DMA_DeInit(DMA_STREAM);//��λ��ʼ��DMA������
  while (DMA_GetCmdStatus(DMA_STREAM) != DISABLE);//ȷ��DMA��������λ���
  
  DMA_InitStructure.DMA_Channel = DMA_CHANNEL;  // DMA������ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = (USART1_BASE+0x04);//�������ݵ�ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)SendBuff;//�ڴ��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize =SENDBUFF_SIZE;//����������Ŀ
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//ʹ�������ַ������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//ʹ���ڴ��Զ���������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݵ�λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�ڴ����ݵ�λ
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//һ�δ���
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;//���������ȼ�Ϊ��
  DMA_Init(DMA_STREAM, &DMA_InitStructure);
  DMA_Cmd(DMA_STREAM, ENABLE);//ʹ��DMA����������ʼDMA���ݴ���

  /* ���DMA�������Ƿ���Ч�����г�ʱ��⹦��,���ENʹ��λ */
  Timeout = TIMEOUT_MAX;
  while ((DMA_GetCmdStatus(DMA_STREAM) != ENABLE) && (Timeout-- > 0));
  if (Timeout == 0)
  {
    while (1)
    {      
			GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_RESET);
    }
  } 
}
