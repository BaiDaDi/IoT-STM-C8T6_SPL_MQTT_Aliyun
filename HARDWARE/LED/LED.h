#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED PCout(13)// PB5

#define RCC_GPIO_LED 	RCC_APB2Periph_GPIOC
#define PROT_LED 		GPIOC
#define PROT_PIN_LED 	GPIO_Pin_13

void LED_Init(void);//��ʼ��
 				    
#endif
