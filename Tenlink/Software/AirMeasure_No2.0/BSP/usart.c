/********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
* 文件名  ：SZ_STM32F107VC_LIB.c
* 描述    ：提供STM32F107VC神舟IV号开发板的库函数
* 实验平台：STM32神舟开发板
* 作者    ：www.armjishu.com 
**********************************************************************************/
#include "usart.h"
#include "stm32f10x_conf.h"

unsigned char ReceiveData[50], SendData_Uart1[20], SendData_Uart2[20], num_ReceiveData=0, flag_send_uart1=0, flag_send_uart2=0;
unsigned char Send_Num_data1=0, SendNum_Over1=0, Send_Num_data2=0, SendNum_Over2=0;
unsigned char flag_485_SendDelay=0, Longth_Uart1Send=0, Longth_Uart2Send=0;

/**-------------------------------------------------------
* @函数名 __SZ_STM32_COMInit
* @功能   面向用户的STM32的USART初始化函数
* @参数1  COM1  对应STM32的USART1 对应开发板上串口1
*         COM2  对应STM32的USART2 对应开发板上串口2
* @参数2  BaudRate 串口的波特率，例如"115200"
* @返回值 无
***------------------------------------------------------*/
void PM2_5_Init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_11);
    delay(3000000);
    GPIO_SetBits(GPIOA,GPIO_Pin_11);
}

void SZ_STM32_Uart1Init(unsigned int BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;  
    USART_InitTypeDef USART_InitStructure; 
    
    /*使能GPIOA端口时钟、复用功能时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 
    
    /*使能USART1时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);   
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    /*串口1重映射*/
    //GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);   
    
    USART_InitStructure.USART_BaudRate = BaudRate;  
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  
    USART_InitStructure.USART_Parity = USART_Parity_No;  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  
    USART_Init(USART1, &USART_InitStructure);  
    
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    USART_Cmd(USART1, ENABLE); 
}

//  通过UART1发送字符串
void Uart1Putstring(const char *s)
{    
    while (*s != '\0')
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART_SendData(USART1, *(s++)); 
    }
}

void Uart1_SendStart(unsigned char* data_send, unsigned char num)
{
    Send_Num_data1 = 0;
    SendNum_Over1 = num;
    //清发送完成中断标志位
    USART_ClearITPendingBit(USART1, USART_IT_TC);
    //发送完成中断使能	
    USART_ITConfig(USART1, USART_IT_TC, ENABLE);
    USART1->DR = data_send[0];
    Send_Num_data1++;
}

void Uart1_SendStop(void)
{
    //关闭发送完成中断
    USART_ITConfig(USART1, USART_IT_TC, DISABLE);
    //清发送完成中断标志位
    USART_ClearITPendingBit(USART1, USART_IT_TC);
    Send_Num_data1 = 0;
}

/**-------------------------------------------------------
* @函数名 UsartIrqHandlerUser
* @功能   共用的串口中断处理函数
* @参数   SZ_USART 串口数据结构体指针
* @返回值 无
***------------------------------------------------------*/
//功能：收到一个字符处理
//参数：收到的字符
//返回值：无
void ReceOneChar_Uart1(unsigned char ReceCharacter)
{
    ReceiveData[num_ReceiveData++] = ReceCharacter;
    if(num_ReceiveData >= 20)
    {
   	num_ReceiveData = 0;  
   	flag_send_uart1 = 1;	
    }
}

void NVIC_Uart1Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);       
}

void SZ_STM32_Uart2Init(unsigned int BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;  
    USART_InitTypeDef USART_InitStructure; 
    
    /*使能GPIOA端口时钟、复用功能时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 
    
    /*使能USART2时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);   
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    /*串口2重映射*/
    //GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);   
    /*
    Bits per Byte: 1 起始位
    8 数据位， 首先发送最低有效位
    1 位作为奇偶校验
    1 停止位
    */
    USART_InitStructure.USART_BaudRate = BaudRate;  
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  
    USART_InitStructure.USART_Parity = USART_Parity_No;  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  
    USART_Init(USART2, &USART_InitStructure);  
    
    USART_Cmd(USART2, ENABLE); 
}

//  通过UART2发送字符串
void Uart2Putstring(const char *s)
{    
    while (*s != '\0')
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
        USART_SendData(USART2, *(s++)); 
    }
}

void Uart2_SendStart(unsigned char* data_send, unsigned char num)
{
    Send_Num_data2 = 0;
    SendNum_Over2 = num;
    //清发送完成中断标志位
    USART_ClearITPendingBit(USART2, USART_IT_TC);
    //发送完成中断使能	
    USART_ITConfig(USART2, USART_IT_TC, ENABLE);
    USART2->DR = data_send[0];
    Send_Num_data2++;
}

void Uart2_SendStop(void)
{
    //关闭发送完成中断
    USART_ITConfig(USART2, USART_IT_TC, DISABLE);
    //清发送完成中断标志位
    USART_ClearITPendingBit(USART2, USART_IT_TC);
    Send_Num_data2 = 0;
}

/**-------------------------------------------------------
* @函数名 UsartIrqHandlerUser
* @功能   共用的串口中断处理函数
* @参数   SZ_USART 串口数据结构体指针
* @返回值 无
***------------------------------------------------------*/
//功能：收到一个字符处理
//参数：收到的字符
//返回值：无
void ReceOneChar_Uart2(unsigned char ReceCharacter)
{
    ReceiveData[num_ReceiveData++] = ReceCharacter;
    if(num_ReceiveData >= 20)
    {
   	num_ReceiveData = 0;
   	flag_send_uart2 = 1;   	
    }
}

void NVIC_Uart2Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;  
    
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);        
}

void SZ_STM32_Uart3Init(unsigned int BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;  
    USART_InitTypeDef USART_InitStructure; 
    
    /*使能GPIOA端口时钟、复用功能时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
    
    /*使能USART1时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);   
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    
    USART_InitStructure.USART_BaudRate = BaudRate;  
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  
    USART_InitStructure.USART_Parity = USART_Parity_No;  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
    USART_InitStructure.USART_Mode = USART_Mode_Rx;  
    USART_Init(USART3, &USART_InitStructure);  
    
    USART_ClearFlag(USART3,USART_FLAG_TC|USART_FLAG_RXNE);
    USART_ClearITPendingBit(USART3,USART_FLAG_TC|USART_FLAG_RXNE|USART_FLAG_ORE);
    USART_Cmd(USART3, ENABLE); 
}

void NVIC_Uart3Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;  
    
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);        
}

/**-------------------------------------------------------
* @函数名 NVIC_GroupConfig
* @功能   配置NVIC中断优先级分组函数.
*         默认配置为1比特表示主优先级, 3比特表示次优先级
*         用户可以根据需要修改
* @参数   无
* @返回值 无
***------------------------------------------------------*/
void NVIC_GroupConfig(void)
{
    /* 配置NVIC中断优先级分组:
    - 1比特表示主优先级  主优先级合法取值为 0 或 1 
    - 3比特表示次优先级  次优先级合法取值为 0..7
    - 数值越低优先级越高，取值超过合法范围时取低bit位 
    */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    
    /*==================================================================================
    NVIC_PriorityGroup   |  主优先级范围  |  次优先级范围  |   描述
    ==================================================================================
    NVIC_PriorityGroup_0  |      0         |      0-15      |   0 比特表示主优先级
    |                |                |   4 比特表示次优先级 
    ----------------------------------------------------------------------------------
    NVIC_PriorityGroup_1  |      0-1       |      0-7       |   1 比特表示主优先级
    |                |                |   3 比特表示次优先级 
    ----------------------------------------------------------------------------------
    NVIC_PriorityGroup_2  |      0-3       |      0-3       |   2 比特表示主优先级
    |                |                |   2 比特表示次优先级 
    ----------------------------------------------------------------------------------
    NVIC_PriorityGroup_3  |      0-7       |      0-1       |   3 比特表示主优先级
    |                |                |   1 比特表示次优先级 
    ----------------------------------------------------------------------------------
    NVIC_PriorityGroup_4  |      0-15      |      0         |   4 比特表示主优先级
    |                |                |   0 比特表示次优先级   
    ==================================================================================*/
}

/******************* (C) COPYRIGHT 2013 www.armjishu.com *****END OF FILE****/
