#include "clock.h"
#include "stm32f4xx.h"
//可以试试超频
#define PLL_M      8		//8M晶振
#define PLL_Q      7
#define PLL_N      336	//192<= N <=432
#define PLL_P      2

#define PLL_M_HSI  16


/* 依据官方寄存器操作编写 */
void User_SetSysClock(void)//by HSE
{
	RCC_DeInit();//这里未注释掉SystemInit,所以先复位RCC
	
	__IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

	//判断HSE是否就绪
  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }

	
	/*HSE已就绪*/
  if (HSEStatus == (uint32_t)0x01)
  {
    /* Select regulator voltage output Scale 1 mode 
				选则电压调节器输出模式为1		*/
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
		
		/* 配置主线的分频因子 */
    /* HCLK = SYSCLK / 1*/
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
		
    /* Configure the main PLL */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);
										//锁相环来源用HSE
		
    /* Enable the main PLL 
			 使能PLL*/
    RCC->CR |= RCC_CR_PLLON;
    /* Wait till the main PLL is ready 
			 等待PLL稳定*/
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
   
		
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state 
			 配置FLASH预取址，指令缓存，数据缓存，等待周期*/
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

    /* Select the main PLL as system clock source 
			 选择主PLL时钟作为系统时钟*/
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source 
			 等待主PLL时钟选为系统时钟*/
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
}


/* 固件库编写HSE */
void HSE_SetSysClock(void)//by HSE
{
	ErrorStatus HSE_ErrorStatu=ERROR;//默认Error
	
	RCC_DeInit();//这里未注释掉SystemInit,所以先复位RCC

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
	HSE_ErrorStatu=RCC_WaitForHSEStartUp();//获取HSE错误返回值
	if(HSE_ErrorStatu==SUCCESS)//已就绪
	{
		/* 选则电压调节器输出模式为1		*/
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
		
		/* 配置主线的分频因子 */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div4);
    RCC_PCLK2Config(RCC_HCLK_Div2);
		
		/* Configure the main PLL */
		RCC_PLLConfig(RCC_PLLSource_HSE,PLL_M, PLL_N, PLL_P, PLL_Q);
		//							时钟来源HSE		
		
		/* PLL使能 */
		RCC_PLLCmd(ENABLE);//开始
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)//等待初始化
		{
		}
		
		/* 配置FLASH预取址，指令缓存，数据缓存，等待周期*/
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

    /* Select the main PLL as system clock source 
			 选择主PLL时钟作为系统时钟*/
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* 等待主PLL时钟选为系统时钟*/
    while(RCC_GetSYSCLKSource()!=0x08)//0x08: PLL used as system clock 
		{
		}
	}
	else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
}


/* 固件库编写HSI */
void HSI_SetSysClock(void)//by HSE
{
	volatile uint32_t HSIStatu=0;//默认未初始化
	
	RCC_DeInit();//这里未注释掉SystemInit,所以先复位RCC

  /* Enable HSI */
  RCC_HSICmd(ENABLE);
	HSIStatu=RCC->CR & RCC_CR_HSIRDY;//获取HSI初始化返回值
	
	if(HSIStatu==RCC_CR_HSIRDY)//已就绪
	{
		/* 选则电压调节器输出模式为1		*/
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
		
		/* 配置主线的分频因子 */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div4);
    RCC_PCLK2Config(RCC_HCLK_Div2);
		
		/* Configure the main PLL */
		RCC_PLLConfig(RCC_PLLSource_HSI,PLL_M_HSI, PLL_N, PLL_P, PLL_Q);
		//							时钟来源HSI
		
		/* PLL使能 */
		RCC_PLLCmd(ENABLE);//开始
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)//等待初始化
		{
		}
		
		/* 配置FLASH预取址，指令缓存，数据缓存，等待周期*/
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

    /* Select the main PLL as system clock source 
			 选择主PLL时钟作为系统时钟*/
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* 等待主PLL时钟选为系统时钟*/
    while(RCC_GetSYSCLKSource()!=0x08)//0x08: PLL used as system clock 
		{
		}
	}
	else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
}
