/**
******************************************************************************
* @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
* @author  MCD Application Team
* @version V3.5.0
* @date    08-April-2011
* @brief   Main Interrupt Service Routines.
*          This file provides template for all exceptions handler and 
*          peripherals interrupt service routine.
******************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "led.h"
#include "usart.h"
#include "Systick.h"
#include "Modbus.h"
#include "Protocol.h"

u16 p[16];
int i=0;
int light_level=0,times=1,LED_ON=1,rease=1,finish=0;
extern unsigned int falg_display, count_display;

/** @addtogroup STM32F10x_StdPeriph_Template
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
* @brief  This function handles NMI exception.
* @param  None
* @retval None
*/
void NMI_Handler(void)
{
}

/**
* @brief  This function handles Hard Fault exception.
* @param  None
* @retval None
*/
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
* @brief  This function handles Memory Manage exception.
* @param  None
* @retval None
*/
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
* @brief  This function handles Bus Fault exception.
* @param  None
* @retval None
*/
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
* @brief  This function handles Usage Fault exception.
* @param  None
* @retval None
*/
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
* @brief  This function handles SVCall exception.
* @param  None
* @retval None
*/
void SVC_Handler(void)
{
}

/**
* @brief  This function handles Debug Monitor exception.
* @param  None
* @retval None
*/
void DebugMon_Handler(void)
{
}

/**
* @brief  This function handles PendSVC exception.
* @param  None
* @retval None
*/
void PendSV_Handler(void)
{
}

/**
* @brief  This function handles SysTick Handler.
* @param  None
* @retval None

void SysTick_Handler(void)
{
    TimingDelay_Decrement();
}*/

/**-------------------------------------------------------
* @函数名 EXTI0_IRQHandler
* @功能   处理外部中断线0的中断，按键4中断处理函数
* @参数   无
* @返回值 无
***------------------------------------------------------*/
void EXTI0_IRQHandler(void)  /* Key 4 */
{
    
}

/**-------------------------------------------------------
* @函数名 EXTI4_IRQHandler
* @功能   处理外部中断线4的中断，按键1中断处理函数
* @参数   无
* @返回值 无
***------------------------------------------------------*/
void EXTI4_IRQHandler(void) /* Key 1 */
{
    
}

/**-------------------------------------------------------
* @函数名 EXTI15_10_IRQHandler
* @功能   处理外部中断线10-15的中断，按键2和3中断处理函数
* @参数   无
* @返回值 无
***------------------------------------------------------*/
void EXTI15_10_IRQHandler(void) /* Key 2 & 3 */
{
    
}

void TIM1_UP_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM1,TIM_IT_Update) !=RESET )
  {
    times++;
    if((rease==1)&&(finish==1))
    {
        light_level++;
        if(light_level==100)
        {
          rease=0;
          LED_ON=0;
        }
        finish=0;
    }
    if((rease==0)&&(finish==1))
    {
      light_level--;
      if(light_level==0)
      {
        rease=1;
        LED_ON=1;
      }
      finish=0;
    }
    if(LED_ON==1)
    {
      if(times==(100-light_level))
      {
        GPIO_SetBits(GPIOB,GPIO_Pin_0);
        times=0;

        LED_ON=0;
      }
    }
    if(LED_ON==0)
    {
      if(times==light_level)
      {
        if(light_level<99)
          GPIO_ResetBits(GPIOB,GPIO_Pin_0);
        else
          GPIO_SetBits(GPIOB,GPIO_Pin_0);
        times=0;
                finish=1;

                LED_ON=1;
      }
    }
    
    TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
  }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        if(VOC_modbusdata_master.num15start == 1)
        {
            VOC_modbusdata_master.num15++;
            if(VOC_modbusdata_master.num15 >= VOC_modbusdata_master.num15end)
            {
                VOC_modbusdata_master.num15 = 0;
                VOC_modbusdata_master.num15start = 0;
                VOC_T15EndHandle_Master();
            }
        }
        if(VOC_modbusdata_master.num35start == 1)
        {
            VOC_modbusdata_master.num35++;
            //3.5个字符时间超时
            if(VOC_modbusdata_master.num35 >= VOC_modbusdata_master.num35end)
            {
                VOC_modbusdata_master.num35 = 0;
                VOC_modbusdata_master.num35start = 0;
                //3.5个字符超时处理函数
                VOC_T35EndHandle_Master();
            }
        }
        if(VOC_Start_DeviceNoEcho == 1)
        {
            VOC_num_DeviceNoEcho++;
            if(VOC_num_DeviceNoEcho >= NUM_DEVICE_NOECHO)
            {
                VOC_Start_DeviceNoEcho = 0;
                VOC_num_DeviceNoEcho = 0;
                VOC_flag_DeviceNoEcho = 1;
            }
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);       
    }
}


/**-------------------------------------------------------
* @函数名 TIM3_IRQHandler
* @功能   TIM3中断处理函数，每秒中断一次 
* @参数   无
* @返回值 无
***------------------------------------------------------*/
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        if(PM2_5_modbusdata_master.num15start == 1)
        {
            PM2_5_modbusdata_master.num15++;
            if(PM2_5_modbusdata_master.num15 >= PM2_5_modbusdata_master.num15end)
            {
                PM2_5_modbusdata_master.num15 = 0;
                PM2_5_modbusdata_master.num15start = 0;
                PM2_5_T15EndHandle_Master();
            }
        }
        if(PM2_5_modbusdata_master.num35start == 1)
        {
            PM2_5_modbusdata_master.num35++;
            //3.5个字符时间超时
            if(PM2_5_modbusdata_master.num35 >= PM2_5_modbusdata_master.num35end)
            {
                PM2_5_modbusdata_master.num35 = 0;
                PM2_5_modbusdata_master.num35start = 0;
                //3.5个字符超时处理函数
                PM2_5_T35EndHandle_Master();
            }
        }
        
        if(PM2_5_Start_DeviceNoEcho == 1)
        {
            PM2_5_num_DeviceNoEcho++;
            if(PM2_5_num_DeviceNoEcho >= NUM_DEVICE_NOECHO)
            {
                PM2_5_Start_DeviceNoEcho = 0;
                PM2_5_num_DeviceNoEcho = 0;
                PM2_5_flag_DeviceNoEcho = 1;
            }
        } 
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);       
    }
}

/**-------------------------------------------------------
* @函数名 TIM4_IRQHandler
* @功能   TIM4中断处理函数，每秒中断一次 
* @参数   无
* @返回值 无
***------------------------------------------------------*/
void TIM4_IRQHandler(void)
{
    /* www.armjishu.com ARM技术论坛 */
    
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        AutoSendTime++;
//        if((AutoSendTime%SpyTimeInterval) == 0)
//        {
//            AutoSendTimerOver = 1;
//        }
//        if((AutoSendTime%AutoSendTimeInterval) == 0)
//        {
//            AutoSendFlag = 0;
//        }
        if(AutoSendTime > SpyTimeInterval)
        {
            AutoSendTime = 0;
            AutoSendOverTime++;
            if(AutoSendOverTime>AutoSendTimeInterval)
            {
                AutoSendOverTime = 0;
                AutoSendFlag = 1;
            }
            AutoSendTimerOver = 1;
        }
        
        count_display++;
        if(count_display > DisTimeInterval)
        {
        		count_display = 0;
        		falg_display = 1;
        }
        
        if(modbusdata_slaver.num15start == 1)
        {
            modbusdata_slaver.num15++;
            if(modbusdata_slaver.num15 >= modbusdata_slaver.num15end)
            {
                modbusdata_slaver.num15 = 0;
                modbusdata_slaver.num15start = 0;
                T15EndHandle_Slaver();
            }
        }
        if(modbusdata_slaver.num35start == 1)
        {
            modbusdata_slaver.num35++;
            if(modbusdata_slaver.num35 >= modbusdata_slaver.num35end)
            {
                modbusdata_slaver.num35 = 0;
                modbusdata_slaver.num35start = 0;
                T35EndHandle_Slaver();
            }
        }		
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);       
    }
}

/**-------------------------------------------------------
* @函数名 USART2_IRQHandler
* @功能   串口2中断处理函数
* @参数   无
* @返回值 无
***------------------------------------------------------*/
void USART1_IRQHandler(void)
{
    /* 开始临界资源访问，禁止中断 */
    __disable_irq(); 
    
    //接收中断 
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        //ReceOneChar_Uart1(USART_ReceiveData(USART1));
        PM2_5_ReceOneChar_Master(USART_ReceiveData(USART1));
       /*  if(USART_ReceiveData(USART1)==42) 
        {
          p[i]=USART_ReceiveData(USART1);
          i++;
        }
        clear interrupt */
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }	
    //发送完成中断
    else if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)
    {
        if(Send_Num_data1 >= SendNum_Over1)
        {			  
            Uart1_SendStop();
        }
        else
        {  
            USART1->DR = SendData_Uart1[Send_Num_data1++];
        }
        
        /* clear interrupt */
        USART_ClearITPendingBit(USART1, USART_IT_TC);
    }	
    //溢出错误中断
    else if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
    {
        /* clear interrupt */
        USART_ClearITPendingBit(USART1, USART_FLAG_ORE);
    }	
    
    /* 结束临界资源访问，开放中断 */
    __enable_irq();
}

/**-------------------------------------------------------
* @函数名 USART2_IRQHandler
* @功能   串口2中断处理函数   从机模式
* @参数   无
* @返回值 无
***------------------------------------------------------*/
void USART2_IRQHandler(void)
{
    /* 开始临界资源访问，禁止中断 */
    __disable_irq(); 
    
    //接收中断 
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        //ReceOneChar_Uart2(USART_ReceiveData(USART2));
        ReceOneChar_Slaver(USART_ReceiveData(USART2));
        /* clear interrupt */
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }	
    //发送完成中断
    else if (USART_GetITStatus(USART2, USART_IT_TC) != RESET)
    {
        if(Send_Num_data2 >= SendNum_Over2)
        {			  
            Uart2_SendStop();
        }
        else
        {  
            USART2->DR = SendData_Uart2[Send_Num_data2++];
        }
        /* clear interrupt */
        USART_ClearITPendingBit(USART2, USART_IT_TC);
    }	
    //溢出错误中断
    else if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
    {
        /* clear interrupt */
        USART_ClearITPendingBit(USART2, USART_FLAG_ORE);
    }	
    
    /* 结束临界资源访问，开放中断 */
    __enable_irq();
}

/**-------------------------------------------------------
* @函数名 USART2_IRQHandler
* @功能   串口2中断处理函数
* @参数   无
* @返回值 无
***------------------------------------------------------*/
void USART3_IRQHandler(void)
{
    /* 开始临界资源访问，禁止中断 */
    __disable_irq(); 
    
    //接收中断 
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        //ReceOneChar_Uart1(USART_ReceiveData(USART1));
        VOC_ReceOneChar_Master(USART_ReceiveData(USART3));
        /* clear interrupt */
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }	
    //发送完成中断
    else if (USART_GetITStatus(USART3, USART_IT_TC) != RESET)
    {
        if(Send_Num_data1 >= SendNum_Over1)
        {			  
            Uart1_SendStop();
        }
        else
        {  
            USART1->DR = SendData_Uart1[Send_Num_data1++];
        }
        
        /* clear interrupt */
        USART_ClearITPendingBit(USART3, USART_IT_TC);
    }	
    //溢出错误中断
    else if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
    {
        /* clear interrupt */
        USART_ClearITPendingBit(USART3, USART_FLAG_ORE);
    }	
    
    /* 结束临界资源访问，开放中断 */
    __enable_irq();
}

void ADC1_2_IRQHandler()
{
    if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
    {
        AirData.adc = ADC_GetConversionValue(ADC1) * 3300 / 4096;
        ADC_DataOK = 1;
    }
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
