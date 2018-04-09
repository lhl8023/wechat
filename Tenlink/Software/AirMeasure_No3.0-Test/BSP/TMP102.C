#include "tmp102.h"

extern float temp_val_real_test;

/*************************************************************
  Function   ：SHT10_1_Dly()  
  Description：SHT10_1时序需要的延时
  Input      : none        
  return     : none    
*************************************************************/
void SHT10_1_Dly(void)
{
	u16 i;
	for(i = 1000; i > 0; i--);
}

/*************************************************************
  Function   ：SHT10_1_Config  
  Description：初始化 SHT10_1引脚
  Input      : none        
  return     : none    
*************************************************************/
void SHT10_1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;        
	//初始化SHT10_1引脚时钟
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );
   RCC_APB2PeriphClockCmd(SHT10_1_AHB2_CLK | RCC_APB2Periph_AFIO,ENABLE);
	       
	GPIO_InitStructure.GPIO_Pin = SHT10_1_DATA_PIN | SHT10_1_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SHT10_1_DATA_PORT, &GPIO_InitStructure);
	GPIO_Init(SHT10_1_SCK_PORT, &GPIO_InitStructure);
   Tmp102_Set(1);
}


/*************************************************************
  Function   ：SHT10_1_DATAOut
  Description：设置DATA引脚为输出
  Input      : none        
  return     : none    
*************************************************************/
void SHT10_1_DATAOut(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//PD0 DATA 推挽输出        
	GPIO_InitStructure.GPIO_Pin = SHT10_1_DATA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         
	GPIO_Init(SHT10_1_DATA_PORT, &GPIO_InitStructure);
}


/*************************************************************
  Function   ：SHT10_1_DATAIn  
  Description：设置DATA引脚为输入
  Input      : none        
  return     : none    
*************************************************************/
void SHT10_1_DATAIn(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//PD0 DATA 必须上拉  如果外面没有接上拉电阻需要配置为上拉输入        
	GPIO_InitStructure.GPIO_Pin = SHT10_1_DATA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(SHT10_1_DATA_PORT, &GPIO_InitStructure);
}


void I2C_Stop_1(void)
{
	SCL_LOW;
	SHT10_1_Dly();
	SDA_LOW;
	SHT10_1_Dly();
	SCL_HIGH;
	SHT10_1_Dly();
	SDA_HIGH;
	SHT10_1_Dly(); 
}

void I2C_Start_1(void)
{
	SDA_HIGH; //ensure data is high
	SHT10_1_Dly();
	SCL_HIGH;
	SHT10_1_Dly();
	SDA_LOW; //High ---> low transition
	SHT10_1_Dly();
	SCL_LOW;
}

unsigned char I2C_SendByte(unsigned char data)
{
	unsigned char i, ret;
	for(i=0; i<8; i++)
	{
		if((data<<i ) & 0x80)
		{
			SDA_HIGH; //bit to send
		}
		else
		{
			SDA_LOW;
		}
		SHT10_1_Dly();
		SCL_HIGH;
		SHT10_1_Dly();
		SCL_LOW;
		SHT10_1_Dly();
	}
	
	SDA_IN;
	SHT10_1_Dly();
	SCL_HIGH;	
	SHT10_1_Dly();
	if(SDA_Data)
	{ 
		ret =	0;
	}
	else
	{
		ret = 1;
	}	
	//while(SDA_Data);

	SHT10_1_Dly();	
	SCL_LOW;	
	SHT10_1_Dly();
	SDA_OUT;
	SHT10_1_Dly();
	SDA_HIGH;   
	return ret;
}

unsigned char I2C_ReadByte(void)
{
	unsigned char i, data=0;
	
	SDA_IN;
	SHT10_1_Dly();
	for(i = 0; i < 8; i++)
	{		
		SCL_LOW;
		SHT10_1_Dly();
		SCL_HIGH;
		SHT10_1_Dly();
		data = data << 1;
		if(SDA_Data)
			data |= 1;		
		SHT10_1_Dly();
		SCL_LOW;
	}
	SDA_OUT;
	return data;
}

void I2C_SendAck(unsigned char status)
{
	if(status == 0)
	{
		SDA_LOW;
	}
	else
	{
		SDA_HIGH; 
	}	
	SHT10_1_Dly();
	SCL_HIGH;
	SHT10_1_Dly();	
	SCL_LOW;
	SHT10_1_Dly();
}

//flag=1,正常；flag=0，低功耗
void Tmp102_Set(unsigned char flag)
{
	I2C_Start_1();	
	I2C_SendByte(0x90); //发送器件从地址（写）
	I2C_SendByte(0x01); //发送器件寄存器（配置）地址
	if(flag)
		I2C_SendByte(0x60); //配置寄存器的值 OS(0) R1(1) R0(1) F1(0) F0(0) POL(0) TM(0) SD(0)
	else
		I2C_SendByte(0x61); //配置寄存器的值 OS(0) R1(1) R0(1) F1(0) F0(0) POL(0) TM(0) SD(1)
	// 比较模式，低保持
	I2C_SendByte(0xa0); // CR1(1) CR0(0) AL(1) EM(0) 0 0 0 0
	//4Hz
	I2C_Stop_1();
}

float ReadTemp(void)
{
	unsigned char valh, vall;
	unsigned int data=0;
   signed char flag;
	float t_data;
	
	I2C_Start_1(); //启动I2C总线	
	I2C_SendByte(0x90); //发送器件从地址（写）
	I2C_SendByte(0x00); //发送器件温度寄存器子地址
	I2C_Start_1();
	I2C_SendByte(0x91); //发送器件从地址（读）
	valh = I2C_ReadByte(); //读温度高位
	I2C_SendAck(0);	
	vall = I2C_ReadByte(); //读温度低位
	I2C_SendAck(1);	
	I2C_Stop_1();
	
	data |= valh;
	data <<=8;
	data |= vall;
	data >>= 4;			
	if(data >= 0x0800)
	{
		flag = -1;
		data <<= 4;
		data = ~data;
		data >>= 4;
		data += 1;
	}
	else
	{
		flag = 1;
	}
	t_data = data*0.0625*flag;
	return t_data;
}

void Air_Measure_test(void)
{		
   temp_val_real_test = ReadTemp();			   
}

