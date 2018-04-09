#ifndef	__MODBUS_H__
#define	__MODBUS_H__
#include "string.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "protocol.h"
#include "math.h"
//��λ����Э��ת����ͨ�Ÿ�ʽ
//�ֽ�1��Э��ת������ַ���ֽ�2�����������ͣ��ֽ�3������������룬�ֽ�4���������룬�ֽ�5������ֵ(W) �� ���ݳ���(R)���ֽ�6��7��У��CRC16

#define	   IN_BAUD_RATE			9600
#define	   OUT_BAUD_RATE			9600
//#define	   OUT_BAUD_RATE			4800
#define     Header_UDP_Send      0xAA

#define     END_UDP_Send         0x7e

#define     Header_Zigbee_Send   0xA0

#define     ADDR_MODBUS_S        0x58

#define     ADDR_CONTROLER5      0xC1//AirMeasure

#define     ADDR_MODBUS_M        0x01

#define		FUNCTION03_MASTER		0x03		//�����Ĵ�������
#define		FUNCTION06_MASTER		0x06		//�����Ĵ���д��
#define		FUNCTION10_MASTER		0x10		//�����Ĵ���д��
#define		DEVICENOECHO_MASTER	        0x88		//�����Ĵ���д��

#define		NUM_DATA_RECV_L		0x07
#define		NUM_DATA_RECV_S		0x10
#define		NUM_DATA_RECV_M		0x10
#define		NUM_DEVICE_NOECHO		4000		//1��ʾ0.5����

    //length_recv�����˵�ǰ�������֡�ĵڼ����ֽ�
    //FrameOK������һ��������֡�Ƿ�������
    //recv���������ڽ��յ�֡�����λ��
    //num15end��num35end �����˶�ʱ��������ʱ�ı�־
    //num15start, num35start��־��ʱ�����Կ�ʼ������
    //num15 �� num35�����˶�ʱ���ļ���ֵ
    //g_endt15 Ϊ��ʱ��־
typedef struct
{
    unsigned char length_recv, length_buf, g_endt15, g_endt35, FrameOK, num15start, num35start, Modbus_flag;
    unsigned int num15, num35, num15end, num35end;
    unsigned long baudrate;
    unsigned char recv[120], buf[100],arr[60];
}MODBUS_STRUCT_SLAVER;

typedef struct
{
    unsigned char length_recv, length_buf, g_endt15, g_endt35, FrameOK, num15start, num35start, Modbus_flag;
    unsigned int num15, num35, num15end, num35end;
    unsigned long baudrate;
    unsigned char recv[60], buf[60],arr[60];
}K3_MODBUS_STRUCT_SLAVER;

typedef struct
{
    unsigned char length_recv, length_buf, g_endt15, g_endt35, FrameOK, num15start, num35start, Modbus_flag;
    unsigned int num15, num35, num15end, num35end;
    unsigned long baudrate;
    unsigned char recv[60], buf[60],arr[60];
}RS485_MODBUS_STRUCT_SLAVER;

typedef struct
{
    unsigned char length_recv, length_buf, g_endt15, g_endt35, FrameOK, num15start, num35start, Modbus_flag;
    unsigned int num15, num35, num15end, num35end;
    unsigned long baudrate;
    unsigned char recv[60], buf[60],arr[60];
}MODBUS_STRUCT_MASTER;

extern MODBUS_STRUCT_SLAVER modbusdata_slaver;
extern K3_MODBUS_STRUCT_SLAVER K3_modbusdata_slaver;
extern RS485_MODBUS_STRUCT_SLAVER RS485_modbusdata_slaver;
extern MODBUS_STRUCT_MASTER PLC_modbusdata_master;

extern unsigned char flag_echo_master, flag_send_master, PLC_flag_echo_master, RS485_flag_echo_master;
extern unsigned char Data_ModBus_S[], PLC_Data_ModBus_M[], Data_ModBus_K3[], Data_ModBus_RS485[];
extern unsigned char flag_DeviceNoEcho, Start_DeviceNoEcho;
extern unsigned char PLC_flag_DeviceNoEcho, PLC_Start_DeviceNoEcho,K3_flag_DeviceNoEcho, K3_Start_DeviceNoEcho,RS485_flag_DeviceNoEcho, RS485_Start_DeviceNoEcho;
extern unsigned int PLC_num_DeviceNoEcho,K3_num_DeviceNoEcho,RS485_num_DeviceNoEcho;
extern unsigned int num_DeviceNoEcho;
extern unsigned char temp_send[];
extern unsigned int temp_crc16_code;
extern u8 again_flag,lenthg;
extern u8 delay_data[];
extern u8 compare[];
extern u8 CSQ[];

unsigned int CRC16(unsigned char *puchMsg, unsigned int usDataLen, unsigned char flag_code);
unsigned char XOR(unsigned char *puchMsg, unsigned int usDataLen);
void ModBusSlaver_Init(void);
void StartCount15_Slaver(void);
void StartCount35_Slaver(void);
void T15EndHandle_Slaver(void);
void T35EndHandle_Slaver(void);
void ReceOneChar_Slaver(unsigned char ReceCharacter);
void MBus_Dispose_Slaver(void);



void RS485_ModBusSlaver_Init(void);
void RS485_StartCount15_slaver(void);
void RS485_StartCount35_slaver(void);
void RS485_T15EndHandle_slaver(void);
void RS485_T35EndHandle_slaver(void);
void RS485_ReceOneChar_slaver(unsigned char ReceCharacter);
void RS485_MBus_Dispose_slaver(void);

void ModBusMaster_Init(void);
void StartCount15_master(void);
void StartCount35_master(void);
void T15EndHandle_master(void);
void T35EndHandle_master(void);
void ReceOneChar_master(unsigned char ReceCharacter);
void PLC_MBus_Dispose_master(void);



void ADC_MBus_Dispose_Master(void);


void Device_NoEcho_Clear(void);

#endif
