 /********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
 * �ļ���  ��led.c
 * ����    ���ṩSTM32F107VC����IV�ſ�����Ŀ⺯��
 * ʵ��ƽ̨��STM32����IV�ſ�����
 * ����    ��www.armjishu.com 
 * �޸����ڣ�2014/03
**********************************************************************************/
#include "led.h"

void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
   /*
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_11;			 
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;			 
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   GPIOA->ODR |= GPIO_Pin_11;
   GPIOB->ODR |= GPIO_Pin_15;
   */
    
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			 
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   
}



/**-------------------------------------------------------
  * @������ SZ_STM32_LED1Toggle
  * @����   ����Ӧ��LEDָʾ��״̬ȡ��
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_LED1Toggle(void)
{
    /* ָ���ܽ�������1��ʵ�ֶ�Ӧ��LEDָʾ��״̬ȡ��Ŀ�� */
    GPIOC->ODR ^= GPIO_Pin_13;
}

void Control485_Init(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;
    
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			 
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
}


