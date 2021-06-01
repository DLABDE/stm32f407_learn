#include "iwdg.h"

/*
 * ���� IWDG �ĳ�ʱʱ��
 * Tout = prv/40 * rlv (s)
 * rlv:Ԥ��Ƶ��ֵ��ȡֵ��ΧΪ��0-0XFFF
 * IWDG_Config(IWDG_Prescaler_64 ,625);  // IWDG 1s ��ʱ��� 
 *						��64/40��*625 = 1s
 
 Tout=((4*2^prv)*rlv)/32
 */

void IWDG_Config(uint8_t prv ,uint16_t rlv)
{	
	IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable );// ʹ�� Ԥ��Ƶ�Ĵ���PR����װ�ؼĴ���RLR��д��ȡ��д������
	IWDG_SetPrescaler( prv );// ����Ԥ��Ƶ��ֵ
	IWDG_SetReload( rlv );// ������װ�ؼĴ���ֵ
	IWDG_ReloadCounter();// ����װ�ؼĴ�����ֵ�ŵ���������
	IWDG_Enable();	// ʹ�� IWDG
}

void IWDG_Feed(void)// ι��
{
	IWDG_ReloadCounter();
}

/*
if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)//�ж��Ƿ��ſڸ�λ
		{
			RCC_ClearFlag();//�����־
		}
*/
