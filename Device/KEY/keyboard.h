#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
#include "Delay.h"
#include "Nixie_tube.h"

//定义矩阵键盘按下标志位外部变量
extern int key_flag;
///矩阵按键扫描函数
void GetKey(void);

#endif