#include "dma.h"

/* 定义DMA传输目标存储器，数组 */
uint8_t SendBuff[SENDBUFF_SIZE];

/* 初始化DMA */
void DMA_Config(void)
{
  DMA_InitTypeDef  DMA_InitStructure;
  __IO uint32_t    Timeout = TIMEOUT_MAX;
    
  RCC_AHB1PeriphClockCmd(DMA_STREAM_CLOCK, ENABLE);//使能DMA时钟
  DMA_DeInit(DMA_STREAM);//复位初始化DMA数据流
  while (DMA_GetCmdStatus(DMA_STREAM) != DISABLE);//确保DMA数据流复位完成
  
  DMA_InitStructure.DMA_Channel = DMA_CHANNEL;  // DMA数据流通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = (USART1_BASE+0x04);//外设数据地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)SendBuff;//内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
  DMA_InitStructure.DMA_BufferSize =SENDBUFF_SIZE;//传输数据数目
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//使能外设地址不递增
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//使能内存自动递增功能
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据单位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//内存数据单位
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//一次传输
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;//数据流优先级为低
  DMA_Init(DMA_STREAM, &DMA_InitStructure);
  DMA_Cmd(DMA_STREAM, ENABLE);//使能DMA数据流，开始DMA数据传输

  /* 检测DMA数据流是否有效并带有超时检测功能,检测EN使能位 */
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
