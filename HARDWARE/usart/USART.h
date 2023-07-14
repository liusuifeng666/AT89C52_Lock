#ifndef	_USART_H
#define _USART_H
#include "REG52.h"

typedef unsigned char BYTE; //�ض����ֽ�����

#define FOSC 11059200L      //�궨��ʱ��Ƶ��
#define BAUD 9600   				//�궨�岨����

	
void USART_Init(void);			//���ڳ�ʼ������
void SendData(BYTE dat);		//���ڷ����ֽں���
void SendString(char *s);		//���ڷ����ַ�������

#endif