#include "adc.h"
#include "stm32f4xx.h"

static void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		
	// ���� IO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
}

static void ADC_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);// ����ADCʱ��

  // ADC Common �ṹ�� ���� ��ʼ��
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;// ����ADCģʽ
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;// ʱ��Ϊfpclk x��Ƶ	
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;// ��ֹDMAֱ�ӷ���ģʽ	
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;// ����ʱ����	
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  //ADC Init �ṹ�� ���� ��ʼ��
	ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;// ADC �ֱ���
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; // ��ֹɨ��ģʽ����ͨ���ɼ�����Ҫ	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // ����ת��	
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ�ⲿ���ش���
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//�ⲿ����ͨ��������ʹ��������������⣩
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�����Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//ת��ͨ�� 1��                        
  ADC_Init(ADC1, &ADC_InitStructure);
 
  // ���� ADC ͨ��ת��˳��Ϊ1������ʱ��Ϊ3��ʱ������
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_56Cycles);
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);// ADC ת�����������ж�
  ADC_Cmd(ADC1, ENABLE);// ʹ��ADC
  ADC_SoftwareStartConv(ADC1);//��ʼadcת�����������
}

static void ADC_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
  NVIC_Init(&NVIC_InitStructure);
}

void THEADC_Init(void)
{
	ADC_GPIO_Config();
	ADC_Mode_Config();
	ADC_NVIC_Config();
}
