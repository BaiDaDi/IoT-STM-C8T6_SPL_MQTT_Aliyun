#ifndef __USART3_H
#define __USART3_H


#include <stdarg.h>
#include <stdio.h>	 	 
#include <string.h>	 
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"

#define USART3_MAX_RECV_LEN 1024
#define USART3_MAX_SEND_LEN 1024

extern vu16 USART3_RX_STA;         		//接收状态标记
//USART3_RX_STA：
//15位：0x0A--换行的ascll码--接收到此数据就标志位置1
//14位：0x0D--回车的ascll码--接收到此数据就标志位置1
//13-0位：作为counter计数，最大可以2^14个

extern u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
extern u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节

void uart3_init(u32 bound);
void U3_printf(char* fmt,...);
void Uart3_SendStr(u8* SendBuf,u8 len);
//void send_data(u8 hongwai,u8 hujiao);
#endif
