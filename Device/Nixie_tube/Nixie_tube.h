#ifndef __NIXIE_TUBE_H__
#define __NIXIE_TUBE_H__

#include "delay.h"

void DispLay_ON(void); 							//�����ɹ���ʾ	
void DispLay_ERR(void);							//����ʧ����ʾ
void Display(int str_num[],int flag);	//  һ����ʾ��λλ����ܷ��� 
void Display1(int s);	//  һ����ʾ��λ����ܷ���
#endif
