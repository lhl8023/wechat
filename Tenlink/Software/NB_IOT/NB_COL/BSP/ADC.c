#include "ADC.h"

void ADC_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ʹ��GPIOCʱ��      */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    /*PC0 ��Ϊģ��ͨ��11�������� */                       
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       //�ܽ�0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   //����ģʽ
    GPIO_Init(GPIOB, &GPIO_InitStructure);    
}

void ADC_Configuration_CQ1(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	  //ʹ��ADCʱ��
    
    ADC_InitTypeDef ADC_InitStructure;        //ADC��ʼ���ṹ������
    
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//������ת��ģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;		  //�ر�ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;   //�ر�����ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//��ʵ��ʹ�õ������������ʽ
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //���뷽ʽ,ADCΪ12λ�У��Ҷ��뷽ʽ
    ADC_InitStructure.ADC_NbrOfChannel = 1;	 //����ͨ������1��
    ADC_Init(ADC1, &ADC_InitStructure);
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);  //ADC����ͨ���飬 ��1��ͨ�� ����˳��1��ת��ʱ�� 
    
    ADC_Cmd(ADC1, ENABLE);   //ʹ��ADC1
    
    ADC_ResetCalibration(ADC1);	   //����ADCУ׼�Ĵ���  	  
    while(ADC_GetResetCalibrationStatus(ADC1));  //�ȴ��������
    
    ADC_StartCalibration(ADC1);	    //��ʼУ׼ 
    while(ADC_GetCalibrationStatus(ADC1));	// �ȴ�У׼���
    
    /*����ת����ʼ*/ 
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
}

void NVIC_ADCConfiguration(void)
{
    ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
    
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

