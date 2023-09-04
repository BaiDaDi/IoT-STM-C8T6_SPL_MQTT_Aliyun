#include "lsens.h"
#include "delay.h"
#include "adc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//LSENS(����������)��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/14 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
 
//��ʼ������������
void Lsens_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_GPIO_LSENS,ENABLE);//ʹ��PORTFʱ��	

	GPIO_InitStructure.GPIO_Pin = PROT_PIN_LSENS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(PROT_LSENS, &GPIO_InitStructure);	
	
	Adc1_Init();
}
//��ȡLight Sens��ֵ
//0~100:0,�;100,���� 
u8 Lsens_Get_Val(void)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val += Get_Adc1(ADC_CHx_LSENS);	//��ȡADCֵ
		delay_ms(5);
	}
	temp_val /= LSENS_READ_TIMES;//�õ�ƽ��ֵ 
	
	if(temp_val>4000)temp_val=4000;
	
	return temp_val;//(u8)(100-(temp_val/40));
}