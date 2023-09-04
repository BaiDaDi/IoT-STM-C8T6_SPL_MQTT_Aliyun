#include "stm32f10x.h"
#include "stdio.h"

#include "delay.h"
#include "usart.h"
#include "usart3.h"
#include "LED.h"
#include "dht11.h"
#include "lsens.h"
#include "esp8266.h"




float BOX_Temp = 1.32;
float BOX_Humi = 1.54;
unsigned char BOX_Adcx = 0;
 

int main(void)
{	
	unsigned char temp = 0;
	unsigned char humi = 0;

	SystemInit();//这样delay才准
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	
	delay_init();
	
	uart_init(9600); //U1作为日志打印串口
	uart3_init(115200); //U1作为日志打印串口

	LED_Init();
	DHT11_Init();//printf("DHT11: %d,0=ok!	\r\n",DHT11_Init());	
	Lsens_Init();
	
	
	printf("Init ESP8266\r\n");	
	ESP8266_Init();
	delay_ms(1000);
		
	while(1)
	{	
		//delay_ms(1000);LED = 0;
		//delay_ms(1000);LED = 1;
		
		DHT11_Read_Data(&temp,&humi);	
		BOX_Temp = (float)temp;
		BOX_Humi = (float)humi;
		
		BOX_Adcx=Lsens_Get_Val();
		
		ESP8266_Pub_Data();
		
		delay_ms(1000);delay_ms(1000);delay_ms(1000);
	}	
}
