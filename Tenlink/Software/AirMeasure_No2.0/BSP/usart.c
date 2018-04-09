/********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
* �ļ���  ��SZ_STM32F107VC_LIB.c
* ����    ���ṩSTM32F107VC����IV�ſ�����Ŀ⺯��
* ʵ��ƽ̨��STM32���ۿ�����
* ����    ��www.armjishu.com 
**********************************************************************************/
#include "usart.h"
#include "stm32f10x_conf.h"

unsigned char ReceiveData[50], SendData_Uart1[20], SendData_Uart2[20], num_ReceiveData=0, flag_send_uart1=0, flag_send_uart2=0;
unsigned char Send_Num_data1=0, SendNum_Over1=0, Send_Num_data2=0, SendNum_Over2=0;
unsigned char flag_485_SendDelay=0, Longth_Uart1Send=0, Longth_Uart2Send=0;

/**-------------------------------------------------------
* @������ __SZ_STM32_COMInit
* @����   �����û���STM32��USART��ʼ������
* @����1  COM1  ��ӦSTM32��USART1 ��Ӧ�������ϴ���1
*         COM2  ��ӦSTM32��USART2 ��Ӧ�������ϴ���2
* @����2  BaudRate ���ڵĲ����ʣ�����"115200"
* @����ֵ ��
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
    
    /*ʹ��GPIOA�˿�ʱ�ӡ����ù���ʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 
    
    /*ʹ��USART1ʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);   
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    /*����1��ӳ��*/
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

//  ͨ��UART1�����ַ���
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
    //�巢������жϱ�־λ
    USART_ClearITPendingBit(USART1, USART_IT_TC);
    //��������ж�ʹ��	
    USART_ITConfig(USART1, USART_IT_TC, ENABLE);
    USART1->DR = data_send[0];
    Send_Num_data1++;
}

void Uart1_SendStop(void)
{
    //�رշ�������ж�
    USART_ITConfig(USART1, USART_IT_TC, DISABLE);
    //�巢������жϱ�־λ
    USART_ClearITPendingBit(USART1, USART_IT_TC);
    Send_Num_data1 = 0;
}

/**-------------------------------------------------------
* @������ UsartIrqHandlerUser
* @����   ���õĴ����жϴ�����
* @����   SZ_USART �������ݽṹ��ָ��
* @����ֵ ��
***------------------------------------------------------*/
//���ܣ��յ�һ���ַ�����
//�������յ����ַ�
//����ֵ����
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
    
    /*ʹ��GPIOA�˿�ʱ�ӡ����ù���ʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 
    
    /*ʹ��USART2ʱ��*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);   
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    /*����2��ӳ��*/
    //GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);   
    /*
    Bits per Byte: 1 ��ʼλ
    8 ����λ�� ���ȷ��������Чλ
    1 λ��Ϊ��żУ��
    1 ֹͣλ
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

//  ͨ��UART2�����ַ���
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
    //�巢������жϱ�־λ
    USART_ClearITPendingBit(USART2, USART_IT_TC);
    //��������ж�ʹ��	
    USART_ITConfig(USART2, USART_IT_TC, ENABLE);
    USART2->DR = data_send[0];
    Send_Num_data2++;
}

void Uart2_SendStop(void)
{
    //�رշ�������ж�
    USART_ITConfig(USART2, USART_IT_TC, DISABLE);
    //�巢������жϱ�־λ
    USART_ClearITPendingBit(USART2, USART_IT_TC);
    Send_Num_data2 = 0;
}

/**-------------------------------------------------------
* @������ UsartIrqHandlerUser
* @����   ���õĴ����жϴ�����
* @����   SZ_USART �������ݽṹ��ָ��
* @����ֵ ��
***------------------------------------------------------*/
//���ܣ��յ�һ���ַ�����
//�������յ����ַ�
//����ֵ����
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
    
    /*ʹ��GPIOA�˿�ʱ�ӡ����ù���ʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
    
    /*ʹ��USART1ʱ��*/
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
* @������ NVIC_GroupConfig
* @����   ����NVIC�ж����ȼ����麯��.
*         Ĭ������Ϊ1���ر�ʾ�����ȼ�, 3���ر�ʾ�����ȼ�
*         �û����Ը�����Ҫ�޸�
* @����   ��
* @����ֵ ��
***------------------------------------------------------*/
void NVIC_GroupConfig(void)
{
    /* ����NVIC�ж����ȼ�����:
    - 1���ر�ʾ�����ȼ�  �����ȼ��Ϸ�ȡֵΪ 0 �� 1 
    - 3���ر�ʾ�����ȼ�  �����ȼ��Ϸ�ȡֵΪ 0..7
    - ��ֵԽ�����ȼ�Խ�ߣ�ȡֵ�����Ϸ���Χʱȡ��bitλ 
    */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    
    /*==================================================================================
    NVIC_PriorityGroup   |  �����ȼ���Χ  |  �����ȼ���Χ  |   ����
    ==================================================================================
    NVIC_PriorityGroup_0  |      0         |      0-15      |   0 ���ر�ʾ�����ȼ�
    |                |                |   4 ���ر�ʾ�����ȼ� 
    ----------------------------------------------------------------------------------
    NVIC_PriorityGroup_1  |      0-1       |      0-7       |   1 ���ر�ʾ�����ȼ�
    |                |                |   3 ���ر�ʾ�����ȼ� 
    ----------------------------------------------------------------------------------
    NVIC_PriorityGroup_2  |      0-3       |      0-3       |   2 ���ر�ʾ�����ȼ�
    |                |                |   2 ���ر�ʾ�����ȼ� 
    ----------------------------------------------------------------------------------
    NVIC_PriorityGroup_3  |      0-7       |      0-1       |   3 ���ر�ʾ�����ȼ�
    |                |                |   1 ���ر�ʾ�����ȼ� 
    ----------------------------------------------------------------------------------
    NVIC_PriorityGroup_4  |      0-15      |      0         |   4 ���ر�ʾ�����ȼ�
    |                |                |   0 ���ر�ʾ�����ȼ�   
    ==================================================================================*/
}

/******************* (C) COPYRIGHT 2013 www.armjishu.com *****END OF FILE****/
