#include "led.h"

void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
        
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_1;			 
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   GPIOB->ODR |= GPIO_Pin_15;
   RS485_EN=0;//����ģʽ
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
    GPIOB->ODR ^= GPIO_Pin_15;
}

void Control485_Init(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;
    
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			 
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   PLC_EN=0;//����ģʽ
}


