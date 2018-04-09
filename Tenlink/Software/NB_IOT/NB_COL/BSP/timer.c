
#include "timer.h"
/*有参数延时函数*/
void delay(__IO uint32_t nCount)
{
    for (; nCount != 0; nCount--);
}
/**-------------------------------------------------------
  * @函数名 NVIC_TIM5Configuration
  * @功能   配置TIM5参数函数，每5/9600秒计数器中断一次，主时钟24M   
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void TIM3_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* 自动重装载寄存器周期的值(计数值) */ 
    TIM_TimeBaseStructure.TIM_Period = (100 - 1);

	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
    /* 这个就是预分频系数*/
    TIM_TimeBaseStructure.TIM_Prescaler = (125 - 1);

    /* 采样分频 */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;

    /* 向上计数模式 */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    /* 初始化定时器5 */
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* 清除溢出中断标志 */
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    /* 溢出中断使能 */
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    /* 计数器使能 */
    TIM_Cmd(TIM3, ENABLE); 
}

/**-------------------------------------------------------
  * @函数名 NVIC_TIM3Configuration
  * @功能   配置TIM5中断向量参数函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void NVIC_TIM3Configuration(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM5 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

/**-------------------------------------------------------
  * @函数名 NVIC_TIM4Configuration
  * @功能   配置TIM4参数函数，每5/9600秒计数器中断一次，主时钟24M，实时性不强，可以放宽到原来的10倍，每50/9600秒计数器中断一次 
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void TIM4_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* 自动重装载寄存器周期的值(计数值) */ 
    //TIM_TimeBaseStructure.TIM_Period = (100 - 1);
    TIM_TimeBaseStructure.TIM_Period = (100 - 1);

	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
    /* 这个就是预分频系数*/
    TIM_TimeBaseStructure.TIM_Prescaler = (125 - 1);

    /* 采样分频 */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;

    /* 向上计数模式 */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    /* 初始化定时器5 */
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    /* 清除溢出中断标志 */
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

    /* 溢出中断使能 */
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    /* 计数器使能 */
    TIM_Cmd(TIM4, ENABLE); 
}

/**-------------------------------------------------------
  * @函数名 NVIC_TIM4Configuration
  * @功能   配置TIM4中断向量参数函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void NVIC_TIM4Configuration(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM5 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}


/**-------------------------------------------------------
* @函数名 NVIC_TIM4Configuration
* @功能   配置TIM4参数函数，每5/9600秒计数器中断一次，主时钟24M，实时性不强，可以放宽到原来的10倍，每50/9600秒计数器中断一次 
* @参数   无
* @返回值 无
***------------------------------------------------------*/
void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    
    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    /* 自动重装载寄存器周期的值(计数值) */ 
    //TIM_TimeBaseStructure.TIM_Period = (100 - 1);
    TIM_TimeBaseStructure.TIM_Period = (10000 - 1);
    
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    /* 这个就是预分频系数*/
    TIM_TimeBaseStructure.TIM_Prescaler = (2400 - 1);
    
    /* 采样分频 */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    
    /* 向上计数模式 */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    /* 初始化定时器5 */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    /* 清除溢出中断标志 */
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    
    /* 溢出中断使能 */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    /* 计数器使能 */
    TIM_Cmd(TIM2, ENABLE); 
}

/**-------------------------------------------------------
* @函数名 NVIC_TIM4Configuration
* @功能   配置TIM4中断向量参数函数
* @参数   无
* @返回值 无
***------------------------------------------------------*/
void NVIC_TIM2Configuration(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM5 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_InitStructure);
}

void TIM1_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    
    /* TIM5 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_DeInit(TIM1);
    
    /* 自动重装载寄存器周期的值(计数值) */ 
    //TIM_TimeBaseStructure.TIM_Period = (100 - 1);
    TIM_TimeBaseStructure.TIM_Period = (94 - 1);
    
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    /* 这个就是预分频系数*/
    TIM_TimeBaseStructure.TIM_Prescaler = (2 - 1);
    
    /* 采样分频 */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    
    /* 向上计数模式 */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    /* 初始化定时器5 */
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    /* 清除溢出中断标志 */
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    
    /* 溢出中断使能 */
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    
    /* 计数器使能 */
    TIM_Cmd(TIM1, ENABLE);
    
    
}

void NVIC_TIM1Configuration(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM5 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
    TIM_ClearFlag(TIM1, TIM_FLAG_Update); 
    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);

    TIM_Cmd(TIM1,ENABLE);
}