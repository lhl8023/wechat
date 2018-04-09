#ifndef	__MODBUS_H__
#define	__MODBUS_H__
#include "string.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "protocol.h"
#include "math.h"
//上位机到协议转换器通信格式
//字节1：协议转换器地址，字节2：控制器类型，字节3：控制命令代码，字节4：参数代码，字节5：参数值(W) 或 数据长度(R)，字节6、7：校验CRC16

#define	   IN_BAUD_RATE			57600//9600
#define	   OUT_BAUD_RATE			9600
//#define	   OUT_BAUD_RATE			4800

#define     ADDR_MODBUS_S        0x58
#define     ADDR_CONTROLER1      0xA1//空气净化器
#define     ADDR_CONTROLER2      0xA2//可耐福温控器
#define     ADDR_CONTROLER3      0xA3//新风控制
#define     ADDR_CONTROLER4      0xA4//扩展

#define     ADDR_CONTROLER5      0xC1//AirMeasure

#define     ADDR_MODBUS_M        0x01

#define		FUNCTION03_MASTER		0x03		//连续寄存器读出
#define		FUNCTION06_MASTER		0x06		//单独寄存器写入
#define		FUNCTION10_MASTER		0x10		//连续寄存器写入
#define		DEVICENOECHO_MASTER	0x88		//连续寄存器写入


#define		NUM_DATA_RECV_S		0x07
#define		NUM_DATA_RECV_M		0x10
#define		NUM_DEVICE_NOECHO		4000		//1表示0.5毫秒


typedef struct
{
    //length_recv代表了当前保存的是帧的第几个字节
    //FrameOK代表了一个完整的帧是否接收完成
    //recv数组是正在接收的帧保存的位置
    //num15end和num35end 代表了定时器计数超时的标志
    //num15start, num35start标志定时器可以开始计数了
    //num15 和 num35代表了定时器的计数值
    //g_endt15 为超时标志
    unsigned char length_recv, length_buf, g_endt15, g_endt35, FrameOK, num15start, num35start, Modbus_flag;
    unsigned int num15, num35, num15end, num35end;
    unsigned long baudrate;
    unsigned char recv[20], buf[20];
}MODBUS_STRUCT_SLAVER;

typedef struct
{
    unsigned char length_recv, length_buf, g_endt15, g_endt35, FrameOK, num15start, num35start, Modbus_flag;
    unsigned int num15, num35, num15end, num35end;
    unsigned long baudrate;
    unsigned char recv[45], buf[45];
}MODBUS_STRUCT_MASTER;

extern MODBUS_STRUCT_SLAVER modbusdata_slaver;
extern MODBUS_STRUCT_MASTER PM2_5_modbusdata_master;
extern MODBUS_STRUCT_MASTER VOC_modbusdata_master;
extern unsigned char flag_echo_master, flag_send_master, PM2_5_flag_echo_master, VOC_flag_echo_master;
extern unsigned char Data_ModBus_S[], Data_ModBus_M[];
extern unsigned char flag_DeviceNoEcho, Start_DeviceNoEcho;
extern unsigned char VOC_flag_DeviceNoEcho, VOC_Start_DeviceNoEcho;
extern unsigned int VOC_num_DeviceNoEcho;
extern unsigned char PM2_5_flag_DeviceNoEcho, PM2_5_Start_DeviceNoEcho;
extern unsigned int PM2_5_num_DeviceNoEcho;
extern unsigned int num_DeviceNoEcho;
extern unsigned char temp_send[];
extern unsigned int temp_crc16_code;

unsigned int CRC16(unsigned char *puchMsg, unsigned int usDataLen, unsigned char flag_code);
void ModBusSlaver_Init(void);
void StartCount15_Slaver(void);
void StartCount35_Slaver(void);
void T15EndHandle_Slaver(void);
void T35EndHandle_Slaver(void);
void ReceOneChar_Slaver(unsigned char ReceCharacter);
void MBus_Dispose_Slaver(void);
void ModBusMaster_Init(void);

void PM2_5_StartCount15_Master(void);
void PM2_5_StartCount35_Master(void);
void PM2_5_T15EndHandle_Master(void);
void PM2_5_T35EndHandle_Master(void);
void PM2_5_ReceOneChar_Master(unsigned char ReceCharacter);

void VOC_StartCount15_Master(void);
void VOC_StartCount35_Master(void);
void VOC_T15EndHandle_Master(void);
void VOC_T35EndHandle_Master(void);
void VOC_ReceOneChar_Master(unsigned char ReceCharacter);

void PM2_5_MBus_Dispose_Master(void);
void VOC_MBus_Dispose_Master(void);
void ADC_MBus_Dispose_Master(void);


void Device_NoEcho_Clear(void);

#endif
