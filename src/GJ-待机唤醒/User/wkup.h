#ifndef _WKUP_H
#define _WKUP_H	 

#include "stm32f4xx.h"

#define WKUP_KD PAin(0)			//PA0 检测是否外部WK_UP按键按下
	 
u8 Check_WKUP(void);  			//检测WKUP脚的信号
void WKUP_Init(void); 			//PA0 WKUP唤醒初始化
void Sys_Enter_Standby(void);	//系统进入待机模式

#endif /* _WKUP_H */
