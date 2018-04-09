#include "Modbus.h"
#include "string.h"
#include "usart.h"
#include "protocol.h"
#include "sys.h" 
#include "delay.h"
#include "led.h"
unsigned char flag_type=0, flag_function=0,flag_addr=0,echo_code_s=0;
u8 PLC_write_OK=0;

u8 AutoSendTimerOver;
u32 AutoSendTime;     //自动发送时间
u8 AutoSendFlag;     //自动发送标志 
u8 addr=0x01,Q_write=0x06,Q_read=0x03;    //从机地址，线圈寄存器状态的读、写            
u8 AutoSendOverTime;  //自动发送超时时间
u8 PLC_Data_H,PLC_Data_L,PLC_Addr;         //PLC数据、地址
u8 AutoSendData[30]; // 自动发送数据缓存区
u8 reg[10];
u8 find[10]={0x01,0x03,0x00,0x01,0x00,0x08,0x15,0xCC};
u8 reg_data[10];
u8 reg_send_data[10];
u8 reg_rev_data[35];
u16 CRC_temp,CRC_date;
u8 LENTH,Addr_data,add;
unsigned char data_O_reg = 0;//向上位机返回数据的寄存器长度(温控系统协议)
//data_O—生成的对从机数据
//data_S—待处理协议数据
//返回值：data_O_reg_len 执行结果，成功=返回1，失败=返回0
unsigned char Protocol_Zigbee(unsigned char *data_S,unsigned char data_len)
{
  LENTH=data_len;
  u8 x;
  if(data_S[0]== 0x01)//DO写控制
  {   
        add=data_S[1];
        find[3]=add;
       reg[0]=data_S[1];  //寄存器存储
       reg[1]=data_S[4];
       reg[2]=data_S[7];
       reg[3]=data_S[10];
       reg[4]=data_S[13];
       reg[5]=data_S[16];
       reg[6]=data_S[19];
       reg[7]=data_S[22];
       
       if(reg[0]!=0)
       {
         PLC_Data_H=data_S[2];   //寄存器存储高字节数据
         PLC_Data_L=data_S[3];  //寄存器存储低字节数据
         PLC_Addr=reg[0];
         Send_PLC();  
       }
      if(reg[1]!=0)
       {
         
         PLC_Data_H=data_S[5];   //寄存器存储高字节数据
         PLC_Data_L=data_S[6];  //寄存器存储低字节数据    
         PLC_Addr=reg[1];
         Send_PLC();
  
       }
       if(reg[2]!=0)
       {
         
         PLC_Data_H=data_S[8];   //寄存器存储高字节数据
         PLC_Data_L=data_S[9];  //寄存器存储低字节数据
         PLC_Addr=reg[2];
         Send_PLC();
       }
       if(reg[3]!=0)
       {
        
         PLC_Data_H=data_S[11];   //寄存器存储高字节数据
         PLC_Data_L=data_S[12];  //寄存器存储低字节数据
         PLC_Addr=reg[3];
         Send_PLC();
       }
       if(reg[4]!=0)
       {
         
         PLC_Data_H=data_S[14];   //寄存器存储高字节数据
         PLC_Data_L=data_S[15];  //寄存器存储低字节数据
         PLC_Addr=reg[4];
         Send_PLC();
       }
       if(reg[5]!=0)
       {
         
         PLC_Data_H=data_S[17];   //寄存器存储高字节数据
         PLC_Data_L=data_S[18];  //寄存器存储低字节数据
         PLC_Addr=reg[5];
         Send_PLC();
       }
       if(reg[6]!=0)
       {
     
         PLC_Data_H=data_S[20];   //寄存器存储高字节数据
         PLC_Data_L=data_S[21];  //寄存器存储低字节数据
         PLC_Addr=reg[6];
         Send_PLC();
       }
       if(reg[7]!=0)
       {
        
         PLC_Data_H=data_S[23];   //寄存器存储高字节数据
         PLC_Data_L=data_S[24];  //寄存器存储低字节数据
         PLC_Addr=reg[7];
         Send_PLC();
       }
       DelayMs(500);
       CRC_date=CRC16(find,6,0);
       find[6]=CRC_date>>8;
       find[7]=CRC_date&0xff;
         for(x=0;x<8;x++)
            {
              USART_ClearITPendingBit(USART1, USART_IT_TC);
              USART_SendData(USART1, find[x]);  
              while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
            }     
       
  }

else data_O_reg=0;

  return data_O_reg;
}
unsigned char Protocol_PLC(unsigned char *data_S,unsigned char data_len)
{
 
         
       memcpy(AutoSendData, &data_S[3], data_len); 
         
       Addr_data=reg[0];
       Send_Zigbee();
      

  return data_O_reg;
}
void Send_PLC(void)
{
       u8 x;
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
       DelayMs(100);
}
void Send_Zigbee(void)
{
       u8 x,len;
       reg_rev_data[0]=0xa0;
       reg_rev_data[1]=0x00;
       reg_rev_data[2]=0x00;
       reg_rev_data[3]=0x00;
       reg_rev_data[4]=0x19;
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
       reg_rev_data[27]=Addr_data+7;
       reg_rev_data[28]=AutoSendData[14];
       reg_rev_data[29]=AutoSendData[15];
       len=reg_rev_data[4]+6-1;
       reg_rev_data[30]=XOR(reg_rev_data,len);
       memset(AutoSendData, 0, 30);
       for(x=0;x<31;x++)
            {
              USART_ClearITPendingBit(USART2, USART_IT_TC);
              USART_SendData(USART2, reg_rev_data[x]);  
              while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
            }
   
}
void delay_PLC(unsigned int nDelay)
{
    unsigned int i,j,k;
    for ( i=0;i<nDelay;i++ )
    for ( j=0;j<6144;j++ )
    k++;
}