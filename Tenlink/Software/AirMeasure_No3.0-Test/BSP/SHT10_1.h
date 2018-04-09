/*************************************************************
                             \(^o^)/
  Copyright (C), 2013-2020, ZheJiang University of Technology
  File name  : SHT10_1.h 
  Author     : ziye334    
  Version    : V1.0 
  Data       : 2014/3/10      
  Description: Digital temperature and humidity sensor driver code
  
*************************************************************/
#ifndef __SHT10_1_H__
#define __SHT10_1_H__
#include "stm32f10x.h"
#include "SHT10.h"

/* GPIO相关宏定义 */
#define SHT10_1_AHB2_CLK        RCC_APB2Periph_GPIOA
#define SHT10_1_DATA_PIN        GPIO_Pin_11
#define SHT10_1_SCK_PIN         GPIO_Pin_12
#define SHT10_1_DATA_PORT       GPIOA
#define SHT10_1_SCK_PORT        GPIOA

#define SHT10_1_DATA_H()        GPIO_SetBits(SHT10_1_DATA_PORT, SHT10_1_DATA_PIN)                  //拉高DATA数据线
#define SHT10_1_DATA_L()        GPIO_ResetBits(SHT10_1_DATA_PORT, SHT10_1_DATA_PIN)                //拉低DATA数据线
#define SHT10_1_DATA_R()        GPIO_ReadInputDataBit(SHT10_1_DATA_PORT, SHT10_1_DATA_PIN)         //读DATA数据线

#define SHT10_1_SCK_H()         GPIO_SetBits(SHT10_1_SCK_PORT, SHT10_1_SCK_PIN)                    //拉高SCK时钟线
#define SHT10_1_SCK_L()         GPIO_ResetBits(SHT10_1_SCK_PORT, SHT10_1_SCK_PIN)                  //拉低SCK时钟线


void SHT10_1_Config(void);
void SHT10_1_ConReset(void);
u8 SHT10_1_SoftReset(void);
u8 SHT10_1_Measure(u16 *p_value, u8 *p_checksum, u8 mode);
void SHT10_1_Calculate(u16 t, u16 rh,float *p_temperature, float *p_humidity);
float SHT10_1_CalcuDewPoint(float t, float h);
void Air_Measure_test(void);

#endif