#include "ir.h"
#include "delay.h"
#include "stdlib.h"
#include "stdio.h"
//***************************
/*天津睿曼智能科技有限公司
  2017/4/8
  pm2.5加红外版本
  修改人：liuhail
  修改内容：将光照接口替换成红外，使用io口PA11--scl，PA12--sda，PA8--busy
*/
//*****************************
struct _F F;
u32 ir_count = 0;
u8 ir_buf[IR_BUF_SIZE];
u32 ir_pos = 0;
u32 g_uart_index = 0;
u32 g_tx_len = 0;
u32 ir_mode = 0;
u8 f;
u8 gl;
u8 gh;

void ir_init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = GPIO_IR_CLK | GPIO_IR_DAT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =  GPIO_IR_BUSY ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	GPIO_Init(GPIOB, &GPIO_InitStructure) ; 
	GPIO_ResetBits(GPIOB, GPIO_IR_DAT);
  	GPIO_ResetBits(GPIOB, GPIO_IR_CLK);
}

void delay_nus(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=10;  
      while(i--) ;    
   }
}

void delay_nms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;  
      while(i--) ;    
   }
}

void
ir_open(void)
{	
	ir_data_output(); 
	ir_clock_hight();  
	ir_data_hight();
}

void
ir_close(void)   
{
	ir_data_output();  
	ir_clock_hight();  
	ir_data_hight();
}

void
ir_start(void) 
{
	ir_data_output(); 
	ir_clock_output(); 
	ir_clock_hight();  
	ir_data_hight();

	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	ir_data_low();
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	ir_clock_low();  
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
}


void
ir_stop(void)
{
	ir_data_output(); 
	ir_clock_output(); 
	ir_clock_low();  
	ir_data_low();
	DelayUs(DELAY_TIME_US);
	ir_clock_hight();
	DelayUs(DELAY_TIME_US);
	ir_data_hight();
	DelayUs(DELAY_TIME_US);  
}


u8
ir_check_ack(void) 
{
	u8 ACKSign;
	ir_data_input();
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	ir_clock_hight();
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	ACKSign = ir_data_status();
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	ir_clock_low();
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	return ACKSign;
}

void
ir_send_ack(void)
{
	ir_data_output();
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);

	ir_clock_hight();
	DelayUs(DELAY_TIME_US);
	ir_clock_low(); 
}

//for dat pin PA12
void 
ir_data_input(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_IR_DAT;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure) ; 
}
void 
ir_data_output(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_IR_DAT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void 
ir_data_low(void)
{
	GPIO_ResetBits(GPIOA, GPIO_IR_DAT);
}
void 
ir_data_hight(void)
{
	GPIO_SetBits(GPIOA, GPIO_IR_DAT);
}
u8
ir_data_status(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_IR_DAT);
}
//for clock pin PA11
void 
ir_clock_input(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_IR_CLK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	GPIO_Init(GPIOA, &GPIO_InitStructure) ; 
}
void 
ir_clock_output(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_IR_CLK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void 
ir_clock_low(void)
{
	GPIO_ResetBits(GPIOA, GPIO_IR_CLK);
}
void 
ir_clock_hight(void)
{
	GPIO_SetBits(GPIOA, GPIO_IR_CLK);
}
u8
ir_clock_status(void)
{
	return GPIO_ReadOutputDataBit(GPIOA, GPIO_IR_CLK);
}
//for busy pin PB5
void 
ir_busy_input(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_IR_BUSY;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	GPIO_Init(GPIOB, &GPIO_InitStructure) ; 
}
void 
ir_busy_output(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_IR_BUSY;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void 
ir_busy_low(void)
{
	GPIO_ResetBits(GPIOB, GPIO_IR_BUSY);
}
void 
ir_busy_hight(void)
{
	GPIO_SetBits(GPIOB, GPIO_IR_BUSY);
}
u8
ir_busy_status(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_IR_BUSY);
}
//Function
void 
ir_read_data(u8* data)
{
	u8 readdata = 0;
	u8 i = 8;
	ir_data_input();
	while (i--)
	{
		readdata <<= 1;
		ir_clock_hight(); 
		DelayUs(DELAY_TIME_US);
		if (ir_data_status() == BIT_HIGH){
			readdata |= 0x01;
		}
		else {
			readdata |= 0x00;
		}
		ir_clock_low();
		DelayUs(DELAY_TIME_US);
		DelayUs(DELAY_TIME_US);
	}
	ir_clock_low(); 
	DelayUs(DELAY_TIME_US);
	*data = readdata;
	ir_send_ack();
}
void
ir_write_data(const u8 data)
{
	u8 Data_Bit;
	int i;
	ir_data_output();
	ir_clock_low();
	DelayUs(DELAY_TIME_US);
	for(i = 7; i >= 0; i--)
	{
		DelayUs(DELAY_TIME_US);
		Data_Bit = (data >> i) & 0x01;
		if(Data_Bit)
		{
			ir_data_hight();
		}		
		else
		{
			ir_data_low();
		}
		DelayUs(DELAY_TIME_US);
		ir_clock_hight();
		DelayUs(DELAY_TIME_US);
		ir_clock_low();		
	}
	ir_check_ack();
}

int
read(u8 *buf, u8 count) 
{
	u8 i 	= 0;
	u8 len 	= 0;
	u8 ch;
	u8 checksum;
	if(count > UART_SIZE) 
	{
		len = UART_SIZE;
	}
	else
	{
		len = count;
	}
	if (ir_busy_status() == 0)
	{
		return 0;
	}
	ir_open();
	DelayUs(DELAY_TIME_US);
	ir_clock_low();
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	ir_clock_hight();
	DelayMs(DELAY_TIME_MS);
	ir_start();
	DelayUs(DELAY_TIME_US);
	ir_write_data(0x30);
	DelayUs(DELAY_TIME_US);
	if (ir_mode == 0x81)
	{
		ir_write_data(0x52);
	}
	else if (ir_mode == 0x82)
	{
		ir_write_data(0x62);
	}
	else {
		ir_write_data(0x52);
	}
	DelayUs(DELAY_TIME_US);
	ir_start();
	DelayUs(DELAY_TIME_US);
	ir_write_data(0x31);
	DelayUs(DELAY_TIME_US);
	ir_read_data(&ch);
	if(ch != 0x00)
	{	
		ir_stop();
		DelayUs(DELAY_TIME_US);
		ir_close();
		DelayUs(DELAY_TIME_US);
		return -1;
	}
	buf[0] = ch;
	if (ir_mode == 0x81)
	{
		checksum = 0xB3;
	}
	else if (ir_mode == 0x82)
	{
		checksum = 0xC3;
	}
	else{
		checksum = 0xB3;
	}
	for(i = 1; i < len; i++)
	{
		ir_read_data(&ch);
		DelayUs(DELAY_TIME_US);
		buf[i] = ch;
		checksum += ch;
	}
	ir_read_data(&ch);	
	DelayUs(DELAY_TIME_US);
	ir_stop();
	DelayUs(DELAY_TIME_US);
	ir_close();
	DelayUs(DELAY_TIME_US);

	if (ir_mode == 0x81)
	{
		if(ch != checksum)
		{
			return -1;
		}
	}
	return len;
}

int 
write(const u8 *buf, u8 count) 
{
	u16 i = 0;
	u16 len	= 0;
	if (count == 0){
		return 0;
	}
	if(count > UART_SIZE) 
	{
		len = UART_SIZE;
	}
	else
	{
		len = count;
	}
	if (buf[0] == 0x30 && buf[1] == 0x10 && buf[2] == 0x40)
	{
		ir_mode = 0x81;
	}
	else if (buf[0] == 0x30 && buf[1] == 0x20 && buf[2] == 0x50)
	{
		ir_mode = 0x82;
	}

	ir_open();
	DelayUs(DELAY_TIME_US);
	ir_clock_low();
	DelayUs(DELAY_TIME_US);
	DelayUs(DELAY_TIME_US);
	ir_clock_hight();

	DelayMs(DELAY_TIME_MS);
	ir_start();
	DelayUs(DELAY_TIME_US);
	for(i = 0; i < len; i++)
	{
		ir_write_data(buf[i]);
	}
	DelayUs(DELAY_TIME_US);
	ir_stop();
	DelayUs(DELAY_TIME_US);
	ir_close();
	DelayUs(DELAY_TIME_US);
	return len;
}


