#include "Modbus.h"
#include "string.h"
#include "usart.h"
#include "protocol.h"
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "stdio.h"
unsigned char flag_type=0, flag_function=0,flag_addr=0,echo_code_s=0;
u8 PLC_write_OK=0;
u8 REC_OK=0;
u8 AutoSendTimerOver,PLC_type;
u32 AutoSendTime;     //自动发送时间
u8 AutoSendFlag;     //自动发送标志 
u8 addr=0x01,Q_write=0x06,Q_read=0x03;    //从机地址，线圈寄存器状态的读、写            
u8 AutoSendOverTime;  //自动发送超时时间
u8 PLC_Data_H,PLC_Data_L,PLC_Addr,Zig_Data_H,Zig_Data_L,Zig_mode;         //PLC数据、地址
u8 AutoSendData[20]; // 自动发送数据缓存区
u8 reg[10];
u8 Zig_Addr_L[10];
u8 reg_data[10];
u8 send_zigbee[50];
u8 reg_send_data[8]={0x01,0x06,0x00,0x00,0x00,0x01,0x48,0x0A};
u8 plc_rec[8]={0x01,0x03,0x00,0x01,0x00,0x07,0x94,0x0D};
u16 CRC_temp;
u8 NB_data[100];
u8 senor[50];
u8 air[50];
u8 reg_rev_data[50];
u8 DO,DI,AI,AO,Addr_data;
u8 y=0,m=0,x=0;
u8 flag_add=0;//定时器计数标志位
unsigned char data_O_reg = 0;//向上位机返回数据的寄存器长度(温控系统协议)
//data_O—生成的对从机数据
//data_S—待处理协议数据
//返回值：data_O_reg_len 执行结果，成功=返回1，失败=返回0
u8 APN[27]={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x43,0x41,0x4c,0x4c,0x3d,0x31,0x2c,0x22,0x63,0x6d,0x6e,0x62,0x69,0x6f,0x74,0x22,0x0d,0x0a};//链接网络
//u8 topicP[38]={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x43,0x46,0x47,0x3d,0x22,0x74,0x6f,0x70,0x69,0x63,0x22,0x2c,0x22,0x2f,0x78,0x79,0x5f,0x74,0x65,0x73,0x74,0x32,0x5f,0x73,0x75,0x62,0x22,0x0d,0x0a};//订阅主题  xingyun
//u8 topicS[38]={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x43,0x46,0x47,0x3d,0x22,0x74,0x6f,0x70,0x69,0x63,0x22,0x2c,0x22,0x2f,0x78,0x79,0x5f,0x74,0x65,0x73,0x74,0x32,0x5f,0x70,0x75,0x62,0x22,0x0d,0x0a};//发布主题  xingyun
//u8 topicS[38]={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x43,0x46,0x47,0x3d,0x22,0x74,0x6f,0x70,0x69,0x63,0x22,0x2c,0x22,0x2f,0x6c,0x68,0x6c,0x5f,0x74,0x65,0x73,0x74,0x5f,0x73,0x75,0x62,0x22,0x0d,0x0a};//订阅主题    test
//u8 topicP[38]={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x43,0x46,0x47,0x3d,0x22,0x74,0x6f,0x70,0x69,0x63,0x22,0x2c,0x22,0x2f,0x6c,0x68,0x6c,0x5f,0x74,0x65,0x73,0x74,0x5f,0x70,0x75,0x62,0x22,0x0d,0x0a};//发布主题    test
//u8 clitd[40] ={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x43,0x46,0x47,0x3d,0x22,0x63,0x6c,0x69,0x65,0x6e,0x74,0x69,0x64,0x22,0x2c,0x22,0x78,0x79,0x5f,0x74,0x65,0x73,0x74,0x32,0x5f,0x70,0x6c,0x63,0x22,0x0d,0x0a};//客户端 xingyun
//u8 clitd[40] ={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x43,0x46,0x47,0x3d,0x22,0x63,0x6c,0x69,0x65,0x6e,0x74,0x69,0x64,0x22,0x2c,0x22,0x6c,0x68,0x6c,0x5f,0x74,0x65,0x73,0x74,0x5f,0x70,0x75,0x62,0x22,0x0d,0x0a};//客户端   test
u8 seion[26]={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x43,0x46,0x47,0x3d,0x22,0x73,0x65,0x73,0x73,0x69,0x6f,0x6e,0x22,0x2c,0x30,0x0d,0x0a};//保留订阅
u8 setd[27]={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x43,0x46,0x47,0x3d,0x22,0x72,0x65,0x74,0x61,0x69,0x6e,0x65,0x64,0x22,0x2c,0x31,0x0d,0x0a};//消息保留
u8 qos[22]={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x43,0x46,0x47,0x3d,0x22,0x71,0x6f,0x73,0x22,0x2c,0x30,0x0d,0x0a};//消息等级
//u8 user[40]={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x43,0x46,0x47,0x3d,0x22,0x75,0x73,0x65,0x72,0x6e,0x61,0x6d,0x65,0x22,0x2c,0x22,0x78,0x79,0x5f,0x74,0x65,0x73,0x74,0x32,0x5f,0x70,0x6c,0x63,0x22,0x0d,0x0a};//账户   xingyun
//u8 user[40]={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x43,0x46,0x47,0x3d,0x22,0x75,0x73,0x65,0x72,0x6e,0x61,0x6d,0x65,0x22,0x2c,0x22,0x6c,0x68,0x6c,0x5f,0x74,0x65,0x73,0x74,0x5f,0x70,0x75,0x62,0x22,0x0d,0x0a};//账户    test
u8 passd[34]={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x43,0x46,0x47,0x3d,0x22,0x70,0x61,0x73,0x73,0x77,0x6f,0x72,0x64,0x22,0x2c,0x22,0x31,0x32,0x33,0x34,0x35,0x36,0x22,0x0d,0x0a};//密码
u8 OPEN[41]={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x4f,0x50,0x45,0x4e,0x3d,0x22,0x31,0x31,0x38,0x2e,0x31,0x39,0x30,0x2e,0x31,0x33,0x36,0x2e,0x31,0x36,0x37,0x22,0x2c,0x31,0x38,0x38,0x33,0x2c,0x36,0x30,0x0d,0x0a};//链接MQTT服务器
u8 SUB[16]={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x53,0x55,0x42,0x3d,0x31,0x0d,0x0a};//订阅主题
u8 PUB[16]={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x50,0x55,0x42,0x3d,0x31,0x0d,0x0a};//发布主题
u8 CALL[16]={0x0d,0x0D,0x0a,0x2b,0x4d,0x51,0x54,0x54,0x43,0x41,0x4c,0x4c,0x3a,0x31,0x0d,0x0a};//联网反馈
u8 OPENF[16]={0x0D,0x0D,0x0A,0x2B,0x4D,0x51,0x54,0x54,0x43,0x41,0x4C,0x4C,0x3A,0x31,0x0D,0x0A};//链接服务器反馈
u8 PUBF[15]={0x0D,0x0D,0x0A,0x2B,0x4D,0x51,0x54,0x54,0x50,0x55,0x42,0x3A,0x31,0x0D,0x0A};
u8 messg[100]={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x43,0x46,0x47,0x3d,0x22,0x6d,0x65,0x73,0x73,0x61,0x67,0x65,0x22,0x2c,0x30,0x2c,0x22};//消息
u8 meg[37]={0x41,0x54,0x2b,0x4c,0x53,0x4d,0x51,0x54,0x54,0x43,0x46,0x47,0x3d,0x22,0x6d,0x65,0x73,0x73,0x61,0x67,0x65,0x22,0x2c,0x30,0x2c,0x22,0x61,0x30,0x30,0x30,0x30,0x30,0x30,0x32,0x22,0x0d,0x0a};//消息
u8 call[12]="+MQTTCALL:0";//mqtt网络连接反馈 连接失败
u8 open[12]="+MQTTOPEN:0";
u8 topicP[]="AT+LSMQTTCFG=\"topic\",\"/tenlink/zhanhui-taiji-command/pub\"\r\n";
u8 topicS[]="AT+LSMQTTCFG=\"topic\",\"/tenlink/zhanhui-taiji-command/sub\"\r\n";
u8 clitd[]="AT+LSMQTTCFG=\"clientid\",\"taiji_zhanhui_lhl\"\r\n";
u8 user[]="AT+LSMQTTCFG=\"username\",\"taiji_zhanhui_lhl\"\r\n";
//a00000021601010001020001030001040001050001060001070001b8  /lhl_test_pub
u8 DATA;
u8 stat_data[9]={0x01,0x03,0x00,0x01,0x00,0x07,0x55,0xC8};
void send_cmd(u8 *data,u8 data_len)//命令发送
{
  u8 i;
  for(i=0;i<data_len;i++)
    {
        USART_ClearITPendingBit(USART2, USART_IT_TC);
        USART_SendData(USART2, data[i]);  
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
    }
}
void AT_CMD(void)//AT命令配置
{
   send_cmd(topicS,sizeof(topicS)); 
   DelayMs(1000);
   send_cmd(meg,37); 
   DelayMs(1000);
   send_cmd(clitd,sizeof(clitd));
   DelayMs(1000);
   send_cmd(seion,26); 
   DelayMs(1000);
   send_cmd(setd,27);
   DelayMs(1000);
   send_cmd(qos,22); 
   DelayMs(1000);
   send_cmd(user,sizeof(user));
   DelayMs(1000);
   send_cmd(passd,34); 
   DelayMs(1000);
   send_cmd(APN,27); 
   DelayMs(5000);
   DelayMs(5000);
   send_cmd(OPEN,41);
   DelayMs(5000);
   DelayMs(5000);
   send_cmd(SUB,16);
   DelayMs(2000);
   send_cmd(topicP,sizeof(topicP));
   DelayMs(2000);
   send_cmd(PUB,16);
   
}
void link(void)//连接服务器及网络
{
   send_cmd(APN,27); 
   DelayMs(5000);
   send_cmd(OPEN,41);
   DelayMs(5000);
   send_cmd(SUB,16);
}
void Turn_up(void)//数据上传
{
   send_cmd(PUB,16);
}
unsigned char Protocol_Data(unsigned char *data_S,unsigned char data_len)
{  
      u8 x;
       PLC_type=data_S[0];//PLC功能       
       reg[0]=data_S[1];  //寄存器存储
       reg[1]=data_S[4];
       reg[2]=data_S[7];
       reg[3]=data_S[10];
       reg[4]=data_S[13];
       reg[5]=data_S[16];
       reg[6]=data_S[19];
       
  if(PLC_type== 0x01)//DO写控制
  {   
       if(reg[0]!=0)
       {
         PLC_Data_H=data_S[2];   //寄存器存储高字节数据
         PLC_Data_L=data_S[3];  //寄存器存储低字节数据
         PLC_Addr=reg[0];
         Send_PLC();
         
       }
      if(reg[1]!=0)
       {
         delay_PLC(100);
         PLC_Data_H=data_S[5];   //寄存器存储高字节数据
         PLC_Data_L=data_S[6];  //寄存器存储低字节数据    
         PLC_Addr=reg[1];
         Send_PLC();
  
       }
       if(reg[2]!=0)
       {
         delay_PLC(100);
         PLC_Data_H=data_S[8];   //寄存器存储高字节数据
         PLC_Data_L=data_S[9];  //寄存器存储低字节数据
         PLC_Addr=reg[2];
         Send_PLC();
       }
       if(reg[3]!=0)
       {
         delay_PLC(100);
         PLC_Data_H=data_S[11];   //寄存器存储高字节数据
         PLC_Data_L=data_S[12];  //寄存器存储低字节数据
         PLC_Addr=reg[3];
         Send_PLC();
       }
       if(reg[4]!=0)
       {
         delay_PLC(100);
         PLC_Data_H=data_S[14];   //寄存器存储高字节数据
         PLC_Data_L=data_S[15];  //寄存器存储低字节数据
         PLC_Addr=reg[4];
         Send_PLC();
       }
       if(reg[5]!=0)
       {
         delay_PLC(100);
         PLC_Data_H=data_S[17];   //寄存器存储高字节数据
         PLC_Data_L=data_S[18];  //寄存器存储低字节数据
         PLC_Addr=reg[5];
         Send_PLC();
       }
       if(reg[6]!=0)
       {
         delay_PLC(100);
         PLC_Data_H=data_S[20];   //寄存器存储高字节数据
         PLC_Data_L=data_S[21];  //寄存器存储低字节数据
         PLC_Addr=reg[6];
         Send_PLC();
       }
       DelayMs(500);
  }

else data_O_reg=0;

  return data_O_reg;
}

unsigned char Protocol_PLC(unsigned char *data_S,unsigned char data_len)
{
  if(delay_data[0]== 0x01)//从机地址
  { 
       reg[0]=delay_data[1];  //类型      
       reg[1]=delay_data[3];   //地址
       reg[2]=delay_data[11];   //地址
       reg[3]=delay_data[19];   //地址
       reg[4]=delay_data[27];   //地址
       reg[5]=delay_data[35];   //地址
       reg[6]=delay_data[43];   //地址
       reg[7]=delay_data[51];   //地址
    
     if(reg[0]==0x06)//read data
     { 
       Addr_data=reg[1];
       if(reg[1]!=0)
       {
          AutoSendData[0]=delay_data[4]; 
          AutoSendData[1]=delay_data[5];
                   
       }
       if(reg[2]!=0)
       {
          AutoSendData[2]=delay_data[12]; 
          AutoSendData[3]=delay_data[13];
          //Addr_data=reg[2];          
       }
       if(reg[3]!=0)
       {
          AutoSendData[4]=delay_data[20]; 
          AutoSendData[5]=delay_data[21];
          //Addr_data=reg[3];          
       }
       if(reg[4]!=0)
       {
          AutoSendData[6]=delay_data[28]; 
          AutoSendData[7]=delay_data[29];
          //Addr_data=reg[4];          
       }
       if(reg[5]!=0)
       {
          AutoSendData[8]=delay_data[36]; 
          AutoSendData[9]=delay_data[37];
          //Addr_data=reg[5];          
       }
       if(reg[6]!=0)
       {
          AutoSendData[10]=delay_data[44]; 
          AutoSendData[11]=delay_data[45];
          //Addr_data=reg[6];          
       }
       if(reg[7]!=0)
       {
          AutoSendData[12]=delay_data[51]; 
          AutoSendData[13]=delay_data[52];
          //Addr_data=reg[7];          
       }
       Send_NB();
       
     }

  }   
      
       memset(delay_data, 0, 100);
       lenthg=0;


  return data_O_reg;
}
unsigned char Protocol_485(unsigned char *data_S,unsigned char data_len)
{
 
  if(data_S[0]== 0x01)//从机地址
  { 
       reg[0]=data_S[1];  //类型      
       reg[1]=data_S[2];   //地址
    
     if(reg[0]==0x03)//read data
     { 
       Addr_data=0x01;
       if(reg[1]==0x02)
       {
          air[0]=data_S[3];//CO2 
          air[1]=data_S[4];
                   y=1;
       }
       else if(reg[1]==0x08)
       {
          senor[0]=data_S[3]; //传感器1土壤湿度
          senor[1]=data_S[4];
          senor[2]=data_S[5]; //传感器1土壤温度
          senor[3]=data_S[6];
          senor[4]=data_S[7]; //传感器1土壤湿度
          senor[5]=data_S[8];
          senor[6]=data_S[9]; //传感器1土壤温度
          senor[7]=data_S[10];
          //Addr_data=reg[2];
          m=1;
         
       }
       else if(reg[1]==0x07)
       {
          air[2]=data_S[3]; //空气湿度
          air[3]=data_S[4];
          air[4]=data_S[5]; //空气温度
          air[5]=data_S[6];
          air[7]=data_S[7];//光照强度
          air[8]=data_S[8]; 
          air[9]=data_S[9];
          //Addr_data=reg[3]; 
          x=1;
       }
       if(flag_add==1)
       {
         if(x==1&&y==1)
         {
           Send_NB1();
           x=y=0;
         }
         DelayMs(200);
         if(m==1)
         {
          Send_NB0(); 
          x=0;
         }
         flag_add=0;
       }
     }
       memset(data_S, 0, 100);

  }   
      

else data_O_reg=0;

  return data_O_reg;
}
void Send_PLC(void)
{
       u8 x;
       RS485_EN=1;//485发送模式
       PLC_EN=1;  //PLC发送模式
       reg_send_data[0]=addr;
       reg_send_data[1]=Q_write;
       reg_send_data[3]=PLC_Addr;
       reg_send_data[4]=PLC_Data_H;
       reg_send_data[5]=PLC_Data_L;
       CRC_temp=CRC16(reg_send_data,6,0);
       reg_send_data[6]=CRC_temp>>8;
       reg_send_data[7]=CRC_temp&0xff;
       for(x=0;x<8;x++)
            {
              USART_ClearITPendingBit(USART1, USART_IT_TC);
              USART_SendData(USART1, reg_send_data[x]);  
              while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
            }
}
void Send_NB0(void)
{
       u8 len,x;
       reg_rev_data[0]=0xa0;
       reg_rev_data[1]=0x00;
       reg_rev_data[2]=0x00;
       reg_rev_data[3]=0x02;
       reg_rev_data[4]=0x16;
       reg_rev_data[5]=0x03;
       reg_rev_data[6]=Addr_data;
       reg_rev_data[7]=senor[0];
       reg_rev_data[8]=senor[1];
       reg_rev_data[9]=Addr_data+1;
       reg_rev_data[10]=senor[2];
       reg_rev_data[11]=senor[3];
       reg_rev_data[12]=Addr_data+2;
       reg_rev_data[13]=senor[4];
       reg_rev_data[14]=senor[5];
       reg_rev_data[15]=Addr_data+3;
       reg_rev_data[16]=senor[6];
       reg_rev_data[17]=senor[7];
       reg_rev_data[18]=Addr_data+4;
       reg_rev_data[19]=senor[8];
       reg_rev_data[20]=senor[9];
       reg_rev_data[21]=Addr_data+5;
       reg_rev_data[22]=senor[10];
       reg_rev_data[23]=senor[11];
       reg_rev_data[24]=Addr_data+6;
       reg_rev_data[25]=senor[12];
       reg_rev_data[26]=senor[13];
       len=reg_rev_data[4]+6-1;
       reg_rev_data[27]=XOR(reg_rev_data,len);
       memset(senor, 0, 30);
       convertUnCharToStr((char *)NB_data,reg_rev_data,28);
       NB_data[56]=0x22;
       NB_data[57]=0x0d;
       NB_data[58]=0x0a;
       memcpy(&messg[26], NB_data, 59);
           for(x=0;x<85;x++)
            {
              USART_ClearITPendingBit(USART2, USART_IT_TC);
              USART_SendData(USART2, messg[x]);  
              while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
            }
           DelayMs(100);
           send_cmd(PUB,16);
}
void Send_NB1(void)
{
       u8 len,x;
       reg_rev_data[0]=0xa0;
       reg_rev_data[1]=0x00;
       reg_rev_data[2]=0x00;
       reg_rev_data[3]=0x01;
       reg_rev_data[4]=0x16;
       reg_rev_data[5]=0x03;
       reg_rev_data[6]=Addr_data;
       reg_rev_data[7]=air[0];
       reg_rev_data[8]=air[1];
       reg_rev_data[9]=Addr_data+1;
       reg_rev_data[10]=air[2];
       reg_rev_data[11]=air[3];
       reg_rev_data[12]=Addr_data+2;
       reg_rev_data[13]=air[4];
       reg_rev_data[14]=air[5];
       reg_rev_data[15]=Addr_data+3;
       reg_rev_data[16]=air[6];
       reg_rev_data[17]=air[7];
       reg_rev_data[18]=Addr_data+4;
       reg_rev_data[19]=air[8];
       reg_rev_data[20]=air[9];
       reg_rev_data[21]=Addr_data+5;
       reg_rev_data[22]=air[10];
       reg_rev_data[23]=air[11];
       reg_rev_data[24]=Addr_data+6;
       reg_rev_data[25]=air[12];
       reg_rev_data[26]=air[13];
       len=reg_rev_data[4]+6-1;
       reg_rev_data[27]=XOR(reg_rev_data,len);
       memset(air, 0, 50);
       convertUnCharToStr((char *)NB_data,reg_rev_data,28);
       NB_data[56]=0x22;
       NB_data[57]=0x0d;
       NB_data[58]=0x0a;
       memcpy(&messg[26], NB_data, 59);
           for(x=0;x<85;x++)
            {
              USART_ClearITPendingBit(USART2, USART_IT_TC);
              USART_SendData(USART2, messg[x]);  
              while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
            }
           DelayMs(200);
           send_cmd(PUB,16);
}
void Send_NB(void)
{
       u8 len,x;
       reg_rev_data[0]=0xa0;
       reg_rev_data[1]=0x00;
       reg_rev_data[2]=0x00;
       reg_rev_data[3]=0x01;
       reg_rev_data[4]=0x16;
       reg_rev_data[5]=0x01;
       reg_rev_data[6]=Addr_data;
       reg_rev_data[7]=AutoSendData[0];
       reg_rev_data[8]=AutoSendData[1];
       reg_rev_data[9]=Addr_data+1;
       reg_rev_data[10]=AutoSendData[2];
       reg_rev_data[11]=AutoSendData[3];
       reg_rev_data[12]=Addr_data+2;
       reg_rev_data[13]=AutoSendData[4];
       reg_rev_data[14]=AutoSendData[5];
       reg_rev_data[15]=Addr_data+3;
       reg_rev_data[16]=AutoSendData[6];
       reg_rev_data[17]=AutoSendData[7];
       reg_rev_data[18]=Addr_data+4;
       reg_rev_data[19]=AutoSendData[8];
       reg_rev_data[20]=AutoSendData[9];
       reg_rev_data[21]=Addr_data+5;
       reg_rev_data[22]=AutoSendData[10];
       reg_rev_data[23]=AutoSendData[11];
       reg_rev_data[24]=Addr_data+6;
       reg_rev_data[25]=AutoSendData[12];
       reg_rev_data[26]=AutoSendData[13];
       len=reg_rev_data[4]+6-1;
       reg_rev_data[27]=XOR(reg_rev_data,len);
       memset(AutoSendData, 0, 30);
       convertUnCharToStr((char *)NB_data,reg_rev_data,28);
       NB_data[56]=0x22;
       NB_data[57]=0x0d;
       NB_data[58]=0x0a;
       memcpy(&messg[26], NB_data, 59);
           for(x=0;x<85;x++)
            {
              USART_ClearITPendingBit(USART2, USART_IT_TC);
              USART_SendData(USART2, messg[x]);  
              while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
            }
           DelayMs(200);
           send_cmd(PUB,16);
}
void convertUnCharToStr(char* str, unsigned char* UnChar, int ucLen)  //数组转字符串
{  
    int i = 0;  
    for(i = 0; i < ucLen; i++)  
    {  
        //格式化输str,每unsigned char 转换字符占两位置%x写输%X写输  
        sprintf(str + i * 2, "%02x", UnChar[i]);  
    }  
} 

u8 HexToChar(u8 bChar)
{
    if((bChar>=0x30)&&(bChar<=0x39))
    bChar -= 0x30;
    else if((bChar>=0x41)&&(bChar<=0x46))//大写字母
    bChar -= 0x37;
    else if((bChar>=0x61)&&(bChar<=0x66))//小写字母
    bChar -= 0x57;
    else bChar = 0xff;
    return bChar;
}
void delay_PLC(unsigned int nDelay)
{
    unsigned int i,j,k;
    for ( i=0;i<nDelay;i++ )
    for ( j=0;j<6144;j++ )
    k++;
}