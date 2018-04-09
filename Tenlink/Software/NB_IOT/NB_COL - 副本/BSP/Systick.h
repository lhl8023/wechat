#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "stm32f10x.h"

void TimingDelay_Decrement(void);
void SZ_STM32_SysTickInit(uint32_t HzPreSecond);
//void Delay_us(__IO u32 nTime);
void Delay_ms(__IO u32 nTime);
void Delay_s(__IO u32 nTime);

#endif 



