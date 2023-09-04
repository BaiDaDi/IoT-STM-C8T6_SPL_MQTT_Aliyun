#ifndef __LSENS_H
#define __LSENS_H	
#include "sys.h" 
#include "adc.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//LSENS(����������)��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved										  
//////////////////////////////////////////////////////////////////////////////////
  
#define RCC_GPIO_LSENS 	RCC_APB2Periph_GPIOA
#define PROT_LSENS 		GPIOA
#define PROT_PIN_LSENS 	GPIO_Pin_1
#define ADC_CHx_LSENS	ADC_Channel_1	//����������������ڵ�ADCͨ�����

#define LSENS_READ_TIMES	5		//���������������ȡ����,����ô���,Ȼ��ȡƽ��ֵ


void Lsens_Init(void); 				//��ʼ������������
u8 Lsens_Get_Val(void);				//��ȡ������������ֵ
#endif 





















