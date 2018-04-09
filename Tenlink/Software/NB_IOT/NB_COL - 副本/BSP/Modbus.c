#include "Modbus.h"


// CRC 高位字节值表 
const unsigned char auchCRCHi[] = 
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;
// CRC低位字节值表
const unsigned char auchCRCLo[] = 
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

MODBUS_STRUCT_SLAVER modbusdata_slaver;
MODBUS_STRUCT_MASTER PLC_modbusdata_master;
K3_MODBUS_STRUCT_SLAVER K3_modbusdata_slaver;
RS485_MODBUS_STRUCT_SLAVER RS485_modbusdata_slaver;

unsigned char Data_ModBus_S[60], PLC_Data_ModBus_M[60], Data_ModBus_K3[60], Data_ModBus_RS485[60];
unsigned char uchCRCHi; //* 高CRC字节初始化 *//
unsigned char uchCRCLo; //* 低CRC 字节初始化 *//
unsigned int  code_crc;
unsigned char flag_echo_master=0, flag_send_master=0, PLC_flag_echo_master=0, RS485_flag_echo_master=0;
unsigned char flag_DeviceNoEcho=0, Start_DeviceNoEcho=0;
unsigned int  num_DeviceNoEcho=0;
unsigned char PLC_flag_DeviceNoEcho=0, PLC_Start_DeviceNoEcho=0,K3_flag_DeviceNoEcho=0, K3_Start_DeviceNoEcho=0,RS485_flag_DeviceNoEcho=0, RS485_Start_DeviceNoEcho=0;
unsigned int  PLC_num_DeviceNoEcho=0,K3_num_DeviceNoEcho=0,RS485_num_DeviceNoEcho=0;
unsigned char temp_send[20];
unsigned int  temp_crc16_code;
u8 again_flag=0;
int P_E=0;
u8 lenth=0,lenthg=0;
u8 c=0,d;
u8 PLC_data[100];
u8 delay_data[100];
u8 compare[50];
u8 data_lenth;
//功能:CRC校验
//参数: 要进行CRC校验的消息，消息中字节数
//返回值：CRC校验码,低位在前，高位在后，即高低位调换了
unsigned int CRC16(unsigned char *puchMsg, unsigned int usDataLen, unsigned char flag_code)
{
    //unsigned char uchCRCHi = 0xFF; 	/* 高CRC字节初始化 */
    //unsigned char uchCRCLo = 0xFF; 	/* 低CRC 字节初始化 */
    //unsigned int  code_crc;
    unsigned char uIndex; 				/* CRC循环中的索引 */
    
    if(flag_code == 0)
    {
   	uchCRCHi = 0xFF;
   	uchCRCLo = 0xFF;
    }   
    while (usDataLen--) 					/* 传输消息缓冲区 */
    {
        uIndex = uchCRCHi ^ *puchMsg++ ; /* 计算CRC */
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
        uchCRCLo = auchCRCLo[uIndex] ;
    }   
    code_crc = uchCRCHi;
    code_crc = (code_crc << 8) + uchCRCLo;	
    return code_crc;
}
//功能:异或和校验
//参数: 要进行异或和校验的消息，消息中字节数
//返回值：异或和校验码,两个字节的校验
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


//功能：ModBus Init
//参数：无
//返回值：无
void ModBusSlaver_Init(void)
{     
    memset(&modbusdata_slaver.length_recv, 0, sizeof(MODBUS_STRUCT_SLAVER));
    memset(Data_ModBus_S, 0, 60);
    
    modbusdata_slaver.baudrate = IN_BAUD_RATE;  
    modbusdata_slaver.num15end = 3*(9600/modbusdata_slaver.baudrate)+1;
    modbusdata_slaver.num35end = 7*(9600/modbusdata_slaver.baudrate) + 1;
    
    //定时器初始化
    TIM4_Init();
    //串口初始化							
    SZ_STM32_Uart2Init(modbusdata_slaver.baudrate);
}

//功能：1.5个字符计时开始
//参数：无
//返回值：无
void StartCount15_Slaver(void)
{
    modbusdata_slaver.num15 = 0;
    modbusdata_slaver.num15start = 1;
}

//功能：3.5个字符计时开始
//参数：无
//返回值：无
void StartCount35_Slaver(void)
{   
    modbusdata_slaver.num35 = 0;
    modbusdata_slaver.num35start = 1;
}

//功能：1.5个字符超时处理
//参数：无
//返回值：无

void T15EndHandle_Slaver(void)
{
 
  modbusdata_slaver.Modbus_flag = 1;
  if(modbusdata_slaver.g_endt15 == 1)
    return;
  modbusdata_slaver.g_endt15 = 1;
  if(modbusdata_slaver.recv[0]== 0x30)          //1、确认码
  {      
       REC_OK=1;
       if(c==0)
       {
           AT_CMD();
           c=1;
       }
       
  }
   else if(modbusdata_slaver.recv[0]== 0x0d)          //1、回车
  {      
    if(modbusdata_slaver.recv[1]== 0x0a)  //换行
    {
      if(modbusdata_slaver.recv[2]== 0x2B&&modbusdata_slaver.recv[3]== 0x54&&modbusdata_slaver.recv[4]== 0x4f)  
      {  

          modbusdata_slaver.length_buf=modbusdata_slaver.length_recv-76; //55 lhl,76  taiji
          modbusdata_slaver.FrameOK = 1; 
          memcpy(modbusdata_slaver.buf, &modbusdata_slaver.recv[76], modbusdata_slaver.length_buf);   
          

      }
       else if(modbusdata_slaver.recv[2]== 0x2B&&modbusdata_slaver.recv[3]== 0x4D&&modbusdata_slaver.recv[4]== 0x51)  
      {
            data_lenth=modbusdata_slaver.length_recv;
            memcpy(compare, &modbusdata_slaver.recv[2], data_lenth-4);
         if(strcmp(compare,call)== 0||strcmp(compare,open)== 0)
          {
              link();
            
          }
      }
      
    }

  }
    memset(compare, 0, 50);
    memset(modbusdata_slaver.recv, 0, 120);
    modbusdata_slaver.length_recv = 0;
}

//功能：3.5个字符超时处理
//参数：无
//返回值：无
void T35EndHandle_Slaver(void)
{
    if(modbusdata_slaver.g_endt15 == 0)
        return;
    modbusdata_slaver.g_endt15 = 0;
    modbusdata_slaver.Modbus_flag = 0;
}

//功能：收到一个字符处理
//参数：收到的字符
//返回值：无
void ReceOneChar_Slaver(unsigned char ReceCharacter)
{
    if(modbusdata_slaver.g_endt15 == 1)
    {
        StartCount35_Slaver();
        return;
    }
    modbusdata_slaver.FrameOK = 0;
    StartCount15_Slaver();
    StartCount35_Slaver();
    modbusdata_slaver.recv[modbusdata_slaver.length_recv++] = ReceCharacter;   
}

//功能：ModBus数据处理
//参数: 无
//返回值：无
void MBus_Dispose_Slaver(void)
{
    u8 i,n;
    u8 m=0;
    //unsigned char code_err, code_function;
  for(i=0;i<modbusdata_slaver.length_buf-2;i++)//字符转16进制
  {
    PLC_data[i]=HexToChar(modbusdata_slaver.buf[i]);
    
  }
  memset(modbusdata_slaver.buf, 0, 100);
  for(n=0;n<100;)//两两组合
  {
    modbusdata_slaver.buf[m]=(PLC_data[n]<<4)|PLC_data[n+1];
    d=PLC_data[n]<<4;
    n=n+2;
    m++;
  }
   
    Protocol_Data(modbusdata_slaver.buf,sizeof(modbusdata_slaver.buf)); //Data_ModBus_S为接收到的待处理数据，send为生成的缓存数据
    memset(modbusdata_slaver.buf, 0, 100);
    memset(PLC_data, 0, 100);
    return;
}

void Device_NoEcho_Clear(void)
{
    Start_DeviceNoEcho = 0;
    num_DeviceNoEcho = 0;
    flag_DeviceNoEcho = 0;
}

void PLC_Device_NoEcho_Clear(void)
{
    PLC_Start_DeviceNoEcho = 0;
    PLC_num_DeviceNoEcho = 0;
    PLC_flag_DeviceNoEcho = 0;
}


//功能：ModBus Init
//参数：无
//返回值：无
void ModBusMaster_Init(void)
{     
    memset(&PLC_modbusdata_master.length_recv, 0, sizeof(MODBUS_STRUCT_MASTER));
    memset(PLC_Data_ModBus_M, 0, 60);
    PLC_modbusdata_master.baudrate = OUT_BAUD_RATE;
    PLC_modbusdata_master.num15end = 3*(9600/PLC_modbusdata_master.baudrate) + 1;
    PLC_modbusdata_master.num35end = 7*(9600/PLC_modbusdata_master.baudrate) + 1;  
    	
    //定时器初始化
    TIM3_Init();
							
    SZ_STM32_Uart1Init(PLC_modbusdata_master.baudrate);
}
//功能：1.5个字符计时开始
//参数：无
//返回值：无
void StartCount15_master(void)
{
    PLC_modbusdata_master.num15 = 0;
    PLC_modbusdata_master.num15start = 1;
}

//功能：3.5个字符计时开始
//参数：无
//返回值：无
void StartCount35_master(void)
{   
    PLC_modbusdata_master.num35 = 0;
    PLC_modbusdata_master.num35start = 1;
}

//功能：1.5个字符超时处理
//参数：无
//返回值：无
void T15EndHandle_master(void)
{
    PLC_modbusdata_master.Modbus_flag = 1;
    if(PLC_modbusdata_master.g_endt15 == 1)
        return;
    PLC_modbusdata_master.g_endt15 = 1;
    if(PLC_modbusdata_master.recv[0] == ADDR_MODBUS_M)
    {    
       if(PLC_modbusdata_master.recv[1] == FUNCTION06_MASTER)
       { 

            PLC_modbusdata_master.FrameOK = 1;
            PLC_modbusdata_master.length_buf = PLC_modbusdata_master.length_recv-2;
            memcpy(PLC_modbusdata_master.buf, PLC_modbusdata_master.recv, PLC_modbusdata_master.length_buf);         
            
       }
    }
    memset(PLC_modbusdata_master.recv, 0, 60);
    PLC_modbusdata_master.length_recv = 0;
}

//功能：3.5个字符超时处理
//参数：无
//返回值：无
void T35EndHandle_master(void)
{
    if(PLC_modbusdata_master.g_endt15 == 0)
        return;
    PLC_modbusdata_master.g_endt15 = 0;
    PLC_modbusdata_master.Modbus_flag = 0;
}

//功能：收到一个字符处理
//参数：收到的字符
//返回值：无
void ReceOneChar_master(unsigned char ReceCharacter)
{
    if(PLC_modbusdata_master.g_endt15 == 1)
    {
        StartCount35_master();
        return;
    }
    PLC_modbusdata_master.FrameOK = 0;
    StartCount15_master();
    StartCount35_master();
    PLC_modbusdata_master.recv[PLC_modbusdata_master.length_recv++] = ReceCharacter; 
    delay_data[lenthg++]=ReceCharacter;
}


//功能：ModBus数据处理
//参数: 无
//返回值：无
void PLC_MBus_Dispose_master(void)
{
    unsigned int crc16_decode;
   //unsigned char send[20], code_err, code_function;
    crc16_decode = ((unsigned int)PLC_modbusdata_master.buf[PLC_modbusdata_master.length_buf-2]<<8) + PLC_modbusdata_master.buf[PLC_modbusdata_master.length_buf-1];   
    
    
        Protocol_PLC(PLC_modbusdata_master.buf,PLC_modbusdata_master.length_buf);
 

    memset(PLC_modbusdata_master.buf, 0, 60);
    return;
    
}

//功能：RS485-ModBus Init
//参数：无
//返回值：无
void RS485_ModBusSlaver_Init(void)
{     
    memset(&RS485_modbusdata_slaver.length_recv, 0, sizeof(RS485_MODBUS_STRUCT_SLAVER));
    memset(Data_ModBus_RS485, 0, 50);
    RS485_modbusdata_slaver.baudrate = OUT_BAUD_RATE;
    RS485_modbusdata_slaver.num15end = 3*(9600/RS485_modbusdata_slaver.baudrate) + 1;
    RS485_modbusdata_slaver.num35end = 7*(9600/RS485_modbusdata_slaver.baudrate) + 1;  
    	
    //定时器初始化
    TIM1_Init();
							
    SZ_STM32_Uart3Init(RS485_modbusdata_slaver.baudrate);
}
//功能：1.5个字符计时开始
//参数：无
//返回值：无
void RS485_StartCount15_slaver(void)
{
    RS485_modbusdata_slaver.num15 = 0;
    RS485_modbusdata_slaver.num15start = 1;
}

//功能：3.5个字符计时开始
//参数：无
//返回值：无
void RS485_StartCount35_slaver(void)
{   
    RS485_modbusdata_slaver.num35 = 0;
    RS485_modbusdata_slaver.num35start = 1;
}

//功能：1.5个字符超时处理
//参数：无
//返回值：无
void RS485_T15EndHandle_slaver(void)
{
    RS485_modbusdata_slaver.Modbus_flag = 1;
    if(RS485_modbusdata_slaver.g_endt15 == 1)
        return;
    RS485_modbusdata_slaver.g_endt15 = 1;
    if(RS485_modbusdata_slaver.recv[0] == ADDR_MODBUS_M)          //1、接收数据包头判定
      {
          if(RS485_modbusdata_slaver.recv[1] == FUNCTION03_MASTER)          //1、接收数据包头判定
          {
                  RS485_modbusdata_slaver.FrameOK = 1;
                  RS485_modbusdata_slaver.length_buf = RS485_modbusdata_slaver.length_recv; 
                  memcpy(RS485_modbusdata_slaver.buf, RS485_modbusdata_slaver.recv, RS485_modbusdata_slaver.length_buf);  
                  memcpy(RS485_modbusdata_slaver.arr, RS485_modbusdata_slaver.recv, RS485_modbusdata_slaver.length_buf); 
          }
   
      }
    memset(RS485_modbusdata_slaver.recv, 0, 60);
    RS485_modbusdata_slaver.length_recv = 0;
}

//功能：3.5个字符超时处理
//参数：无
//返回值：无
void RS485_T35EndHandle_slaver(void)
{
    if(RS485_modbusdata_slaver.g_endt15 == 0)
        return;
    RS485_modbusdata_slaver.g_endt15 = 0;
    RS485_modbusdata_slaver.Modbus_flag = 0;
}

//功能：收到一个字符处理
//参数：收到的字符
//返回值：无
void RS485_ReceOneChar_slaver(unsigned char ReceCharacter)
{
    if(RS485_modbusdata_slaver.g_endt15 == 1)
    {
        RS485_StartCount35_slaver();
        return;
    }
    RS485_modbusdata_slaver.FrameOK = 0;
    RS485_StartCount15_slaver();
    RS485_StartCount35_slaver();
    RS485_modbusdata_slaver.recv[RS485_modbusdata_slaver.length_recv++] = ReceCharacter;   
}

//功能：ModBus数据处理
//参数: 无
//返回值：无
void RS485_MBus_Dispose_slaver(void)
{
   
    
        Protocol_485(RS485_modbusdata_slaver.buf,RS485_modbusdata_slaver.length_buf);


    memset(RS485_modbusdata_slaver.buf, 0, 50);
    return;
    
}

void RS485_Device_NoEcho_Clear(void)
{
    RS485_Start_DeviceNoEcho = 0;
    RS485_num_DeviceNoEcho = 0;
    RS485_flag_DeviceNoEcho = 0;
}


