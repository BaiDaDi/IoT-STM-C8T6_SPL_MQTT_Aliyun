#ifndef __LSENS_H
#define __LSENS_H	
#include "sys.h" 
#include "adc.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//LSENS(光敏传感器)驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved										  
//////////////////////////////////////////////////////////////////////////////////
  
#define RCC_GPIO_LSENS 	RCC_APB2Periph_GPIOA
#define PROT_LSENS 		GPIOA
#define PROT_PIN_LSENS 	GPIO_Pin_1
#define ADC_CHx_LSENS	ADC_Channel_1	//定义光敏传感器所在的ADC通道编号

#define LSENS_READ_TIMES	5		//定义光敏传感器读取次数,读这么多次,然后取平均值


void Lsens_Init(void); 				//初始化光敏传感器
u8 Lsens_Get_Val(void);				//读取光敏传感器的值
#endif 





















