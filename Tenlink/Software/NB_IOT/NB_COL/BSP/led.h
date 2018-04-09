#ifndef __LED_H__
#define __LED_H__	
#include "stm32f10x.h" 
#include "sys.h" 


#define RS485_EN		        PBout(1)	//485ģʽ����.0,����;1,����.
#define PLC_EN		                PAout(11)	//485ģʽ����.0,����;1,����.
#define LED1		                PBout(15)	//485ģʽ����.0,����;1,����.

void LED_Init(void);
void SZ_STM32_LED1Toggle(void);
void Control485_Init(void);
void SZ_STM32_Control485(void);
		 				    
#endif


