#include "esp8266.h"

u16 PostMsgID=0;
				
char Params[Params_SIZE];               
char JsonBuf[JsonBuf_SIZE]; 
char Message[Message_SIZE];
				
void RST_ESP8266(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_GPIO_ESP, ENABLE);	 //使能PB,PE端口时钟

	GPIO_InitStructure.GPIO_Pin = PROT_PIN_ESP;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz

	GPIO_Init(PROT_ESP, &GPIO_InitStructure);
	
	ESP_RST = 0;
	delay_ms(1000);
	ESP_RST = 1;

	//RST will sizeover the BUFF,so use delay 
	//char cmd1[] = "AT+RESTORE";
	//U3_printf("%s\r\n", cmd1); //发送命令

	delay_ms(1000);delay_ms(1000);delay_ms(1000);delay_ms(1000);delay_ms(1000);
	//Now esp8266 is ready ,clean the buf
	USART3_RX_STA = 0;
	memset(USART3_RX_BUF, 0, USART3_MAX_RECV_LEN);
}

//向ESP8266发送命令
//cmd:发送的命令字符串;ack:期待的应答结果,如果为空,则表示不需要等待应答;waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果);1,发送失败
char ESP8266_Send_CMD(u8 *cmd, u8 *ack, u16 waittime)
{
	USART3_RX_STA = 0;	//Clean BUF
	memset(USART3_RX_BUF, 0, USART3_MAX_RECV_LEN);
	
	U3_printf("%s\r\n", cmd); //发送命令
	delay_ms(1);
	
	if (ack && waittime)	  //需要等待应答
	{
		while (--waittime) //等待倒计时
		{			
			delay_ms(10);
			
			if (USART3_RX_STA&0X8000) //接收到期待的应答结果
			{
				if (strstr((const char *)USART3_RX_BUF, (const char *)ack))
				{
					printf("CMD ACK Check OK\r\n");
					printf("----------------------------------------\r\n");
					printf("%s\r\n", (u8 *)USART3_RX_BUF);
					printf("----------------------------------------\r\n");
					
					USART3_RX_STA = 0;	//check ok,delet the buf 
					memset(USART3_RX_BUF, 0, USART3_MAX_RECV_LEN);
					
					break; //得到有效数据了。退出
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



//ESP8266模块和PC进入透传模式
void ESP8266_Init(void)
{	
	//----1让Wifi模块重启
	printf("########################################\r\n");
	printf("[Reset ESP8266:]\r\n");
	RST_ESP8266();
	printf("[Reset ESP8266 OK]\r\n");

	delay_ms(1000);delay_ms(1000);delay_ms(1000);
	
	//----2设置工作模式 		// 1：station模式   2：AP模式  3：兼容 AP+station模式
	printf("########################################\r\n");
	printf("[Set Mode:]\r\n");
	while (ESP8266_Send_CMD((u8*)CMD_2,(u8*)"OK", 100))  
	{
		delay_ms(10);
	}  printf("[Set Mode Success]\r\n");
	

	//让模块连接上自己的路由WIFI GOT IP
	printf("########################################\r\n");
	printf("[WIFI GOT IP:]\r\n");
	while (ESP8266_Send_CMD((u8*)CMD_3,(u8*)"WIFI GOT IP", 500))
	{
		delay_ms(1000);
	};
	printf("[WIFI GOT IP Success]\r\n");
	
	//4	让模块连接上时域
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





// 向onenet发送数据
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
	
	U3_printf("%s\r\n", Message); //发送命令
	
	while (--waittime) //等待倒计时
	{			
		delay_ms(10);
		
		if (USART3_RX_STA&0X8000) //接收到期待的应答结果
		{
			if (strstr((const char *)USART3_RX_BUF, (const char *)Topic_1_Reply))
			{
				printf("Publish-(%d) OK\r\n",(PostMsgID-1));
				printf("--------------------------------------------------\r\n");
				printf("%s", (u8 *)USART3_RX_BUF);
				printf("--------------------------------------------------\r\n");
		
				USART3_RX_STA = 0;	//check ok,delet the buf 
				memset(USART3_RX_BUF, 0, USART3_MAX_RECV_LEN);
				
				break; //得到有效数据了。退出
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
	
	if (USART3_RX_STA&0X8000) //接收到期待的应答结果
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


