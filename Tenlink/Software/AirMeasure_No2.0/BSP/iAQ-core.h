/*************************************************************
                             \(^o^)/
  Copyright (C), 2013-2020, ZheJiang University of Technology
  File name  : SHT10.h 
  Author     : ziye334    
  Version    : V1.0 
  Data       : 2014/3/10      
  Description: Digital temperature and humidity sensor driver code
  
*************************************************************/
#ifndef __IAQ_CORE_H__
#define __IAQ_CORE_H__
#include "stm32f10x.h"
#include <stdbool.h>
#include <stdint.h>

void iCore_Dly(void);
void iCore_DATAOut(void);
void iCore_DATAIn(void);
void iCore_SCKOut(void);
void iCore_SCKIn(void);


/* GPIO相关宏定义 */
#define iCore_AHB2_CLK        RCC_APB2Periph_GPIOB
#define iCore_DATA_PIN        GPIO_Pin_7
#define iCore_SCK_PIN        	GPIO_Pin_8
#define iCore_DATA_PORT       GPIOB
#define iCore_SCK_PORT        GPIOB

#define iCore_DATA_H()        GPIO_SetBits(iCore_DATA_PORT, iCore_DATA_PIN)                  //拉高DATA数据线
#define iCore_DATA_L()        GPIO_ResetBits(iCore_DATA_PORT, iCore_DATA_PIN)                //拉低DATA数据线
#define iCore_DATA_R()        GPIO_ReadInputDataBit(iCore_DATA_PORT, iCore_DATA_PIN)         //读DATA数据线

#define iCore_SCK_H()        	GPIO_SetBits(iCore_SCK_PORT, iCore_SCK_PIN)                    //拉高SCK时钟线
#define iCore_SCK_L()        	GPIO_ResetBits(iCore_SCK_PORT, iCore_SCK_PIN)                  //拉低SCK时钟线
#define iCore_SCK_R()        	GPIO_ReadInputDataBit(iCore_SCK_PORT, iCore_SCK_PIN)         	//读SCK数据线

#define TWI_SCL_HIGH()   		iCore_SCK_H()
#define TWI_SCL_LOW()    		iCore_SCK_L()
#define TWI_SDA_HIGH()   		iCore_DATA_H()
#define TWI_SDA_LOW()    		iCore_DATA_L()
#define TWI_SDA_INPUT()  		iCore_DATAIn()
#define TWI_SCL_INPUT()  		iCore_SCKIn()
#define TWI_SDA_OUTPUT() 		iCore_DATAOut()
#define TWI_SCL_OUTPUT() 		iCore_SCKOut()
/*lint -restore */

#define TWI_SDA_READ() 			iCore_DATA_R()                     	/*!< Reads current state of SDA */
#define TWI_SCL_READ() 			iCore_SCK_R()                    	/*!< Reads current state of SCL */

#define TWI_DELAY() 				iCore_Dly() 								/*!< Time to wait when pin states are changed. For fast-mode the delay can be zero and for standard-mode 4 us delay is sufficient. */

extern u8 data_iCore[];
extern u16 CO2_iCore, VOC_iCore;

void iCoreInit(void);
void IAQRead(uint8_t *s);

#endif