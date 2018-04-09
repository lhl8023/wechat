/*************************************************************
                             \(^o^)/
  Copyright (C), 2013-2020, ZheJiang University of Technology
  File name  : SHT10.c 
  Author     : ziye334    
  Version    : V1.0 
  Data       : 2014/3/10      
  Description: Digital temperature and humidity sensor driver code
  
*************************************************************/
#include "iAQ-core.h"
#include <math.h>

u8 data_iCore[9];

u16 CO2_iCore=0, VOC_iCore=0;

void iCore_Dly(void)
{
	u16 i;
	for(i = 500; i > 0; i--);
}

void iCoreInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;        
	//初始化SHT10引脚时钟
	RCC_APB2PeriphClockCmd(iCore_AHB2_CLK | RCC_APB2Periph_AFIO,ENABLE);
	       
	GPIO_InitStructure.GPIO_Pin = iCore_DATA_PIN | iCore_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(iCore_DATA_PORT, &GPIO_InitStructure);	
	GPIO_Init(iCore_SCK_PORT, &GPIO_InitStructure);
	
	TWI_SDA_HIGH();
	TWI_SCL_HIGH();
}


/*************************************************************
  Description：设置DATA引脚为输出
  Input      : none        
  return     : none    
*************************************************************/
void iCore_DATAOut(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//PD0 DATA 推挽输出        
	GPIO_InitStructure.GPIO_Pin = iCore_DATA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         
	GPIO_Init(iCore_DATA_PORT, &GPIO_InitStructure);
}


/*************************************************************
  Description：设置DATA引脚为输入
  Input      : none        
  return     : none    
*************************************************************/
void iCore_DATAIn(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//PD0 DATA 必须上拉  如果外面没有接上拉电阻需要配置为上拉输入        
	GPIO_InitStructure.GPIO_Pin = iCore_DATA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(iCore_DATA_PORT, &GPIO_InitStructure);
}

/*************************************************************
  Description：设置DATA引脚为输出
  Input      : none        
  return     : none    
*************************************************************/
void iCore_SCKOut(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//PD0 DATA 推挽输出        
	GPIO_InitStructure.GPIO_Pin = iCore_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         
	GPIO_Init(iCore_SCK_PORT, &GPIO_InitStructure);
}


/*************************************************************
  Description：设置DATA引脚为输入
  Input      : none        
  return     : none    
*************************************************************/
void iCore_SCKIn(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//PD0 DATA 必须上拉  如果外面没有接上拉电阻需要配置为上拉输入        
	GPIO_InitStructure.GPIO_Pin = iCore_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(iCore_SCK_PORT, &GPIO_InitStructure);
}	 

static uint8_t I2C_Start(void)
{
	TWI_SDA_OUTPUT();
	TWI_DELAY();
	TWI_SDA_HIGH();
	TWI_DELAY();
	TWI_SCL_HIGH();
	TWI_DELAY();
	TWI_SDA_LOW();
	TWI_DELAY();
	TWI_SCL_LOW();
	TWI_DELAY();
	
	return true;
}

static void I2C_Stop(void)
{
	TWI_SDA_OUTPUT();
	TWI_DELAY();
	TWI_SCL_LOW();
	TWI_DELAY();
	TWI_SDA_LOW();
	TWI_DELAY();
	TWI_SCL_HIGH();
	TWI_DELAY();
	TWI_SDA_HIGH();
	TWI_DELAY();
}

static void I2C_Ack(void)
{
	TWI_SCL_LOW();
	TWI_DELAY();
	TWI_SDA_OUTPUT();
	TWI_SDA_LOW();
	TWI_DELAY();
	TWI_SCL_HIGH();
	TWI_DELAY();
	TWI_SCL_LOW();
	TWI_DELAY();
}

static void I2C_NoAck(void)
{
	TWI_SCL_LOW();
	TWI_DELAY();
	TWI_SDA_OUTPUT();
	TWI_SDA_HIGH();
	TWI_DELAY();
	TWI_SCL_HIGH();
	TWI_DELAY();
	TWI_SCL_LOW();
	TWI_DELAY();
}

static uint8_t I2C_WaitAck(void)
{
	uint16_t timeout = 0;
	
	TWI_SCL_LOW();
	TWI_DELAY();
	TWI_SDA_INPUT();	
	TWI_DELAY();
	TWI_SCL_HIGH();
	TWI_DELAY();
	
	while(TWI_SDA_READ())
	{
		timeout++;
		if(timeout > 250)
		{
			I2C_Stop();
			return false;
		}
		TWI_DELAY();
	}
	TWI_SCL_LOW();
	
	return true;
}

static void I2C_SendByte(uint8_t byte)
{
	uint8_t i = 8;
	TWI_SDA_OUTPUT();
	TWI_DELAY();
	TWI_SCL_LOW();
	TWI_DELAY();
	while (i--) 
	{
		if (byte & 0x80)
			TWI_SDA_HIGH();
		else
		   TWI_SDA_LOW();
		TWI_DELAY();
		byte <<= 1;
		TWI_SCL_HIGH();
		TWI_DELAY();
		TWI_SCL_LOW();
		TWI_DELAY();
	}
}

/************************iAQ-core read start**************************/
#define READ_SCL_IO		1

static bool iAQ_I2C_ReceiveByte(uint8_t *byte, bool ack)
{
	uint8_t i = 8;
	
	TWI_SDA_INPUT();	
	TWI_DELAY();
	while (i--) 
	{
		*byte <<= 1;
		TWI_SCL_LOW();
		TWI_DELAY();
		TWI_SCL_HIGH();
		TWI_DELAY();
		
		#if READ_SCL_IO
		{	
			uint16_t timeout = 0;
			TWI_SCL_INPUT();
			while(TWI_SCL_READ()==0)
			{
				TWI_DELAY();
				timeout++;
				if(timeout > 1000)
				{
					TWI_SCL_OUTPUT();
					return false;
				}
			}
			TWI_SCL_OUTPUT();
		}
		#endif
				
		if (TWI_SDA_READ()) 
		{
			*byte |= 0x01;
		}
	}
	
	if(ack)
		I2C_Ack();
	else
		I2C_NoAck();
	
	return true;
}
/************************iAQ-core read end**************************/

#define I2C_ACK		1
#define I2C_NAK		0 

void IAQRead(uint8_t *s)
{
	uint8_t i;
	uint8_t error = true;
	
	I2C_Start();
	I2C_SendByte(0xb5);
	I2C_WaitAck();
	
	for(i = 0; i < 8; i++)
	{   
		error = iAQ_I2C_ReceiveByte(&s[i],I2C_ACK);
		if(error == false) 
			goto read_error;
	}
	error = iAQ_I2C_ReceiveByte(&s[i],I2C_NAK);
	if(s[2] == 0x00)
	{
		CO2_iCore = s[0];
		CO2_iCore <<= 8;
		CO2_iCore += s[1];
		
		VOC_iCore = s[7];
		VOC_iCore <<= 8;
		VOC_iCore += s[8];		
	}
	
read_error:		
	I2C_Stop();
}