#ifndef __ESP8266
#define __ESP8266

#include <stdlib.h>
#include <string.h>
#include "led.h"
#include "delay.h"

#include "usart.h"
#include "usart3.h"
#include "stm32f10x.h"
#include "sys.h"



#define ESP_RST PBout(5)// PB5

#define RCC_GPIO_ESP 	RCC_APB2Periph_GPIOB
#define PROT_ESP 		GPIOB
#define PROT_PIN_ESP 	GPIO_Pin_5


#define Params_SIZE		200
#define JsonBuf_SIZE 	500
#define Message_SIZE 	1000

#define WIFI_NAME		"666"
#define WIFI_PASSWORD	"66669999"	
/***********************************/
/*			MQTT 三元组		       */
/***********************************/
#define ProductKey		"i8j2A4bJx6S"
#define USERNAME		"DEV001&i8j2A4bJx6S"
#define PASSWARD		"F374C0B161F33AFEB87AB6844A554E9DE83A0E5C"
#define ClientID		"12345|securemode=3\\\,signmethod=hmacsha1\\\,timestamp=1692278795711|"

#define Topic_1			"/sys/i8j2A4bJx6S/DEV001/thing/event/property/post"
#define Topic_1_Reply	"/sys/i8j2A4bJx6S/DEV001/thing/event/property/post_reply"
#define Topic_2			"/sys/i8j2A4bJx6S/DEV001/thing/service/property/set"

#define POST_BODY_FORMAT "{\\\"id\\\": \\\"%d\\\"\\\,\\\"params\\\":%s }"

/***********************************/
/*			七步上云		       */
/***********************************/
#define CMD_1		"AT+RST"
#define CMD_2		"AT+CWMODE=1"
#define CMD_3		"AT+CWJAP=\"" WIFI_NAME "\",\"" WIFI_PASSWORD "\"" //宏定义的字符串拼接，多一个"
#define CMD_4		"AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\""
#define CMD_5		"AT+MQTTUSERCFG=0,1,\"\",\"" USERNAME "\",\"" PASSWARD "\",0,0,\"\""
#define CMD_6		"AT+MQTTCLIENTID=0,\"" ClientID "\""
#define CMD_7		"AT+MQTTCONN=0,\"" ProductKey ".iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,1"
#define SUB_Topic 	"AT+MQTTSUB=0,\"" Topic_1 "\",1"



extern float BOX_Temp;
extern float BOX_Humi;
extern unsigned char BOX_Adcx;
extern unsigned char IOT_SET_Flag;  


void RST_ESP8266(void);
char ESP8266_Send_CMD(u8 *cmd,u8 *ack,u16 waittime);
void ESP8266_Init(void);
char ESP8266_Pub_Data(void);
//void ESP8266_Rec_Data(void);
	

#endif
