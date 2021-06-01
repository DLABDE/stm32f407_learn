#include "dma.h"

//m to m 只能DMA2
#define DMA_STREAM               DMA2_Stream0//0数据流
#define DMA_CHANNEL              DMA_Channel_0//0通道
#define DMA_STREAM_CLOCK         RCC_AHB1Periph_DMA2 //挂在在AHB1
#define DMA_FLAG_TCIF            DMA_FLAG_TCIF0

#define BUFFER_SIZE              32
#define TIMEOUT_MAX              10000 /* Maximum timeout value */

/* 定义DMA传输数据源
  const关键字将其定义为常量，会放入flash（只读） 4*8=32位*/
const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};
/* 定义DMA传输目标存储器，数组,sram */
uint32_t aDST_Buffer[BUFFER_SIZE];
                                    
void delay(uint32_t time)
{
	for(;time>0;time--);
}

/* 初始化DMA */
void DMA_Config(void)
{
  DMA_InitTypeDef  DMA_InitStructure;
  __IO uint32_t    Timeout = TIMEOUT_MAX;
    
  RCC_AHB1PeriphClockCmd(DMA_STREAM_CLOCK, ENABLE);//使能DMA时钟
  DMA_DeInit(DMA_STREAM);//复位初始化DMA数据流
  while (DMA_GetCmdStatus(DMA_STREAM) != DISABLE);//确保DMA数据流复位完成
  
  DMA_InitStructure.DMA_Channel = DMA_CHANNEL;  // DMA数据流通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)aSRC_Const_Buffer;//源数据地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)aDST_Buffer;//目标地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;//存储器到存储器模式
  DMA_InitStructure.DMA_BufferSize = (uint32_t)BUFFER_SIZE;//数据数目
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;//使能flash地址自动递增功能
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//使能sram地址自动递增功能
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//源数据字大小(32位)
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//目标数据字大小(32位)
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//一次传输模式,m to m 只能一次
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//数据流优先级为高
	//FIFO
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;//m to m 自动使用FIFO，软件不可控制    
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//阈值,满
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发模式,单次模式
	
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发模式,单次模式
  DMA_Init(DMA_STREAM, &DMA_InitStructure);
  //DMA_ClearFlag(DMA_STREAM,DMA_FLAG_TCIF);//清除DMA数据流传输完成标志位
  DMA_Cmd(DMA_STREAM, ENABLE);//使能DMA数据流，开始DMA数据传输

  /* 检测DMA数据流是否有效并带有超时检测功能,检测EN使能位 */
  Timeout = TIMEOUT_MAX;
  while ((DMA_GetCmdStatus(DMA_STREAM) != ENABLE) && (Timeout-- > 0));
  if (Timeout == 0)//超时则提示
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
  * 判断指定长度的两个数据源是否完全相等，
  * 如果完全相等返回1，只要其中一对数据不相等返回0
  */
uint8_t Buffercmp(const uint32_t* pBuffer, 
                  uint32_t* pBuffer1, uint16_t BufferLength)
{
  /* 数据长度递减 */
  while(BufferLength--)
  {
    /* 判断两个数据源是否对应相等 */
    if(*pBuffer != *pBuffer1)
    {
      /* 对应数据源不相等马上退出函数，并返回0 */
      return 0;
    }
    /* 递增两个数据源的地址指针 */
    pBuffer++;
    pBuffer1++;
  }
  /* 完成判断并且对应数据相对 */
  return 1;  
}

uint8_t konwyes(void)
{
  while(DMA_GetFlagStatus(DMA_STREAM,DMA_FLAG_TCIF)==DISABLE);//等待DMA传输完成
  if(Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE)==0)//比较源数据与传输后数据,判断
  {
    return 1;//不同
  }
  else
  { 
    return 0;//相同
  }
}

