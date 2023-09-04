#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   
 
//IO方向设置,参考CSDN有一个表
#define DHT11_IO_IN()  {GPIOA->CRL&=0XF0FFFFFF;GPIOA->CRL|=8<<24;}
#define DHT11_IO_OUT() {GPIOA->CRL&=0XF0FFFFFF;GPIOA->CRL|=3<<24;}

#define	DHT11_DQ_OUT PAout(6)
#define	DHT11_DQ_IN  PAin(6)  

#define RCC_GPIO_DHT11 	RCC_APB2Periph_GPIOA
#define PROT_DHT11 		GPIOA
#define PROT_PIN_DHT11 	GPIO_Pin_6

u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//读取温湿度
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11    
#endif
