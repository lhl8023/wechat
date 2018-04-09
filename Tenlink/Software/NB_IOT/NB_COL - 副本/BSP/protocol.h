#ifndef	__PROTOCOL_H__
#define	__PROTOCOL_H__

#define AutoSendTimeInterval  10//定义自动上报的时间间隔
#define SpyTimeInterval 1920*2
#define DisTimeInterval 960
#define SpyThreshold_Percent 		0.1
#define SpyThreshold_Data 			10


extern u8 REC_OK;
extern u32 AutoSendTime;
extern u8 AutoSendTimerOver;
extern u8 AutoSendOverTime;
extern u8 PLC_write_OK;
extern u8 flag_Tsensor;
extern u8 AutoSendFlag;     //自动发送标志
extern u8 AutoSendData[20];
extern u8 flag_add;
extern u8 open[];
extern u8 call[];
unsigned char Protocol_Data(unsigned char *data_S,unsigned char data_len);
//unsigned char Protocol_Zigbee(unsigned char *data_S,unsigned char data_len);
unsigned char Protocol_PLC(unsigned char *data_S,unsigned char data_len);
unsigned char Protocol_485(unsigned char *data_S,unsigned char data_len);
void link(void);
void send_cmd(u8 *data,u8 data_len);
void AT_CMD(void);
void Turn_up(void);
void Send_PLC(void);
void Send_NB(void);
void Send_NB0(void);
void Send_NB1(void);
u8 HexToChar(u8 bChar);
void convertUnCharToStr(char* str, unsigned char* UnChar, int ucLen) ;
void delay_PLC(unsigned int nDelay);
#endif
