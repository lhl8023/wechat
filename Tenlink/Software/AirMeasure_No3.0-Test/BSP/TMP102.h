
#ifndef  __TMP102_H__
#define  __TMP102_H__

#include "stm32f10x.h"

/* GPIO相关宏定义 */
#define SHT10_1_AHB2_CLK        RCC_APB2Periph_GPIOA
#define SHT10_1_DATA_PIN        GPIO_Pin_12
#define SHT10_1_SCK_PIN         GPIO_Pin_11
#define SHT10_1_DATA_PORT       GPIOA
#define SHT10_1_SCK_PORT        GPIOA

#define SHT10_1_DATA_H()        GPIO_SetBits(SHT10_1_DATA_PORT, SHT10_1_DATA_PIN)                  //拉高DATA数据线
#define SHT10_1_DATA_L()        GPIO_ResetBits(SHT10_1_DATA_PORT, SHT10_1_DATA_PIN)                //拉低DATA数据线
#define SHT10_1_DATA_R()        GPIO_ReadInputDataBit(SHT10_1_DATA_PORT, SHT10_1_DATA_PIN)         //读DATA数据线

#define SHT10_1_SCK_H()         GPIO_SetBits(SHT10_1_SCK_PORT, SHT10_1_SCK_PIN)                    //拉高SCK时钟线
#define SHT10_1_SCK_L()         GPIO_ResetBits(SHT10_1_SCK_PORT, SHT10_1_SCK_PIN)                  //拉低SCK时钟线

#define 	SCL_HIGH 	SHT10_1_SCK_H()
#define 	SCL_LOW 		SHT10_1_SCK_L()
#define 	SDA_HIGH 	SHT10_1_DATA_H()
#define 	SDA_LOW 		SHT10_1_DATA_L()
#define 	SDA_OUT 		SHT10_1_DATAOut()
#define 	SDA_IN 		SHT10_1_DATAIn()
#define 	SDA_Data 	SHT10_1_DATA_R()

#define		TMP102_TEMP_REG		0x00
#define		TMP102_CONF_REG		0x01
/* note: these bit definitions are byte swapped */
#define		TMP102_CONF_SD			0x0100
#define		TMP102_CONF_TM			0x0200
#define		TMP102_CONF_POL		0x0400
#define		TMP102_CONF_F0			0x0800
#define		TMP102_CONF_F1			0x1000
#define		TMP102_CONF_R0			0x2000
#define		TMP102_CONF_R1			0x4000
#define		TMP102_CONF_OS			0x8000
#define		TMP102_CONF_EM			0x0010
#define		TMP102_CONF_AL			0x0020
#define		TMP102_CONF_CR0		0x0040
#define		TMP102_CONF_CR1		0x0080
#define		TMP102_TLOW_REG		0x02
#define		TMP102_THIGH_REG		0x03

float ReadTemp(void);
void Tmp102_Set(unsigned char flag);

void SHT10_1_Config(void);
void Air_Measure_test(void);

#endif   /* __tmp102_h__ */

