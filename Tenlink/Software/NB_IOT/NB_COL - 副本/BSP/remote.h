#ifndef __IR_H_
#define __IR_H_


#include "sys.h"



struct _F{
     u8 bit0:1;
     u8 bit1:1;																		
     u8 bit2:1;
     u8 bit3:1;
     u8 bit4:1;
     u8 bit5:1;
     u8 bit6:1; 
     u8 bit7:1;
};

extern struct _F F;
#define F_IR_RX			(F.bit0)
#define F_IR_TX			(F.bit1)
#define F_IR_DO			(F.bit2)
#define F_IR_F			(F.bit7)


#define BIT_HIGH	1
#define BIT_LOW		0

//#define GPIO_IR_TX GPIO_Pin_0
//#define GPIO_IR_RX GPIO_Pin_1

#define GPIO_IR_CLK GPIO_Pin_11
#define GPIO_IR_DAT GPIO_Pin_12
#define GPIO_IR_BUSY GPIO_Pin_5

#define UART_SIZE	     255
#define IR_BUF_SIZE	   1500
#define DELAY_TIME_US  28
#define DELAY_TIME_MS  30



void ir_init(void);
void ir_open(void);
void ir_close(void);
void ir_start(void);
void ir_stop(void);
void ir_send_ack(void);
u8 ir_check_ack(void);
void ir_write_data(const u8 data);
void ir_read_data(u8* data);
//for dat pin
void ir_data_input(void);
void ir_data_output(void);
void ir_data_low(void);
void ir_data_hight(void);
u8 ir_data_status(void);
//for clk pin
void ir_clock_input(void);
void ir_clock_output(void);
void ir_clock_low(void);
void ir_clock_hight(void);
u8 ir_clock_status(void);
//for busy pin
void ir_busy_input(void);
void ir_busy_output(void);
void ir_busy_low(void);
void ir_busy_hight(void);
u8 gpio_busy_status(void);
//write, read
int write(const u8 *buf, u8 count);
int read(u8 *buf, u8 count);
void send_ir(u8 h, u8 l);
int compareMatch(const u8 *rev,int revLenth,const u8 *loc,int locLenth);
void delay_nus(u16 time);

void delay_nms(u16 time);
#endif

