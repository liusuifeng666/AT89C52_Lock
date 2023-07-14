#include "Timer.h"
#include "main.h"
unsigned long alarm_time;
unsigned int alarm_flag_time;

unsigned long lock_off_time;
unsigned int time_off_lock_count;

void Timer_Init(void)
{
	TMOD = 0x01;
	
	TL0 = 0x66;		//���ö�ʱ��ʼֵ
	TH0 = 0xFC;		//���ö�ʱ��ʼֵ
	
	ET0 = 1;     //����ʱ���ж�
	TR0 = 1;		 //������ʱ��
	EA = 1;			 //�����ж�
}

void timer_ISR() interrupt 1
{
	TL0 = 0x66;		
	TH0 = 0xFC;		
	if(lock_T.alarm_flag>=3)  //��������������
	{
		alarm_time++;
		if(alarm_time>1000)			
		{
				ALARM=~ALARM;      //������˸
				alarm_time=0;				
				alarm_flag_time++;	//�ۼ�ʱ��
		}
	}
	if(lock_T.time_off_lock==1)		//������п���ƥ���
	{
		lock_off_time++;
		if(lock_off_time>1000)
		{
				lock_off_time=0;				
				time_off_lock_count++;	//ʱ��ͳ�ƺ�������״̬
		}
	}
}
