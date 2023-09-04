#include "esp8266.h"

u16 PostMsgID=0;
				
char Params[Params_SIZE];               
char JsonBuf[JsonBuf_SIZE]; 
char Message[Message_SIZE];
				
void RST_ESP8266(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_GPIO_ESP, ENABLE);	 //ʹ��PB,PE�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = PROT_PIN_ESP;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz

	GPIO_Init(PROT_ESP, &GPIO_InitStructure);
	
	ESP_RST = 0;
	delay_ms(1000);
	ESP_RST = 1;

	//RST will sizeover the BUFF,so use delay 
	//char cmd1[] = "AT+RESTORE";
	//U3_printf("%s\r\n", cmd1); //��������

	delay_ms(1000);delay_ms(1000);delay_ms(1000);delay_ms(1000);delay_ms(1000);
	//Now esp8266 is ready ,clean the buf
	USART3_RX_STA = 0;
	memset(USART3_RX_BUF, 0, USART3_MAX_RECV_LEN);
}

//��ESP8266��������
//cmd:���͵������ַ���;ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��;waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����);1,����ʧ��
char ESP8266_Send_CMD(u8 *cmd, u8 *ack, u16 waittime)
{
	USART3_RX_STA = 0;	//Clean BUF
	memset(USART3_RX_BUF, 0, USART3_MAX_RECV_LEN);
	
	U3_printf("%s\r\n", cmd); //��������
	delay_ms(1);
	
	if (ack && waittime)	  //��Ҫ�ȴ�Ӧ��
	{
		while (--waittime) //�ȴ�����ʱ
		{			
			delay_ms(10);
			
			if (USART3_RX_STA&0X8000) //���յ��ڴ���Ӧ����
			{
				if (strstr((const char *)USART3_RX_BUF, (const char *)ack))
				{
					printf("CMD ACK Check OK\r\n");
					printf("----------------------------------------\r\n");
					printf("%s\r\n", (u8 *)USART3_RX_BUF);
					printf("----------------------------------------\r\n");
					
					USART3_RX_STA = 0;	//check ok,delet the buf 
					memset(USART3_RX_BUF, 0, USART3_MAX_RECV_LEN);
					
					break; //�õ���Ч�����ˡ��˳�
				}
				else
				{
					printf(">CMD ACK Check Failed:\r\n");
					printf(">******************************\r\n");
					printf("%s\r\n", (u8 *)USART3_RX_BUF);
					printf(">******************************\r\n");
					
					USART3_RX_STA = 0;	//retry again
					memset(USART3_RX_BUF, 0, USART3_MAX_RECV_LEN);
				}
			}
		}
		
		if (waittime == 0) return 1;
	}

	return 0;
}



//ESP8266ģ���PC����͸��ģʽ
void ESP8266_Init(void)
{	
	//----1��Wifiģ������
	printf("########################################\r\n");
	printf("[Reset ESP8266:]\r\n");
	RST_ESP8266();
	printf("[Reset ESP8266 OK]\r\n");

	delay_ms(1000);delay_ms(1000);delay_ms(1000);
	
	//----2���ù���ģʽ 		// 1��stationģʽ   2��APģʽ  3������ AP+stationģʽ
	printf("########################################\r\n");
	printf("[Set Mode:]\r\n");
	while (ESP8266_Send_CMD((u8*)CMD_2,(u8*)"OK", 100))  
	{
		delay_ms(10);
	}  printf("[Set Mode Success]\r\n");
	

	//��ģ���������Լ���·��WIFI GOT IP
	printf("########################################\r\n");
	printf("[WIFI GOT IP:]\r\n");
	while (ESP8266_Send_CMD((u8*)CMD_3,(u8*)"WIFI GOT IP", 500))
	{
		delay_ms(1000);
	};
	printf("[WIFI GOT IP Success]\r\n");
	
	//4	��ģ��������ʱ��
	printf("########################################\r\n");
	printf("[Connet NTP :]\r\n");
	while (ESP8266_Send_CMD((u8*)CMD_4,(u8*)"OK", 100))  
	{
		delay_ms(10);						
	}  printf("[Connet NTP Success\\r\n");
	
	//5
	printf("########################################\r\n");
	printf("[Set USERCFG :]\r\n");
	while (ESP8266_Send_CMD((u8*)CMD_5,(u8*)"OK", 100))  
	{
		delay_ms(10);				
	} printf("[Set USERCFG Success\\r\n");
	
	//6
	printf("########################################\r\n");
	printf("[Set CLIENTID:]\r\n"); 
	while (ESP8266_Send_CMD((u8*)CMD_6,(u8*)"OK", 100))  
	{
		delay_ms(10);				
	} printf("[Set CLIENTID Success]\r\n");
	
	//7
	printf("########################################\r\n");
	printf("[Connet IoT :]\r\n");
	if (ESP8266_Send_CMD((u8*)CMD_7,(u8*)"MQTTCONNECTED", 100))  
	{
		delay_ms(1000);
		printf("[Connet IoT Success]\r\n");		
	} 
	else printf("[Connet IoT ACK Check Failed]\\r\n");
	
	//8
	printf("########################################\r\n");
	printf("[Sub Topic  :]\r\n");
	while (ESP8266_Send_CMD((u8*)SUB_Topic,(u8*)"OK", 100))  
	{
		delay_ms(10);					
	} printf("[Sub Topic Success]\r\n");
	
}





// ��onenet��������
//reply have 3 '0x0a',so we need clean the buf and receive 3 times
//0:ok 1:failed
char ESP8266_Pub_Data(void)
{
	u8 i;
	u16 waittime = 400;
	
	memset(Params, 0, Params_SIZE);
	memset(JsonBuf, 0, JsonBuf_SIZE);
	memset(Message, 0, Message_SIZE);
	
	sprintf((char *)Params,"{\\\"LEDSwitch\\\":1\\\,\\\"Temperature\\\":%.2f\\\,\\\"Humidity\\\":%.2f}",BOX_Temp,BOX_Humi);			  
	sprintf((char *)JsonBuf,POST_BODY_FORMAT,PostMsgID,Params);
	PostMsgID += 1;
	sprintf((char *)Message,"AT+MQTTPUB=0,\""Topic_1"\",\"%s\",1,0",JsonBuf);
	
	USART3_RX_STA = 0;	//Clean BUF
	memset(USART3_RX_BUF, 0, USART3_MAX_RECV_LEN);
	
	U3_printf("%s\r\n", Message); //��������
	
	while (--waittime) //�ȴ�����ʱ
	{			
		delay_ms(10);
		
		if (USART3_RX_STA&0X8000) //���յ��ڴ���Ӧ����
		{
			if (strstr((const char *)USART3_RX_BUF, (const char *)Topic_1_Reply))
			{
				printf("Publish-(%d) OK\r\n",(PostMsgID-1));
				printf("--------------------------------------------------\r\n");
				printf("%s", (u8 *)USART3_RX_BUF);
				printf("--------------------------------------------------\r\n");
		
				USART3_RX_STA = 0;	//check ok,delet the buf 
				memset(USART3_RX_BUF, 0, USART3_MAX_RECV_LEN);
				
				break; //�õ���Ч�����ˡ��˳�
			}
			else
			{
				USART3_RX_STA = 0;	//check failed,delet the buf and try again!!!!
				memset(USART3_RX_BUF, 0, USART3_MAX_RECV_LEN);
				
				printf(">Publish-(%d) Failed, Retry\r\n",(PostMsgID-1));
			}
		}
	}
	
	if(waittime == 0) return 1;
	
	return 0;
}

/*
void ESP8266_Rec_Data(void)
{
	char *ret;
	char str[20];
	int  val;
	
	if (USART3_RX_STA&0X8000) //���յ��ڴ���Ӧ����
	{
		if(strstr((const char *)USART3_RX_BUF, (const char *)Topic_2))
		{
			printf("Find IoT CMD\r\n");
			printf("----------------------------------------\r\n");
			printf("%s\r\n", (u8 *)USART3_RX_BUF);
			printf("----------------------------------------\r\n");
			
			ret = strstr(USART3_RX_BUF,"LEDSwitch"); 
			
			if(ret!=NULL)
			{		
				str[0]=*(ret+12);                                                             //?powerswith????????str?(??1???????) 
				str[1]='\0';
				
				printf("This is Str : %s\r\n",str);
			
				val=atoi(str);                                                                //atoi?????????????????1??0
				printf("This is val : %d\r\n",val);                                           //??
			
				if(val==0)
				{
					LED = 0;
				}	
				else if(val==1)
				{
					LED = 1;
				}
			}
		}
		
		USART3_RX_STA = 0;	
		memset(USART3_RX_BUF, 0, USART3_MAX_RECV_LEN);
	}
}
*/


