#ifndef	__PROTOCOL_H__
#define	__PROTOCOL_H__

#define AutoSendTimeInterval  10//定义自动上报的时间间隔
#define SpyTimeInterval 1920*2
#define DisTimeInterval 960
#define SpyThreshold_Percent 		0.1
#define SpyThreshold_Data 			10

typedef struct
{    
    u16 Temp, Humidity;
    u16 adc;
    u8 PM2_5[35];
    u8 VOC[15];
}AirStruct;

extern u8 ADC_DataOK;
extern u8 AutoSendDataOK;
extern u8 AutoSendFlag;
extern u8 EmergencySendFlag;
extern u32 AutoSendTime;
extern u8 AutoSendTimerOver;
extern u8 AutoSendOverTime;
extern unsigned char flag_type;
extern AirStruct AirData;

extern u8 flag_Tsensor;
extern u16 PM2_5_Data, VOC_Data, Methanal_Data;
extern float Temperature_Data, Humidity_Data;
extern u8 AutoSendData[20];

void Protocol_AirClear_S(unsigned char* data_O, unsigned char* data_S);
void Protocol_TControl_KNF_S(unsigned char* data_O, unsigned char* data_S);
void Protocol_FineWind_S(unsigned char* data_O, unsigned char* data_S);
void Protocol_AirClearNSL_S(unsigned char* data_O, unsigned char* data_S);
void Protocol_AirClear_M(unsigned char* data_S);
void Protocol_TControl_KNF_M(unsigned char* data_S);
void Protocol_FineWind_M(unsigned char* data_S);
void Protocol_AirLClearNSL_M(unsigned char* data_S);
void Air_Measure(void);

#endif
