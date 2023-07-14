#ifndef __NIXIE_TUBE_H__
#define __NIXIE_TUBE_H__

#include "delay.h"

void DispLay_ON(void); 							//开锁成功显示	
void DispLay_ERR(void);							//开锁失败显示
void Display(int str_num[],int flag);	//  一次显示两位位数码管方法 
void Display1(int s);	//  一次显示四位数码管方法
#endif
