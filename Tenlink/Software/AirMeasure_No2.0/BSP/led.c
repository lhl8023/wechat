 /********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
 * 文件名  ：led.c
 * 描述    ：提供STM32F107VC神舟IV号开发板的库函数
 * 实验平台：STM32神舟IV号开发板
 * 作者    ：www.armjishu.com 
 * 修改日期：2014/03
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
  * @函数名 SZ_STM32_LED1Toggle
  * @功能   将对应的LED指示灯状态取反
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_LED1Toggle(void)
{
    /* 指定管脚输出异或1，实现对应的LED指示灯状态取反目的 */
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


