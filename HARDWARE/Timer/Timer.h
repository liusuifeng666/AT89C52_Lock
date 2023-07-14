#ifndef __TIMER_H__
#define __TimER_H__
#include "REG52.h"


void Timer_Init(void);
extern unsigned int alarm_flag_time;
extern unsigned int time_off_lock_count;
#endif