#ifndef _MAIN_H
#define _MAIN_H

#include "delay.h"
#include "keyboard.h"
#include "Nixie_tube.h"
#include "Timer.h"
#include "IIC.h"

#define uchar 		unsigned char
#define uint 			unsigned int
#define uint16_t  unsigned short
	
#define SLAVE_ADDR		0xa0
#define E2PROM_ADDR_1 0x12
#define E2PROM_ADDR_2 0x13

typedef struct NumberT
{
	int input_index;				//�洢��������������Ҳ���������ʾ��
	int numbers[5];					//�洢��ǰ���������
}Number_T;

typedef struct LockT
{
	int Set_pwd_status;			//���������־λ
	int Nixie_tube_status;	//�Ƿ����ɹ��������ʾ��־λ
	int time_off_lock;			//���Ŀ�����رձ�־λ
	int alarm_flag;				����//������־λ
}Lock_Status_T;
	
sbit Lock			=P3^4;
sbit LED_Set	=P3^3;
sbit ALARM		=P3^5;

#define Lock_ON() 			  	(Lock=0)
#define Lock_OFF() 			  	(Lock=1)
#define LED_displayOFF()  	(P0=0xFF)

#define SETLED_ON() 				(LED_Set=0)
#define SETLED_OFF() 				(LED_Set=1)

#define ALARM_ON() 					(ALARM=0)
#define ALARM_OFF() 				(ALARM=1)
extern Lock_Status_T lock_T;
extern unsigned int cur_pwd[5];					//�洢��ǰ����
#endif 