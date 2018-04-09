#ifndef __TIMER_H__
#define __TIMER_H__
#include "stm32f10x.h"
#include <stdio.h>
void delay(__IO uint32_t nCount);

void TIM3_Init(void);
void NVIC_TIM3Configuration(void);
void TIM4_Init(void);
void NVIC_TIM4Configuration(void);
void TIM2_Init(void);
void NVIC_TIM2Configuration(void);

#endif
