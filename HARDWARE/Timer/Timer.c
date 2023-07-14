#include "Timer.h"
#include "main.h"
unsigned long alarm_time;
unsigned int alarm_flag_time;

unsigned long lock_off_time;
unsigned int time_off_lock_count;

void Timer_Init(void)
{
	TMOD = 0x01;
	
	TL0 = 0x66;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
	
	ET0 = 1;     //开定时器中断
	TR0 = 1;		 //启动定时器
	EA = 1;			 //开总中断
}

void timer_ISR() interrupt 1
{
	TL0 = 0x66;		
	TH0 = 0xFC;		
	if(lock_T.alarm_flag>=3)  //如果密码错误三次
	{
		alarm_time++;
		if(alarm_time>1000)			
		{
				ALARM=~ALARM;      //报警闪烁
				alarm_time=0;				
				alarm_flag_time++;	//累加时间
		}
	}
	if(lock_T.time_off_lock==1)		//如果进行开锁匹配后
	{
		lock_off_time++;
		if(lock_off_time>1000)
		{
				lock_off_time=0;				
				time_off_lock_count++;	//时间统计后消除锁状态
		}
	}
}
