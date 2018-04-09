#include "ADC.h"

void ADC_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能GPIOC时钟      */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    /*PC0 作为模拟通道11输入引脚 */                       
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       //管脚0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   //输入模式
    GPIO_Init(GPIOB, &GPIO_InitStructure);    
}

void ADC_Configuration_CQ1(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	  //使能ADC时钟
    
    ADC_InitTypeDef ADC_InitStructure;        //ADC初始化结构体声明
    
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//独立的转换模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;		  //关闭扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;   //关闭连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//本实验使用的是软件触发方式
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //对齐方式,ADC为12位中，右对齐方式
    ADC_InitStructure.ADC_NbrOfChannel = 1;	 //开启通道数，1个
    ADC_Init(ADC1, &ADC_InitStructure);
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);  //ADC规则通道组， 第1个通道 采样顺序1，转换时间 
    
    ADC_Cmd(ADC1, ENABLE);   //使能ADC1
    
    ADC_ResetCalibration(ADC1);	   //重置ADC校准寄存器  	  
    while(ADC_GetResetCalibrationStatus(ADC1));  //等待重置完成
    
    ADC_StartCalibration(ADC1);	    //开始校准 
    while(ADC_GetCalibrationStatus(ADC1));	// 等待校准完成
    
    /*连续转换开始*/ 
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

