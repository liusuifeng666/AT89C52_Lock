#ifndef	_USART_H
#define _USART_H
#include "REG52.h"

typedef unsigned char BYTE; //重定义字节类型

#define FOSC 11059200L      //宏定义时钟频率
#define BAUD 9600   				//宏定义波特率

	
void USART_Init(void);			//串口初始化程序
void SendData(BYTE dat);		//串口发送字节函数
void SendString(char *s);		//串口发送字符串函数

#endif