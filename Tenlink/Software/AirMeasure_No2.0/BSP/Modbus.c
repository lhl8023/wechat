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
MODBUS_STRUCT_MASTER PM2_5_modbusdata_master;
MODBUS_STRUCT_MASTER VOC_modbusdata_master;

unsigned char Data_ModBus_S[20], PM2_5_Data_ModBus_M[45], VOC_Data_ModBus_M[15];
unsigned char uchCRCHi; //* 高CRC字节初始化 *//
unsigned char uchCRCLo; //* 低CRC 字节初始化 *//
unsigned int code_crc;
unsigned char flag_echo_master=0, flag_send_master=0, PM2_5_flag_echo_master=0, VOC_flag_echo_master=0;
unsigned char flag_DeviceNoEcho=0, Start_DeviceNoEcho=0;
unsigned int num_DeviceNoEcho=0;
unsigned char VOC_flag_DeviceNoEcho=0, VOC_Start_DeviceNoEcho=0;
unsigned int VOC_num_DeviceNoEcho=0;
unsigned char PM2_5_flag_DeviceNoEcho=0, PM2_5_Start_DeviceNoEcho=0;
unsigned int PM2_5_num_DeviceNoEcho=0;
unsigned char temp_send[50];
unsigned int temp_crc16_code;
int P_E=0;
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

//功能：ModBus Init
//参数：无
//返回值：无
void ModBusSlaver_Init(void)
{     
    memset(&modbusdata_slaver.length_recv, 0, sizeof(MODBUS_STRUCT_SLAVER));
    memset(Data_ModBus_S, 0, 20);
    
    modbusdata_slaver.baudrate = IN_BAUD_RATE;  
    modbusdata_slaver.num15end = 3*(9600/modbusdata_slaver.baudrate)+1;
    modbusdata_slaver.num35end = 7*(9600/modbusdata_slaver.baudrate) + 1;
    
    //定时器初始化
    TIM4_Init();
    //串口初始化							
    SZ_STM32_Uart2Init(IN_BAUD_RATE);
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
    if(modbusdata_slaver.recv[0] == ADDR_MODBUS_S)
    {      
        if(modbusdata_slaver.length_recv <= NUM_DATA_RECV_S)
        {
            modbusdata_slaver.FrameOK = 1;
            modbusdata_slaver.length_buf = modbusdata_slaver.length_recv;
            memcpy(modbusdata_slaver.buf, modbusdata_slaver.recv, modbusdata_slaver.length_buf);         
        }
    }
    memset(modbusdata_slaver.recv, 0, 20);
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
    unsigned int crc16_decode, type_decode, crc16_code;
    unsigned char send[20], code_err, code_function;
    
    type_decode = modbusdata_slaver.buf[1];
    code_function = modbusdata_slaver.buf[2];
    crc16_decode = ((unsigned int)modbusdata_slaver.buf[5]<<8) + modbusdata_slaver.buf[6];   
    
    if(CRC16(modbusdata_slaver.buf, 5, 0) == crc16_decode)    //CRC16校验正确
    {      
//        memcpy(Data_ModBus_S, &modbusdata_slaver.buf[1], 4);
//        if(type_decode == ADDR_CONTROLER1)			//空气净化器
//        {
//            Protocol_AirClear_S(send, Data_ModBus_S);
//        }
//        else if(type_decode == ADDR_CONTROLER2)	//可耐福
//        {
//            Protocol_TControl_KNF_S(send, Data_ModBus_S);
//        }
//        else if(type_decode == ADDR_CONTROLER3)	//新风
//        {
//            Protocol_FineWind_S(send, Data_ModBus_S);
//        }
//        else if(type_decode == ADDR_CONTROLER4)	//纳斯蓝净化器
//        {
//            Protocol_AirClearNSL_S(send, Data_ModBus_S);
//        }
//        else if(type_decode == ADDR_CONTROLER5)
//        {
//            Air_Measure(Data_ModBus_S);
//        }
    }
    else  //非法功能：01
    {
        code_err = 0x01;
        goto MBus_Err;         
    }
    memset(modbusdata_slaver.buf, 0, 20);
    return;
    
MBus_Err:
    send[0] = ADDR_MODBUS_S;
    send[1] = type_decode;
    send[2] = code_function+0x80;
    send[3] = modbusdata_slaver.buf[3];
    send[4] = code_err;
    crc16_code  = CRC16(send, 5, 0);
    send[5] = crc16_code >> 8;
    send[6] = crc16_code & 0xff;
    
    memcpy(SendData_Uart2, send, 7);
    Longth_Uart2Send = 7;
    flag_send_uart2 = 1;
    memset(modbusdata_slaver.buf, 0, 20);
}

void Device_NoEcho_Clear(void)
{
    Start_DeviceNoEcho = 0;
    num_DeviceNoEcho = 0;
    flag_DeviceNoEcho = 0;
}

void PM2_5_Device_NoEcho_Clear(void)
{
    PM2_5_Start_DeviceNoEcho = 0;
    PM2_5_num_DeviceNoEcho = 0;
    PM2_5_flag_DeviceNoEcho = 0;
}

void VOC_Device_NoEcho_Clear(void)
{
    VOC_Start_DeviceNoEcho = 0;
    VOC_num_DeviceNoEcho = 0;
    VOC_flag_DeviceNoEcho = 0;
}


//功能：ModBus Init
//参数：无
//返回值：无
void ModBusMaster_Init(void)
{     
    memset(&PM2_5_modbusdata_master.length_recv, 0, sizeof(MODBUS_STRUCT_MASTER));
    //memset(&VOC_modbusdata_master.length_recv, 0, sizeof(MODBUS_STRUCT_MASTER));
    memset(PM2_5_Data_ModBus_M, 0, 35);
    //memset(VOC_Data_ModBus_M, 0, 15);
    PM2_5_modbusdata_master.baudrate = OUT_BAUD_RATE;
    PM2_5_modbusdata_master.num15end = 3*(9600/PM2_5_modbusdata_master.baudrate) + 1;
    PM2_5_modbusdata_master.num35end = 7*(9600/PM2_5_modbusdata_master.baudrate) + 1; 
    
    //VOC_modbusdata_master.baudrate = OUT_BAUD_RATE;
    //VOC_modbusdata_master.num15end = 3*(9600/VOC_modbusdata_master.baudrate) + 1;
    //VOC_modbusdata_master.num35end = 7*(9600/VOC_modbusdata_master.baudrate) + 1; 
    	
    //定时器初始化
    TIM3_Init();
    //TIM2_Init();
    //串口初始化							
    SZ_STM32_Uart1Init(PM2_5_modbusdata_master.baudrate);
    //SZ_STM32_Uart3Init(VOC_modbusdata_master.baudrate);
}

//功能：1.5个字符计时开始
//参数：无
//返回值：无
void PM2_5_StartCount15_Master(void)
{
    PM2_5_modbusdata_master.num15 = 0;
    PM2_5_modbusdata_master.num15start = 1;
}
void VOC_StartCount15_Master(void)
{
    VOC_modbusdata_master.num15 = 0;
    VOC_modbusdata_master.num15start = 1;
}

//功能：3.5个字符计时开始
//参数：无
//返回值：无
void PM2_5_StartCount35_Master(void)
{   
    PM2_5_modbusdata_master.num35 = 0;
    PM2_5_modbusdata_master.num35start = 1;
}

void VOC_StartCount35_Master(void)
{   
    VOC_modbusdata_master.num35 = 0;
    VOC_modbusdata_master.num35start = 1;
}

//功能：1.5个字符超时处理
//参数：无
//返回值：无

void PM2_5_T15EndHandle_Master(void)
{
    PM2_5_modbusdata_master.Modbus_flag = 1;
    if(PM2_5_modbusdata_master.g_endt15 == 1)
        return;
    PM2_5_modbusdata_master.g_endt15 = 1;
    if(PM2_5_flag_echo_master == 1)
    {
        PM2_5_flag_echo_master = 0;
        PM2_5_modbusdata_master.length_buf = PM2_5_modbusdata_master.length_recv;
        memcpy(PM2_5_modbusdata_master.buf, PM2_5_modbusdata_master.recv, PM2_5_modbusdata_master.length_buf);         
        PM2_5_modbusdata_master.FrameOK = 1;
    }    
    memset(PM2_5_modbusdata_master.recv, 0, 20);
    PM2_5_modbusdata_master.length_recv = 0;
}

void VOC_T15EndHandle_Master(void)
{
    VOC_modbusdata_master.Modbus_flag = 1;
    if(VOC_modbusdata_master.g_endt15 == 1)
        return;
    VOC_modbusdata_master.g_endt15 = 1;
    if(VOC_flag_echo_master == 1)
    {
        VOC_flag_echo_master = 0;
        VOC_modbusdata_master.length_buf = VOC_modbusdata_master.length_recv;
        memcpy(VOC_modbusdata_master.buf, VOC_modbusdata_master.recv, VOC_modbusdata_master.length_buf);         
        VOC_modbusdata_master.FrameOK = 1;
    }
    memset(VOC_modbusdata_master.recv, 0, 20);
    VOC_modbusdata_master.length_recv = 0;
}

//功能：3.5个字符超时处理
//参数：无
//返回值：无
void PM2_5_T35EndHandle_Master(void)
{
    if(PM2_5_modbusdata_master.g_endt15 == 0)
        return;
    PM2_5_modbusdata_master.g_endt15 = 0;
    PM2_5_modbusdata_master.Modbus_flag = 0;
}

void VOC_T35EndHandle_Master(void)
{
    if(VOC_modbusdata_master.g_endt15 == 0)
        return;
    VOC_modbusdata_master.g_endt15 = 0;
    VOC_modbusdata_master.Modbus_flag = 0;
}

//功能：收到一个字符处理
//参数：收到的字符
//返回值：无
void PM2_5_ReceOneChar_Master(unsigned char ReceCharacter)
{
    if(PM2_5_modbusdata_master.g_endt15 == 1)
    {
        PM2_5_StartCount35_Master();
        return;
    }
    PM2_5_modbusdata_master.FrameOK = 0;
    PM2_5_StartCount15_Master();
    PM2_5_StartCount35_Master();
    PM2_5_modbusdata_master.recv[PM2_5_modbusdata_master.length_recv++] = ReceCharacter;   
}

void VOC_ReceOneChar_Master(unsigned char ReceCharacter)
{
    if(VOC_modbusdata_master.g_endt15 == 1)
    {
        PM2_5_StartCount35_Master();
        return;
    }
    VOC_modbusdata_master.FrameOK = 0;
    VOC_StartCount15_Master();
    VOC_StartCount35_Master();
    VOC_modbusdata_master.recv[VOC_modbusdata_master.length_recv++] = ReceCharacter;   
}

//功能：ModBus数据处理
//参数: 无
//返回值：无
u16 PM2_5_SUMCHECK(int length)
{
    int i = 0;
    u16 sumcheck = 0;
    for(;i<length-2;i++)
    {
        sumcheck += PM2_5_modbusdata_master.buf[i];
    }
    return sumcheck;
}

void PM2_5_MBus_Dispose_Master(void)
{
    u16 SumCode_PM2_5, data_temp;
    
    SumCode_PM2_5 = PM2_5_modbusdata_master.buf[PM2_5_modbusdata_master.length_buf-2];
    SumCode_PM2_5 = (SumCode_PM2_5<<8) + PM2_5_modbusdata_master.buf[PM2_5_modbusdata_master.length_buf-1];
    
    PM2_5_Device_NoEcho_Clear();
    if(PM2_5_modbusdata_master.buf[0]==0x42 && PM2_5_modbusdata_master.buf[1]==0x4D \
    		&& PM2_5_SUMCHECK(PM2_5_modbusdata_master.length_buf)==SumCode_PM2_5)
    {
        	/*下面为自动上报数据的程序*/        
        	data_temp = PM2_5_modbusdata_master.buf[6];
			data_temp = (data_temp<<8) + PM2_5_modbusdata_master.buf[7];
			
			//优<=50,良51-100,轻度101-150,中度151-200,重度201-300,严重>300
			/*if(PM2_5_Data <= 50)
			{
				if(fabs(data_temp-PM2_5_Data) >= SpyThreshold_Data)
				{
				    EmergencySendFlag |= 0x10;
				}
			}
			else
			{
                        if(fabs(data_temp-PM2_5_Data) >= PM2_5_Data*SpyThreshold_Percent)
				{
				    EmergencySendFlag |= 0x10;
				}
			}*/
			
			PM2_5_Data = data_temp;
			flag_Tsensor = 0x01;//不带温湿度传感器
			
			//甲醛数据，暂时没有该数据，补零
			Methanal_Data = 0;
    }
    else//校验错误  再要一次数据
    {
        PM2_5_flag_echo_master = 1;
    }
}

void VOC_MBus_Dispose_Master(void)
{
	int i=0, j=0, k=0;
	
	VOC_Device_NoEcho_Clear();
	if(VOC_modbusdata_master.buf[VOC_modbusdata_master.length_buf-2]==0x0D && VOC_modbusdata_master.buf[VOC_modbusdata_master.length_buf-1]==0x0A)
	{
		while(VOC_modbusdata_master.buf[i]!=0x20)
		{
			i++;
		}
		
		for(j=0; j<i; j++)
		{
		   k = k*10 + (VOC_modbusdata_master.buf[j]&0x0F);
		}
		
		if(fabs(k-VOC_Data)>(VOC_Data*SpyThreshold_Percent))
		{
		   EmergencySendFlag |= 0x01;//紧急上报
		}
                if(k>700)
		{
		   EmergencySendFlag |= 0x01;//紧急上报
		}
		
		VOC_Data = k;
		AutoSendData[4] = (VOC_Data>>8)&0x0FF;
		AutoSendData[5] = VOC_Data&0x0FF;
		AutoSendDataOK |= 0x01;//代表VOC的数据已经准备好
	}
	else//校验错误  再要一次数据
	{
		VOC_flag_echo_master = 1;
	}
}

void ADC_MBus_Dispose_Master()
{
    u16 crc_code;
    *SendData_Uart2 = ADDR_MODBUS_S;
    memcpy(SendData_Uart2 + 1, Data_ModBus_S, 4);
    //adc电压值 = AirData.adc * 3.3 / 4096
    SendData_Uart2[5] = (AirData.adc & 0xFF00)>>8;
    SendData_Uart2[6] = (AirData.adc & 0x00FF);
    crc_code = CRC16(SendData_Uart2, 7, 0);
    memcpy(SendData_Uart2 + 7, &crc_code, 2);   
    Longth_Uart2Send = 9;
    flag_send_uart2 = 1;
    ADC_DataOK = 0;
}


