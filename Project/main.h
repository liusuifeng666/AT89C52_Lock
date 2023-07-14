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
	int input_index;				//存储输入内容索引，也是数码管显示索
	int numbers[5];					//存储当前输入的内容
}Number_T;

typedef struct LockT
{
	int Set_pwd_status;			//设置密码标志位
	int Nixie_tube_status;	//是否开锁成功数码管显示标志位
	int time_off_lock;			//锁的开启与关闭标志位
	int alarm_flag;				··//报警标志位
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
extern unsigned int cur_pwd[5];					//存储当前密码
#endif 