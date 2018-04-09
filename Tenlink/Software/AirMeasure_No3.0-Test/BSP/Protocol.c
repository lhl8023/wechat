#include "Modbus.h"
#include "string.h"
#include "usart.h"
#include "protocol.h"
#include "SHT10.h"

unsigned char flag_type=0, flag_function=0;
AirStruct AirData;
u8 ADC_DataOK = 0;
u8 AutoSendFlag = 0;//标志是否自动上报
u8 AutoSendDataOK = 0;
u8 EmergencySendFlag = 0;
u8 AutoSendTimerOver = 0;
u8 AutoSendOverTime = 0;
u32 AutoSendTime = 0;//自动上报的计数值

u8 flag_Tsensor=0;
u16 PM2_5_Data=0, VOC_Data=0, Methanal_Data;
float Temperature_Data=0.0, Humidity_Data=0.0;
u8 AutoSendData[20]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


	u16 humi_val, temp_val;
	u8 err=0, checksum=0;
	float humi_val_real=0.0, temp_val_real=0.0, dew_point=0.0; 
   
   u16 humi_val_test, temp_val_test, temp_val_test_temp;
	u8 err_test=0, checksum_test=0;
	float humi_val_real_test=0.0, temp_val_real_test=0.0, dew_point_test=0.0;

//空气净化器
void Protocol_AirClear_S(unsigned char* data_O, unsigned char* data_S)
{
   unsigned char code_function, addr_decode, data_decode;
	
	flag_type 		= data_S[0];
	flag_function	= data_S[1];
	code_function 	= data_S[1];
	addr_decode 	= data_S[2];
	data_decode 	= data_S[3];
	
	
	if(code_function == FUNCTION06_MASTER)
	{		
		switch(addr_decode)
		{
			case 0x00:		//状态
				if(data_decode == 0)				//关机
				{
					data_O[2] = 0x01;
   				data_O[3] = 0x02;
				}
				else if(data_decode == 1)		//开机
				{
					data_O[2] = 0x01;
   				data_O[3] = 0x01;
				}
				flag_send_master = 1;						
				break;
				
			case 0x02:		//风机档位
				if(data_decode == 0)				//关闭风机
				{
					data_O[2] = 0x02;
   				data_O[3] = 0xff;
				}
				else if(data_decode == 1)		//低风
				{
					data_O[2] = 0x02;
   				data_O[3] = 0x01;
				}
				else if(data_decode == 2)		//中风
				{
					data_O[2] = 0x02;
   				data_O[3] = 0x02;
				}
				else if(data_decode == 3)		//高风
				{
					data_O[2] = 0x02;
   				data_O[3] = 0x03;
				}
				else if(data_decode == 4)		//大风
				{
					data_O[2] = 0x02;
   				data_O[3] = 0x04;
				}	
				else if(data_decode == 5)		//自动
				{
					data_O[2] = 0x03;
   				data_O[3] = 0x02;
				}
				else if(data_decode == 6)		//关闭自动
				{
					data_O[2] = 0x03;
   				data_O[3] = 0x03;
				}
				flag_send_master = 2;						
				break;
				
			default :
				break;
		}
   }
	else if(code_function == FUNCTION03_MASTER)
	{		
		switch(addr_decode)
		{				
			case 0x87:		//读PM2.5
				data_O[2] = 0x03;
   			data_O[3] = 0x01;	
   			flag_send_master = 3;		
				break;
			
			default :
				break;
		}
	}
	data_O[0] = 0x54;
	data_O[1] = 0x57;
	memcpy(SendData_Uart1, data_O, 4);
	Longth_Uart1Send = 4;
	flag_send_uart1 = 1;
	flag_echo_master = 1;
}

//可耐福温控器
void Protocol_TControl_KNF_S(unsigned char* data_O, unsigned char* data_S)
{
	unsigned int crc16_code;
   unsigned char code_function, addr_decode, data_decode;
	
	flag_type 		= data_S[0];
	flag_function	= data_S[1];
	code_function 	= data_S[1];
	addr_decode 	= data_S[2];
   data_decode 	= data_S[3];
	
	if(code_function == FUNCTION06_MASTER)
	{		
		switch(addr_decode)
		{
			case 0x00:		//状态
				if(data_decode == 0)				//关机
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x00;
				}
				else if(data_decode == 1)		//开机
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x01;
				}
				data_O[2] = 0x00;
   			data_O[3] = 1;	
   			flag_send_master = 1;					
				break;
			
			case 0x01:		//模式
				if(data_decode == 1)				//制冷
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x00;
				}
				else if(data_decode == 2)		//制热
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x01;
				}
				data_O[2] = 0x00;
   			data_O[3] = 2;	
   			flag_send_master = 2;					
				break;	
			
			case 0x03:		//温度设定
				data_O[2] = 0x00;
   			data_O[3] = 3;
   			data_O[4] = 0x00;
   			data_O[5] = data_decode;
   			flag_send_master = 3;						
				break;
					
			case 0x04:		//湿度设定
				data_O[2] = 0x00;
   			data_O[3] = 8;
   			data_O[4] = 0x00;
   			data_O[5] = data_decode;	
   			flag_send_master = 4;					
				break;
				
			default :
				break;
		}
	}
	else if(code_function == FUNCTION03_MASTER)
	{		
		switch(addr_decode)
		{				
			case 0x83:		//读实测温度
				data_O[2] = 0x00;
   			data_O[3] = 13;	
   			data_O[4] = 0x00;
				data_O[5] = 0x01;	
				flag_send_master = 5;	
				break;
			case 0x84:		//读实测湿度
				data_O[2] = 0x00;
   			data_O[3] = 12;	
   			data_O[4] = 0x00;
				data_O[5] = 0x01;	
				flag_send_master = 6;	
				break;
			case 0x85:		//读露点温度
				data_O[2] = 0x00;
   			data_O[3] = 14;	
   			data_O[4] = 0x00;
				data_O[5] = 0x01;		
				flag_send_master = 7;
				break;
         case 0x86:		//读外置温度值
				data_O[2] = 0x00;
   			data_O[3] = 17;	
   			data_O[4] = 0x00;
				data_O[5] = 0x01;		
				flag_send_master = 8;
				break;		
			default :
				break;
		}
	}
	data_O[0] = ADDR_MODBUS_M;
	data_O[1] = code_function;
	crc16_code = CRC16(data_O, 6, 0);
   data_O[6] = crc16_code >> 8;
   data_O[7] = crc16_code & 0xff;
	memcpy(SendData_Uart1, data_O, 8);
	Longth_Uart1Send = 8;
	flag_send_uart1 = 1;
	flag_echo_master = 1;
}

//新风控制
void Protocol_FineWind_S(unsigned char* data_O, unsigned char* data_S)
{
	unsigned int crc16_code;
   unsigned char code_function, addr_decode, data_decode;
	
	flag_type 		= data_S[0];
	flag_function	= data_S[1];
	code_function 	= data_S[1];
	addr_decode 	= data_S[2];
   data_decode 	= data_S[3];
	
	if(code_function == FUNCTION06_MASTER)
	{		
		switch(addr_decode)
		{
			case 0x00:		//状态
				if(data_decode == 0)				//关机
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x00;
				}
				else if(data_decode == 1)		//开机
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x01;
				}
				data_O[2] = 0x00;
   			data_O[3] = 1;	
   			flag_send_master = 1;					
				break;
				
			default :
				break;
		}
	}
	else if(code_function == FUNCTION03_MASTER)
	{		
		switch(addr_decode)
		{				
			case 0x83:		//读实测温度
				data_O[2] = 0x00;
   			data_O[3] = 2;	
   			data_O[4] = 0x00;
				data_O[5] = 0x01;	
				flag_send_master = 5;	
				break;
			case 0x84:		//读实测湿度
				data_O[2] = 0x00;
   			data_O[3] = 3;	
   			data_O[4] = 0x00;
				data_O[5] = 0x01;	
				flag_send_master = 6;	
				break;
         case 0x87:		//读PM2.5值
				data_O[2] = 0x00;
   			data_O[3] = 4;	
   			data_O[4] = 0x00;
				data_O[5] = 0x01;		
				flag_send_master = 7;
				break;		
			default :
				break;
		}
	}
	data_O[0] = ADDR_MODBUS_M;
	data_O[1] = code_function;
	crc16_code = CRC16(data_O, 6, 0);
   data_O[6] = crc16_code >> 8;
   data_O[7] = crc16_code & 0xff;
	memcpy(SendData_Uart1, data_O, 8);
	Longth_Uart1Send = 8;
	flag_send_uart1 = 1;
	flag_echo_master = 1;
}

//纳斯蓝空气净化器
void Protocol_AirClearNSL_S(unsigned char* data_O, unsigned char* data_S)
{
	unsigned int crc16_code;
   unsigned char code_function, addr_decode, data_decode;
	
	flag_type 		= data_S[0];
	flag_function	= data_S[1];
	code_function 	= data_S[1];
	addr_decode 	= data_S[2];
   data_decode 	= data_S[3];
	
	if(code_function == FUNCTION06_MASTER)
	{		
		switch(addr_decode)
		{
			case 0x00:		//状态
				if(data_decode == 0)				//关机
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x00;
				}
				else if(data_decode == 1)		//开机
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x01;
				}
				data_O[2] = 0x00;
   			data_O[3] = 4;	
   			flag_send_master = 1;					
				break;
			case 0x02:		//风机档位
				if(data_decode == 1)				//低风
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x03;
				}
				else if(data_decode == 2)		//中风
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x02;
				}
				else if(data_decode == 3)		//高风
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x01;
				}
				data_O[2] = 0x00;
   			data_O[3] = 5;	
				flag_send_master = 2;						
				break;
			case 0x05:		//风阀
				if(data_decode == 0)				//关闭
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x00;
				}
				else if(data_decode == 1)		//开机
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x01;
				}
				data_O[2] = 0x00;
   			data_O[3] = 6;	
   			flag_send_master = 3;					
				break;
			case 0x06:		//光触媒
				if(data_decode == 0)				//关闭
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x00;
				}
				else if(data_decode == 1)		//开机
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x01;
				}
				data_O[2] = 0x00;
   			data_O[3] = 7;	
   			flag_send_master = 4;					
				break;
			case 0x07:		//离子
				if(data_decode == 0)				//关闭
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x00;
				}
				else if(data_decode == 1)		//开机
				{
					data_O[4] = 0x00;
   				data_O[5] = 0x01;
				}
				data_O[2] = 0x00;
   			data_O[3] = 8;	
   			flag_send_master = 5;					
				break;
				
			default :
				break;
		}
	}
	else if(code_function == FUNCTION03_MASTER)
	{		
		switch(addr_decode)
		{				
			case 0x83:		//读实测温度
				
				break;
				
			default :
				break;
		}
	}
	data_O[0] = ADDR_MODBUS_M;
	data_O[1] = code_function;
	crc16_code = CRC16(data_O, 6, 0);
   data_O[6] = crc16_code >> 8;
   data_O[7] = crc16_code & 0xff;
	memcpy(SendData_Uart1, data_O, 8);
	Longth_Uart1Send = 8;
	flag_send_uart1 = 1;
	flag_echo_master = 1;
}

//空气净化器
void Protocol_AirClear_M(unsigned char* data_S)
{
   unsigned int crc16_code, data_PM25;
	unsigned char send[20];
	
	if(flag_function == FUNCTION06_MASTER)
	{		
		Device_NoEcho_Clear();
		
		send[0] = ADDR_MODBUS_S;
		send[1] = ADDR_CONTROLER1;
		if(data_S[0]=='O' && data_S[1]=='K')		//控制成功
		{
			send[2] = FUNCTION06_MASTER;
		}
		else
		{
			send[2] = FUNCTION06_MASTER+0x80;
		}
		send[3] = Data_ModBus_S[2];
		send[4] = Data_ModBus_S[3];
		crc16_code  = CRC16(send, 5, 0);
   	send[5] = crc16_code >> 8;
   	send[6] = crc16_code & 0xff;
		memcpy(SendData_Uart2, send, 7);
   	Longth_Uart2Send = 7;
   	flag_send_uart2 = 1;		
   }
	else if(flag_function == FUNCTION03_MASTER)
	{				
		Device_NoEcho_Clear();
		
		send[0] = ADDR_MODBUS_S;
		send[1] = ADDR_CONTROLER1;
		send[2] = FUNCTION03_MASTER;
		send[3] = Data_ModBus_S[2];
		if(flag_send_master == 3)
		{
			data_PM25 = data_S[0]-0x30;
			data_PM25 = data_PM25*10 + data_S[1]-0x30;
			data_PM25 = data_PM25*10 + data_S[2]-0x30;
			send[4] = data_PM25>>8;
			send[5] = data_PM25 & 0xff;
		}
		crc16_code  = CRC16(send, 6, 0);
   	send[6] = crc16_code >> 8;
   	send[7] = crc16_code & 0xff;
		memcpy(SendData_Uart2, send, 8);
   	Longth_Uart2Send = 8;
   	flag_send_uart2 = 1;
   	flag_echo_master = 0;
	}
}

//可耐福温控器
void Protocol_TControl_KNF_M(unsigned char* data_S)
{
	unsigned int crc16_code;
	unsigned char send[20];
	
	if(flag_function == FUNCTION06_MASTER)
	{		
		Device_NoEcho_Clear();
		
		send[0] = ADDR_MODBUS_S;
		send[1] = ADDR_CONTROLER2;
		if(data_S[0] == FUNCTION06_MASTER)		//正常
		{
			send[2] = FUNCTION06_MASTER;
		}
		else		//错误
		{
			send[2] = FUNCTION06_MASTER+0x80;
		}
		send[3] = Data_ModBus_S[2];
		send[4] = Data_ModBus_S[3];
		crc16_code  = CRC16(send, 5, 0);
   	send[5] = crc16_code >> 8;
   	send[6] = crc16_code & 0xff;
		memcpy(SendData_Uart2, send, 7);
   	Longth_Uart2Send = 7;
   	flag_send_uart2 = 1;
   	flag_echo_master = 0;
	}
	else if(flag_function == FUNCTION03_MASTER)
	{		
		Device_NoEcho_Clear();
		
		send[0] = ADDR_MODBUS_S;
		send[1] = ADDR_CONTROLER2;
		send[2] = FUNCTION03_MASTER;
		send[3] = Data_ModBus_S[2];
		send[4] = data_S[2];
		send[5] = data_S[3];	
		crc16_code  = CRC16(send, 6, 0);
   	send[6] = crc16_code >> 8;
   	send[7] = crc16_code & 0xff;
		memcpy(SendData_Uart2, send, 8);
   	Longth_Uart2Send = 8;
   	flag_send_uart2 = 1;
   	flag_echo_master = 0;
	}
}

//新风控制
void Protocol_FineWind_M(unsigned char* data_S)
{
	unsigned int crc16_code;
	unsigned char send[20];
	
	if(flag_function == FUNCTION06_MASTER)
	{		
		Device_NoEcho_Clear();
		
		send[0] = ADDR_MODBUS_S;
		send[1] = ADDR_CONTROLER3;
		if(data_S[0] == FUNCTION06_MASTER)		//正常
		{
			send[2] = FUNCTION06_MASTER;
		}
		else		//错误
		{
			send[2] = FUNCTION06_MASTER+0x80;
		}
		send[3] = Data_ModBus_S[2];
		send[4] = Data_ModBus_S[3];
		crc16_code  = CRC16(send, 5, 0);
   	send[5] = crc16_code >> 8;
   	send[6] = crc16_code & 0xff;
		memcpy(SendData_Uart2, send, 7);
   	Longth_Uart2Send = 7;
   	flag_send_uart2 = 1;
   	flag_echo_master = 0;
	}
	else if(flag_function == FUNCTION03_MASTER)
	{		
		Device_NoEcho_Clear();
		
		send[0] = ADDR_MODBUS_S;
		send[1] = ADDR_CONTROLER3;
		send[2] = FUNCTION03_MASTER;
		send[3] = Data_ModBus_S[2];
		send[4] = data_S[2];
		send[5] = data_S[3];	
		crc16_code  = CRC16(send, 6, 0);
   	send[6] = crc16_code >> 8;
   	send[7] = crc16_code & 0xff;
		memcpy(SendData_Uart2, send, 8);
   	Longth_Uart2Send = 8;
   	flag_send_uart2 = 1;
   	flag_echo_master = 0;
	}
}

//纳斯蓝空气净化器
void Protocol_AirLClearNSL_M(unsigned char* data_S)
{
	unsigned int crc16_code;
	unsigned char send[20];
	
	if(flag_function == FUNCTION06_MASTER)
	{		
		Device_NoEcho_Clear();
		
		send[0] = ADDR_MODBUS_S;
		send[1] = ADDR_CONTROLER4;
		if(data_S[0] == FUNCTION06_MASTER)		//正常
		{
			send[2] = FUNCTION06_MASTER;
		}
		else		//错误
		{
			send[2] = FUNCTION06_MASTER+0x80;
		}
		send[3] = Data_ModBus_S[2];
		send[4] = Data_ModBus_S[3];
		crc16_code  = CRC16(send, 5, 0);
   	send[5] = crc16_code >> 8;
   	send[6] = crc16_code & 0xff;
		memcpy(SendData_Uart2, send, 7);
   	Longth_Uart2Send = 7;
   	flag_send_uart2 = 1;
   	flag_echo_master = 0;
	}
	else if(flag_function == FUNCTION03_MASTER)
	{		
		Device_NoEcho_Clear();
		
	}
}
