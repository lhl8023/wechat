#include "stdio.h"
#include "stm32f10x_conf.h"
#include "led.h"
#include "usart.h"
#include "timer.h"	
#include "Systick.h"
#include "Modbus.h"
#include "protocol.h"
#include "string.h"
#include "SHT10.h"
#include "ADC.h"
#include "BKP.h"
#include "iAQ-core.h"
#include "delay.h"
#include "OLED_I2C.h"
#include "CODE_MY.h"
#include "BH1750.h"
#include "sys.h"
#include "i2c.h"


//全局变量定义
extern u16 CO2_iCore, VOC_iCore;
extern u16 PM2_5_Data;
float CO2,VOC,light;
int PM2_5_D,PM2_5[3];
unsigned char CO2_S[10],VOC_S[10],PM2_5_S[10],light_S[10];
extern float humi_val_real, temp_val_real, dew_point; 
extern u8 AutoSendData[20];

unsigned char temp_real[10];
unsigned char humi_real[10];
unsigned char dew_real[10];

unsigned char PM25_Level;
uint32_t msTick=0, msBlinkTick=0,sTick = 0;
u16 lightVal=0;
unsigned int hcho=0;
extern unsigned char BACK[];
extern const unsigned char F16X40[];
extern const unsigned char F24X48[];
extern const unsigned char t[];
int count=0;
extern u8 light_value;
extern float result_lx;
int VOC_LEAVE=0,SHT_LEAVE=0,BH_LEAVE=0,PM25_LEAVE=0;
unsigned char aaa[2];
u16 lx_value=0;
u8 n;
unsigned int falg_display=0, count_display=0;
u8 send_data[32];
//oled界面初始化
void GUI_Init()
{
  int i;
  OLED_OFF();
  OLED_Fill(0X00);
  OLED_ON();
  for(i=0;i<4;i++)
  {
    OLED_ShowCN(33+i*16,1,i);
  }
  OLED_ShowStr(15,5,"aispace technology",1);
  DelayS(2);
}


void OLED_PicDraw()
{
  OLED_ShowStr(85,7,"CO2",1);
  OLED_ShowStr(108,7,"VOC",1);
  
  if(CO2<=450)                                               //画一个格
  {
    OLED_ShowChar(93,6,'<',1);
    OLED_ShowChar(93,5,'>',1);
    OLED_ShowChar(93,4,'>',1);
    OLED_ShowChar(93,3,'>',1);
    OLED_ShowChar(93,2,'>',1);
    OLED_ShowChar(93,1,'>',1);
    OLED_ShowChar(93,0,'>',1);
  }
  else if((CO2>450)&&(CO2<=750))                            //画两个格
  {
    OLED_ShowChar(93,6,'<',1);
    OLED_ShowChar(93,5,'<',1);
    OLED_ShowChar(93,4,'>',1);
    OLED_ShowChar(93,3,'>',1);
    OLED_ShowChar(93,2,'>',1);
    OLED_ShowChar(93,1,'>',1);
    OLED_ShowChar(93,0,'>',1);
  }
  else if ((CO2>750)&&(CO2<=1050))                          //画三个格
  {
    OLED_ShowChar(93,6,'<',1);
    OLED_ShowChar(93,5,'<',1);
    OLED_ShowChar(93,4,'<',1);
    OLED_ShowChar(93,3,'>',1);
    OLED_ShowChar(93,2,'>',1);
    OLED_ShowChar(93,1,'>',1);
    OLED_ShowChar(93,0,'>',1);
  }
  else if ((CO2>1050)&&(CO2<=1350))                         //画四个格
  {
    OLED_ShowChar(93,6,'<',1);
    OLED_ShowChar(93,5,'<',1);
    OLED_ShowChar(93,4,'<',1);
    OLED_ShowChar(93,3,'<',1);
    OLED_ShowChar(93,2,'>',1);
    OLED_ShowChar(93,1,'>',1);
    OLED_ShowChar(93,0,'>',1);
  }
  else if ((CO2>1350)&&(CO2<=1650))                                         //画五个格
  {
    OLED_ShowChar(93,6,'<',1);
    OLED_ShowChar(93,5,'<',1);
    OLED_ShowChar(93,4,'<',1);
    OLED_ShowChar(93,3,'<',1);
    OLED_ShowChar(93,2,'<',1);
    OLED_ShowChar(93,1,'>',1);
    OLED_ShowChar(93,0,'>',1);
  }
  else if ((CO2>1650)&&(CO2<=1950))                                         //画五个格
  {
    OLED_ShowChar(93,6,'<',1);
    OLED_ShowChar(93,5,'<',1);
    OLED_ShowChar(93,4,'<',1);
    OLED_ShowChar(93,3,'<',1);
    OLED_ShowChar(93,2,'<',1);
    OLED_ShowChar(93,1,'<',1);
    OLED_ShowChar(93,0,'>',1);
  }
  else if (CO2>1950)                                         //画五个格
  {
    OLED_ShowChar(93,6,'<',1);
    OLED_ShowChar(93,5,'<',1);
    OLED_ShowChar(93,4,'<',1);
    OLED_ShowChar(93,3,'<',1);
    OLED_ShowChar(93,2,'<',1);
    OLED_ShowChar(93,1,'<',1);
    OLED_ShowChar(93,0,'<',1);
  }
  
  if(VOC<=125)                                              //画一个格
  {
    OLED_ShowChar(116,6,'<',1);
    OLED_ShowChar(116,5,'>',1);
    OLED_ShowChar(116,4,'>',1);
    OLED_ShowChar(116,3,'>',1);
    OLED_ShowChar(116,2,'>',1);
    OLED_ShowChar(116,1,'>',1);
    OLED_ShowChar(116,0,'>',1);        
  }
  else if((VOC>125)&&(VOC<=225))                            //画两个格
  {
    OLED_ShowChar(116,6,'<',1);
    OLED_ShowChar(116,5,'<',1);
    OLED_ShowChar(116,4,'>',1);
    OLED_ShowChar(116,3,'>',1);
    OLED_ShowChar(116,2,'>',1);
    OLED_ShowChar(116,1,'>',1);
    OLED_ShowChar(116,0,'>',1);   
  }
  else if ((VOC>225)&&(VOC<=325))                           //画三个格
  {
    OLED_ShowChar(116,6,'<',1);
    OLED_ShowChar(116,5,'<',1);
    OLED_ShowChar(116,4,'<',1);
    OLED_ShowChar(116,3,'>',1);
    OLED_ShowChar(116,2,'>',1);
    OLED_ShowChar(116,1,'>',1);
    OLED_ShowChar(116,0,'>',1);   
  }
  else if ((VOC>325)&&(VOC<=425))                           //画四个格
  {
    OLED_ShowChar(116,6,'<',1);
    OLED_ShowChar(116,5,'<',1);
    OLED_ShowChar(116,4,'<',1);
    OLED_ShowChar(116,3,'<',1);
    OLED_ShowChar(116,2,'>',1);
    OLED_ShowChar(116,1,'>',1);
    OLED_ShowChar(116,0,'>',1);   
  }
  else if ((VOC>425)&&(VOC<=525))                                         //画五个格
  {
    OLED_ShowChar(116,6,'<',1);
    OLED_ShowChar(116,5,'<',1);
    OLED_ShowChar(116,4,'<',1);
    OLED_ShowChar(116,3,'<',1);
    OLED_ShowChar(116,2,'<',1);
    OLED_ShowChar(116,1,'<',1);
    OLED_ShowChar(116,0,'>',1);   
  }
  else if (VOC>525)                                         //画五个格
  {
    OLED_ShowChar(116,6,'<',1);
    OLED_ShowChar(116,5,'<',1);
    OLED_ShowChar(116,4,'<',1);
    OLED_ShowChar(116,3,'<',1);
    OLED_ShowChar(116,2,'<',1);
    OLED_ShowChar(116,1,'<',1);
    OLED_ShowChar(116,0,'<',1);   
  }
}

void bigger_number(unsigned char x,unsigned char y,unsigned int A,unsigned char size)
{
  unsigned char c,i;
      c=A;
      if(x > 120)
      {
        x = 0;
        y++;
      }
      OLED_SetPos(x,y);
      for(i=0;i<24;i++)
        WriteDat(F24X48[c*144+i]);
      
      OLED_SetPos(x,y+1);
      for(i=0;i<24;i++)
        WriteDat(F24X48[c*144+i+24]);
      
      OLED_SetPos(x,y+2);
      for(i=0;i<24;i++)
        WriteDat(F24X48[c*144+i+48]);
      
      OLED_SetPos(x,y+3);
      for(i=0;i<24;i++)
        WriteDat(F24X48[c*144+i+72]);
      
      OLED_SetPos(x,y+4);
      for(i=0;i<24;i++)
        WriteDat(F24X48[c*144+i+96]);
      
      /*OLED_SetPos(x,y+5);
      for(i=0;i<24;i++)
        WriteDat(F24X48[c*144+i+120]);
      
      OLED_SetPos(x,y+6);
      for(i=0;i<24;i++)
        WriteDat(F16X40[c*48+i+144]);
      OLED_SetPos(x,y+7);
      for(i=0;i<24;i++)
        WriteDat(F16X40[c*48+i+168]);*/
      x += 24;
}

void big_number(unsigned char x,unsigned char y,unsigned int A,unsigned char size)
{
  unsigned char c,i;
      c=A;
      if(x > 120)
      {
        x = 0;
        y++;
      }
      OLED_SetPos(x,y);
      for(i=0;i<16;i++)
        WriteDat(F16X40[c*80+i]);
      OLED_SetPos(x,y+1);
      for(i=0;i<16;i++)
        WriteDat(F16X40[c*80+i+16]);
      OLED_SetPos(x,y+2);
      for(i=0;i<16;i++)
        WriteDat(F16X40[c*80+i+32]);
      OLED_SetPos(x,y+3);
      for(i=0;i<16;i++)
        WriteDat(F16X40[c*80+i+48]);
      /*OLED_SetPos(x,y+4);
      for(i=0;i<16;i++)
        WriteDat(F16X40[c*80+i+64]);*/
      x += 16;
}

void signal(unsigned char x,unsigned char y)
{
  unsigned char c,i;
      c=0;
      if(x > 120)
      {
        x = 0;
        y++;
      }
      OLED_SetPos(x,y);
      for(i=0;i<8;i++)
        WriteDat(t[c*16+i]);
      OLED_SetPos(x,y+1);
      for(i=0;i<8;i++)
        WriteDat(t[c*16+i+8]);
      x += 8;
}
//oled界面进程
void GUI_Process()
{
  int i=0;
  //背景图绘制
  //OLED_DrawBMP(0,0,128,8,(unsigned char *)BACK);
  
  //温湿度数据处理及绘制
//  sprintf(temp_real,"%2.0f", temp_val_real);
//  sprintf(humi_real,"%2.0f", humi_val_real);
//  if(SHT_LEAVE==1)
//  {
//    OLED_ShowStr(4,6,"00",2);
//    OLED_ShowStr(46,6,"00",2);     
//  }
//  else
//  {
//    OLED_ShowStr(4,6,temp_real,2);
//    OLED_ShowStr(46,6,humi_real,2);    
//  }
//  signal(24,6);
//  OLED_ShowStr(64,6,"%",2);
//  lx_value=(u16)result_lx;
//  aaa[0] = (lx_value<<8)&0x0FF;
//  aaa[1] = lx_value&0x0FF;
//  OLED_ShowStr(4,6,aaa,2);

  
  //PM2.5数据处理及绘制
  //OLED_ShowStr(0,1,"PM2.5:",1);
  PM2_5_D=(int)PM2_5_Data;
  if(PM2_5_D>=999)
  {
    PM2_5_D=999;
  }
  PM2_5[0]=PM2_5_D/100;
  PM2_5[1]=PM2_5_D/10%10;
  PM2_5[2]=PM2_5_D%10;
  
  if(PM25_LEAVE==1)
  {
    bigger_number(4+2*24,1,PM2_5[i],3);
  }
  else
  {
    for(;i<3;i++)
    {
      if(i==0)
      {
        if(PM2_5[i]==0)
        {
          OLED_ShowStr(4,6,">>>>",1);
          OLED_ShowStr(4,1,">>>>",1);
          OLED_ShowStr(4,2,">>>>",1);
          OLED_ShowStr(4,3,">>>>",1);
          OLED_ShowStr(4,4,">>>>",1);
          OLED_ShowStr(4,5,">>>>",1);
          continue;
        }
      }
      bigger_number(4+i*24,1,PM2_5[i],3);
    }
  }
  
  //bigger_number(32,0,4,3);
  
  //CO2/VOC数据处理及绘制
  //OLED_ShowStr(0,4,"CO2 :",1);
  //OLED_ShowStr(0,5,"VOC :",1);
  CO2=(float)CO2_iCore;
  VOC=(float)VOC_iCore;
  sprintf(CO2_S,"%-4.0f", CO2);
  sprintf(VOC_S,"%-4.0f", VOC);
  //OLED_ShowStr(50,4,CO2_S,1);
  //OLED_ShowStr(50,5,VOC_S,1);
  
  
  //CO2柱状图绘制实验
  OLED_PicDraw();
}

void Test_IO(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;
    
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   GPIOA->ODR &= ~GPIO_Pin_0;  
}

//计算pm2.5污染等级
unsigned char PM25_LevelCau()
{
  unsigned char a;
  if(PM2_5_Data<=50)
    a='0';
  else if (PM2_5_Data>50&&PM2_5_Data<=100)
    a='1';
  else if (PM2_5_Data>100&&PM2_5_Data<=150)
    a='2';
  else if (PM2_5_Data>150&&PM2_5_Data<=200)
    a='3';
  else if (PM2_5_Data>200&&PM2_5_Data<=300)
    a='4';
  else if (PM2_5_Data>300)
    a='5';
  else 
    a=NULL;
}

//数据处理
void DATA_Deal()
{
  u16 temp,humi,dew,lx_value;
  temp=(u16)(temp_val_real*10);
  humi=(u16)(humi_val_real*10);
  dew=(u16)(dew_point*10); 
  if((humi<=0)||(humi>=1000))
    SHT_LEAVE=1;
  else
  {
    SHT_LEAVE=0;
    
    //温度
    AutoSendData[6] = (temp>>8)&0xff;
    AutoSendData[7] = temp&0xff;
    
    //湿度
    AutoSendData[8] = (humi>>8)&0xff;
    AutoSendData[9] = humi&0xff;
  }
  
  //VOC数据处理  
  
  if((VOC_iCore>3000)||(VOC_iCore<100)||(CO2_iCore>3000)||(CO2_iCore<100))
    VOC_LEAVE=1;
  else
  {
    VOC_LEAVE=0;
    
    AutoSendData[0] = (VOC_iCore>>8)&0x0FF;
    AutoSendData[1] = VOC_iCore&0x0FF;
    AutoSendDataOK |= 0x01;    
    
      //CO2数据处理
    AutoSendData[10] = (CO2_iCore>>8)&0x0FF;
    AutoSendData[11] = CO2_iCore&0x0FF;
    AutoSendDataOK |= 0x01;
  }
  
  //PM2.5数据处理
  if((PM2_5_Data<=0)||(PM2_5_Data>3000))
    PM25_LEAVE=1;
  else
  {
    PM25_LEAVE=0;
    AutoSendData[2] = (PM2_5_Data
                       8)&0x0FF;
    AutoSendData[3] = PM2_5_Data&0x0FF;
    AutoSendDataOK |= 0x10;
    PM25_Level=PM25_LevelCau();    
  }
  
  lx_value=(u16)result_lx;
  AutoSendData[4] = (lx_value>>8)&0x0FF;
  AutoSendData[5] = lx_value&0x0FF;
}

//检查是否有紧急情况
void emergency_check()
{
  if(PM2_5_Data > 250)
  {
    EmergencySendFlag |= 0x10;
    //OLED_ShowChar(110,6,'P',1);
  }
  
  if(VOC_iCore>600)
  {
    EmergencySendFlag |= 0x01;//紧急上报
    //OLED_ShowChar(110,5,'V',1);
  }
}

void leave_check()
{
  if(PM25_LEAVE==1)
  {
    AutoSendData[2] = 0x0FF;
    AutoSendData[3] = 0x0FF;
  }
  if(VOC_LEAVE==1)
  {
    AutoSendData[0] = 0xFF;
    AutoSendData[1] = 0xFF;   
    AutoSendData[10] = 0xFF;
    AutoSendData[11] = 0xFF;
  }
//  if(SHT_LEAVE==1)
//  {
//    AutoSendData[6] = 0xff;
//    AutoSendData[7] = 0xff;
//    
//    //湿度
//    AutoSendData[8] = 0xff;
//    AutoSendData[9] = 0xff;    
//  }
}

void Hart_Init()
{
  	GPIO_InitTypeDef GPIO_InitStructure;        
	//初始化SHT10引脚时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	       
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	        
}


void PC_GPIO_Init()
{
  
  //WR_CTRL低则芯片，高则usb
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);//开C口时钟，复用时钟。
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  //设为输出　注意：2MHZ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  //设为输出　注意：2MHZ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
   
  PWR_BackupAccessCmd(ENABLE);//允许修改RTC 和后备寄存器
  RCC_LSEConfig(RCC_LSE_OFF);//关闭外部低速外部时钟信号功能 后，PC13 PC14 PC15 才可以当普通IO用。
  BKP_TamperPinCmd(DISABLE);//关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用
  PWR_BackupAccessCmd(DISABLE);//禁止修改后备寄存器  
}
//功能:异或校验
//参数: 要进行异或校验的消息，消息中字节数(不含校验位)
//返回值：异或校验码
unsigned char XOR(unsigned char *puchMsg, unsigned int usDataLen)
{
  unsigned char code_xor;
  usDataLen-=1;
  code_xor=puchMsg[usDataLen];
  
  while (usDataLen--)
  {
    code_xor ^= puchMsg[usDataLen];
  
  }
  return code_xor;
}

//主函数
int main(void)
{	
  Test_IO();
  //BKP_Init();
  DelayInit();
  I2C_Configuration();
  OLED_Init();
  GUI_Init();
  OLED_OFF();
  OLED_Fill(0X00);
  OLED_ON();
  SystemInit();
  PC_GPIO_Init();
  Init_BH1750();
  Hart_Init();
  SHT10_Config();
  __disable_irq();
  __enable_irq(); 
//  SHT10_Config();

  GPIOA->ODR &= ~GPIO_Pin_1; 
//   
//  SHT10_Config();
  iCoreInit();

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

  GUI_Process();     
  while (1)
   {
//        Start_BH1750();  //power on
//        DelayMs(180);              //延时180ms
//        Read_BH1750();     //连续读出数据，存储在BUF中
//        Convert_BH1750(); //转换结果至result_lx     
     
     //单刀双掷
     if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)==RESET)
       GPIO_ResetBits(GPIOC,GPIO_Pin_13);
     if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)==SET)
       GPIO_SetBits(GPIOC,GPIO_Pin_13);
     
         //SZ_STM32_LED1Toggle();
      if(falg_display)
     {
        falg_display = 0;
        GUI_Process();
     }
      //GUI_Process(); 
      DATA_Deal();
      
      
      //接收ZigBee指令
      if(modbusdata_slaver.FrameOK == 1)
      {
         modbusdata_slaver.FrameOK = 0;
         GPIOA->ODR ^= GPIO_Pin_1;
         MBus_Dispose_Slaver();
      }

      //接收PM2.5自动输出数据
      if(PM2_5_modbusdata_master.FrameOK == 1)
      {
         PM2_5_modbusdata_master.FrameOK = 0;
         PM2_5_MBus_Dispose_Master();
      }

      if(flag_send_uart2 == 1)
      {
         flag_send_uart2 = 0;
         Uart2_SendStart(SendData_Uart2, Longth_Uart2Send);
      }
      
      //2s钟接收一次数据
      if(AutoSendTimerOver)
      {
         AutoSendTimerOver = 0;
         PM2_5_flag_echo_master = 1;
         PM2_5_Start_DeviceNoEcho = 1;
         VOC_flag_echo_master = 1;
         VOC_Start_DeviceNoEcho = 1;
         
         Air_Measure();                         //读取温湿度传感器数值
         IAQRead(data_iCore);                   //读取VOC
         
        Start_BH1750();  //power on
        DelayMs(180);              //延时180ms
        Read_BH1750();     //连续读出数据，存储在BUF中
        Convert_BH1750(); //转换结果至result_lx
      }
      
      emergency_check();
      leave_check();
      
//      //22s自动上传
//      if(EmergencySendFlag&0x11)
//      {
//         GPIOA->ODR ^= GPIO_Pin_1;
//         AutoSendFlag = 0;
//         temp_send[0] = ADDR_MODBUS_S;
//         temp_send[1] = 0xC1;
//         temp_send[2] = 0x00;
//         if(VOC_iCore!=0)
//         {
//           temp_send[2]|=0x01;
//         }
//         if(PM2_5_Data!=0)
//         {
//           temp_send[2]|=0x02;
//         }
//         if(lx_value!=0)
//         {
//           temp_send[2]|=0x04;
//         }
//         if(temp_val_real!=0)
//         {
//           temp_send[2]|=0x08;
//         }
//         if(humi_val_real!=0)
//         {
//            temp_send[2]|=0x10;
//         }
//         if(CO2_iCore!=0)
//         {
//           temp_send[2]|=0x20;
//         }
//                  
//         temp_send[3] = 0x00;
//         temp_send[4] = 0x00;
//         
//         memcpy(&temp_send[5],AutoSendData,12);
//         temp_crc16_code  = CRC16(temp_send, 17, 0);
//         temp_send[17] = temp_crc16_code >> 8;
//         temp_send[18] = temp_crc16_code & 0xff;
//         memcpy(SendData_Uart2,temp_send,19);
//         memset(temp_send,0,20);
//         Longth_Uart2Send = 19;
//         flag_send_uart2 = 1;
//         
//         EmergencySendFlag = 0;
//      }
      
      //20s自动上传
          if(AutoSendFlag)
          {
             GPIOA->ODR ^= GPIO_Pin_1;
             AutoSendFlag = 0;
             
             temp_send[0] = 0x04;
             temp_send[1] = 0x01;
             if(VOC_iCore!=0)
             {
               temp_send[3]|=0x01;
             }
             if(PM2_5_Data!=0)
             {
               temp_send[3]|=0x02;
             }
             if(lx_value!=0)
             {
               temp_send[3]|=0x04;
             }
             if(temp_val_real!=0)
             {
               temp_send[3]|=0x08;
             }
             if(humi_val_real!=0)
             {
               temp_send[3]|=0x10;
             }
             if(CO2_iCore!=0)
             {
               temp_send[3]|=0x20;
             }
             temp_send[4]=0x02;
             temp_send[5]=AutoSendData[0];
             temp_send[6]=AutoSendData[1];
             temp_send[7]=0x03;
             temp_send[8]=AutoSendData[2];
             temp_send[9]=AutoSendData[3];
             temp_send[10]=0x04;
             temp_send[11]=AutoSendData[4];
             temp_send[12]=AutoSendData[5];             
             temp_send[13]=0x05;
             temp_send[14]=AutoSendData[6];
             temp_send[15]=AutoSendData[7];
             temp_send[16]=0x06;
             temp_send[17]=AutoSendData[8];
             temp_send[18]=AutoSendData[9];
             temp_send[19]=0x07;
             temp_send[20]=AutoSendData[10];
             temp_send[21]=AutoSendData[11];
             send_data[0]=0xa0;
             send_data[1]=0x00;
             send_data[2]=0x00;
             send_data[3]=0x00;
             send_data[4]=0x17;
             memcpy(&send_data[5],temp_send,22);
             send_data[27]=XOR(send_data,27);
              for(n=0;n<28;n++)
              {
                USART_ClearITPendingBit(USART2, USART_IT_TC);
                USART_SendData(USART2, send_data[n]);  
                while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
               }

//             memcpy(SendData_Uart2,temp_send,25);
//             memset(temp_send,0,26);
//             Longth_Uart2Send = 25;
//             flag_send_uart2 = 1;
             
             AutoSendDataOK = 0x00;
          }            
   }  
}