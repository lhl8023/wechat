#ifndef	__USART_H__
#define	__USART_H__

/********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
 * 文件名  ：SZ_STM32F107VC_LIB.h
 * 描述    ：提供STM32F107VC神舟IV号开发板的库函数
 * 实验平台：STM32神舟开发板
 * 作者    ：www.armjishu.com 
**********************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "timer.h"

#define     CTRL_485_INIT_M      Control485_Init()
#define     CTRL_485_TX_M        GPIO_ResetBits(GPIOA,GPIO_Pin_11)
#define     CTRL_485_RX_M        GPIO_SetBits(GPIOA,GPIO_Pin_11)

extern unsigned char ReceiveData[], SendData_Uart1[], SendData_Uart2[], num_ReceiveData, flag_send_uart1, flag_send_uart2;
extern unsigned char Send_Num_data1, SendNum_Over1, Send_Num_data2, SendNum_Over2;
extern unsigned char flag_485_SendDelay, Longth_Uart1Send, Longth_Uart2Send;
void PM2_5_Init();
void SZ_STM32_Uart1Init(unsigned int BaudRate);
void Uart1Putstring(const char *s);
void ReceOneChar_Uart1(unsigned char ReceCharacter);
void NVIC_Uart1Configuration(void);
void Uart1_SendStart(unsigned char* data_send, unsigned char num);
void Uart1_SendStop(void);

void SZ_STM32_Uart2Init(unsigned int BaudRate);
void Uart2Putstring(const char *s);
void ReceOneChar_Uart2(unsigned char ReceCharacter);
void NVIC_Uart2Configuration(void);
void Uart2_SendStart(unsigned char* data_send, unsigned char num);
void Uart2_SendStop(void);

void SZ_STM32_Uart3Init(unsigned int BaudRate);
void NVIC_Uart3Configuration(void);
void NVIC_GroupConfig(void);

#endif
/******************* (C) COPYRIGHT 2013 www.armjishu.com *****END OF FILE****/

