#include "LED.h"

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_GPIO_LED, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = PROT_PIN_LED;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	
 GPIO_Init(PROT_LED, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(PROT_LED,PROT_PIN_LED);						 //PB.5 输出高

}
