#ifndef	__PROTOCOL_H__
#define	__PROTOCOL_H__

#define AutoSendTimeInterval  10//定义自动上报的时间间隔
#define SpyTimeInterval 1920*2
#define DisTimeInterval 960
#define SpyThreshold_Percent 		0.1
#define SpyThreshold_Data 			10



extern u32 AutoSendTime;
extern u8 AutoSendTimerOver;
extern u8 AutoSendOverTime;
extern u8 PLC_write_OK;
extern u8 flag_Tsensor;
extern u8 AutoSendFlag;     //自动发送标志
extern u8 AutoSendData[30];

unsigned char Protocol_Zigbee(unsigned char *data_S,unsigned char data_len);
unsigned char Protocol_PLC(unsigned char *data_S,unsigned char data_len);
void Send_PLC(void);
void Send_Zigbee(void);
void delay_PLC(unsigned int nDelay);
#endif
