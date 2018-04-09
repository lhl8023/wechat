#include "stdio.h"
#include "stm32f10x_conf.h"
#include "led.h"
#include "usart.h"
#include "timer.h"	
#include "Systick.h"
#include "Modbus.h"
#include "protocol.h"
#include "string.h"
#include "ADC.h"
#include "BKP.h"
#include "delay.h"
#include "CODE_MY.h"
#include "sys.h"
#include "i2c.h"

//全局变量定义
extern u16 PLC_Data;
extern u8 AutoSendData[30];

unsigned char PM25_Level;
uint32_t msTick=0, msBlinkTick=0,sTick = 0;
u16 lightVal=0;
u8 n;
extern unsigned char BACK[];
extern const unsigned char F16X40[];
extern const unsigned char F24X48[];
extern const unsigned char t[];
int count=0;
unsigned int falg_display=0, count_display=0;
//红外学习数组变量声明
u8 txsend_data[8]={0x01,0x03,0x00,0x01,0x00,0x08,0x15,0xCC};//帧格式：52,4d，5a，4e为包头；00 00 保留字； e6  长度为230；01是数据包的第一帧；17是一帧数据的长度23

//主函数


int main(void)
{
  u8 x;
  u8 n=0;

  //BKP_Init();
  DelayInit();
  SystemInit();

  __disable_irq();
  __enable_irq(); 
//  SHT10_Config();



  //定时器初始化
  TIM1_Init();
  TIM2_Init();
  TIM3_Init();
  TIM4_Init();

  //串口初始化
  SZ_STM32_Uart1Init(9600);
  SZ_STM32_Uart2Init(9600);
  SZ_STM32_Uart3Init(9600);

  //中断配置
  NVIC_GroupConfig();
  NVIC_TIM1Configuration();  
  NVIC_TIM2Configuration();
  NVIC_TIM3Configuration();
  NVIC_TIM4Configuration();
  NVIC_Uart1Configuration();
  NVIC_Uart2Configuration();
  NVIC_Uart3Configuration();

   ModBusSlaver_Init();
   ModBusMaster_Init();  
//     for(x=0;x<8;x++)
//            {
//              USART_ClearITPendingBit(USART1, USART_IT_TC);
//              USART_SendData(USART1, txsend_data[x]);  
//              while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
//            }
  while (1)
   {


      //接收ZigBee指令
      if(modbusdata_slaver.FrameOK == 1)
      {
         modbusdata_slaver.FrameOK = 0;
         MBus_Dispose_Slaver();
      }


      //接收PLC-IO状态数据
      if(PLC_modbusdata_master.FrameOK == 1)
      {
         PLC_modbusdata_master.FrameOK = 0;
         PLC_MBus_Dispose_master();
      }

   }  
}