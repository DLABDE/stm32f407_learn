#include "clock.h"
#include "stm32f4xx.h"
//�������Գ�Ƶ
#define PLL_M      8		//8M����
#define PLL_Q      7
#define PLL_N      336	//192<= N <=432
#define PLL_P      2

#define PLL_M_HSI  16


/* ���ݹٷ��Ĵ���������д */
void User_SetSysClock(void)//by HSE
{
	RCC_DeInit();//����δע�͵�SystemInit,�����ȸ�λRCC
	
	__IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

	//�ж�HSE�Ƿ����
  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }

	
	/*HSE�Ѿ���*/
  if (HSEStatus == (uint32_t)0x01)
  {
    /* Select regulator voltage output Scale 1 mode 
				ѡ���ѹ���������ģʽΪ1		*/
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
		
		/* �������ߵķ�Ƶ���� */
    /* HCLK = SYSCLK / 1*/
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
		
    /* Configure the main PLL */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);
										//���໷��Դ��HSE
		
    /* Enable the main PLL 
			 ʹ��PLL*/
    RCC->CR |= RCC_CR_PLLON;
    /* Wait till the main PLL is ready 
			 �ȴ�PLL�ȶ�*/
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
   
		
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state 
			 ����FLASHԤȡַ��ָ��棬���ݻ��棬�ȴ�����*/
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

    /* Select the main PLL as system clock source 
			 ѡ����PLLʱ����Ϊϵͳʱ��*/
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source 
			 �ȴ���PLLʱ��ѡΪϵͳʱ��*/
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
}


/* �̼����дHSE */
void HSE_SetSysClock(void)//by HSE
{
	ErrorStatus HSE_ErrorStatu=ERROR;//Ĭ��Error
	
	RCC_DeInit();//����δע�͵�SystemInit,�����ȸ�λRCC

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
	HSE_ErrorStatu=RCC_WaitForHSEStartUp();//��ȡHSE���󷵻�ֵ
	if(HSE_ErrorStatu==SUCCESS)//�Ѿ���
	{
		/* ѡ���ѹ���������ģʽΪ1		*/
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
		
		/* �������ߵķ�Ƶ���� */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div4);
    RCC_PCLK2Config(RCC_HCLK_Div2);
		
		/* Configure the main PLL */
		RCC_PLLConfig(RCC_PLLSource_HSE,PLL_M, PLL_N, PLL_P, PLL_Q);
		//							ʱ����ԴHSE		
		
		/* PLLʹ�� */
		RCC_PLLCmd(ENABLE);//��ʼ
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)//�ȴ���ʼ��
		{
		}
		
		/* ����FLASHԤȡַ��ָ��棬���ݻ��棬�ȴ�����*/
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

    /* Select the main PLL as system clock source 
			 ѡ����PLLʱ����Ϊϵͳʱ��*/
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* �ȴ���PLLʱ��ѡΪϵͳʱ��*/
    while(RCC_GetSYSCLKSource()!=0x08)//0x08: PLL used as system clock 
		{
		}
	}
	else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
}


/* �̼����дHSI */
void HSI_SetSysClock(void)//by HSE
{
	volatile uint32_t HSIStatu=0;//Ĭ��δ��ʼ��
	
	RCC_DeInit();//����δע�͵�SystemInit,�����ȸ�λRCC

  /* Enable HSI */
  RCC_HSICmd(ENABLE);
	HSIStatu=RCC->CR & RCC_CR_HSIRDY;//��ȡHSI��ʼ������ֵ
	
	if(HSIStatu==RCC_CR_HSIRDY)//�Ѿ���
	{
		/* ѡ���ѹ���������ģʽΪ1		*/
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
		
		/* �������ߵķ�Ƶ���� */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div4);
    RCC_PCLK2Config(RCC_HCLK_Div2);
		
		/* Configure the main PLL */
		RCC_PLLConfig(RCC_PLLSource_HSI,PLL_M_HSI, PLL_N, PLL_P, PLL_Q);
		//							ʱ����ԴHSI
		
		/* PLLʹ�� */
		RCC_PLLCmd(ENABLE);//��ʼ
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)//�ȴ���ʼ��
		{
		}
		
		/* ����FLASHԤȡַ��ָ��棬���ݻ��棬�ȴ�����*/
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

    /* Select the main PLL as system clock source 
			 ѡ����PLLʱ����Ϊϵͳʱ��*/
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* �ȴ���PLLʱ��ѡΪϵͳʱ��*/
    while(RCC_GetSYSCLKSource()!=0x08)//0x08: PLL used as system clock 
		{
		}
	}
	else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
}
