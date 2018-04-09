
#include "timer.h"
/*�в�����ʱ����*/
void delay(__IO uint32_t nCount)
{
    for (; nCount != 0; nCount--);
}
/**-------------------------------------------------------
  * @������ NVIC_TIM5Configuration
  * @����   ����TIM5����������ÿ5/9600��������ж�һ�Σ���ʱ��24M   
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void TIM3_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */ 
    TIM_TimeBaseStructure.TIM_Period = (100 - 1);

	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    /* �������Ԥ��Ƶϵ��*/
    TIM_TimeBaseStructure.TIM_Prescaler = (125 - 1);

    /* ������Ƶ */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;

    /* ���ϼ���ģʽ */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    /* ��ʼ����ʱ��5 */
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* �������жϱ�־ */
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    /* ����ж�ʹ�� */
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    /* ������ʹ�� */
    TIM_Cmd(TIM3, ENABLE); 
}

/**-------------------------------------------------------
  * @������ NVIC_TIM3Configuration
  * @����   ����TIM5�ж�������������
  * @����   ��
  * @����ֵ ��
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
  * @������ NVIC_TIM4Configuration
  * @����   ����TIM4����������ÿ5/9600��������ж�һ�Σ���ʱ��24M��ʵʱ�Բ�ǿ�����Էſ�ԭ����10����ÿ50/9600��������ж�һ�� 
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void TIM4_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */ 
    //TIM_TimeBaseStructure.TIM_Period = (100 - 1);
    TIM_TimeBaseStructure.TIM_Period = (100 - 1);

	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    /* �������Ԥ��Ƶϵ��*/
    TIM_TimeBaseStructure.TIM_Prescaler = (125 - 1);

    /* ������Ƶ */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;

    /* ���ϼ���ģʽ */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    /* ��ʼ����ʱ��5 */
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    /* �������жϱ�־ */
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

    /* ����ж�ʹ�� */
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    /* ������ʹ�� */
    TIM_Cmd(TIM4, ENABLE); 
}

/**-------------------------------------------------------
  * @������ NVIC_TIM4Configuration
  * @����   ����TIM4�ж�������������
  * @����   ��
  * @����ֵ ��
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
* @������ NVIC_TIM4Configuration
* @����   ����TIM4����������ÿ5/9600��������ж�һ�Σ���ʱ��24M��ʵʱ�Բ�ǿ�����Էſ�ԭ����10����ÿ50/9600��������ж�һ�� 
* @����   ��
* @����ֵ ��
***------------------------------------------------------*/
void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    
    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */ 
    //TIM_TimeBaseStructure.TIM_Period = (100 - 1);
    TIM_TimeBaseStructure.TIM_Period = (10000 - 1);
    
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    /* �������Ԥ��Ƶϵ��*/
    TIM_TimeBaseStructure.TIM_Prescaler = (2400 - 1);
    
    /* ������Ƶ */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    
    /* ���ϼ���ģʽ */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    /* ��ʼ����ʱ��5 */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    /* �������жϱ�־ */
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    
    /* ����ж�ʹ�� */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    /* ������ʹ�� */
    TIM_Cmd(TIM2, ENABLE); 
}

/**-------------------------------------------------------
* @������ NVIC_TIM4Configuration
* @����   ����TIM4�ж�������������
* @����   ��
* @����ֵ ��
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
    
    /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */ 
    //TIM_TimeBaseStructure.TIM_Period = (100 - 1);
    TIM_TimeBaseStructure.TIM_Period = (94 - 1);
    
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    /* �������Ԥ��Ƶϵ��*/
    TIM_TimeBaseStructure.TIM_Prescaler = (2 - 1);
    
    /* ������Ƶ */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    
    /* ���ϼ���ģʽ */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    /* ��ʼ����ʱ��5 */
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    /* �������жϱ�־ */
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    
    /* ����ж�ʹ�� */
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    
    /* ������ʹ�� */
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