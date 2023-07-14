#include "Nixie_tube.h"
#include "main.h"
#define wela P0  	//����P3��ǰ��λλλѡ
#define Duan P1		//����P1Ϊ��ѡ

uchar code table_yan[]={	0xC0,0xF9,0xA4,0xB0,0x99,
													0x92,0x82,0xF8,0x80,0x90}; //������������
uchar code table_yin[]={	0x3f,0x06,0x5b,0x4f,0x66,
													0x6d,0x7d,0x07,0x7f,0x6f}; //������������
/*��ʾON�ֿ⺯��*/
void DispLay_ON(void)
{
	int i=0;
	for(i=0;i<2;i++)
	{
		if(i==0)
			  Duan=0x5C; 				//ѡ����Ӧ����
		else if(i==1)
				Duan=0x54; 				//ѡ����Ӧ����
		 wela&=~(0x01 << i);	//ѡ����һλ�������ʾ
		 Delay(1); 
		 wela|=(0x01 << i);		//ÿ����ʾ����Ҫ���һ����Ӧ��λ
	}
}
/*��ʾERR�ֿ⺯��*/
void DispLay_ERR()
{
	int i=0;
	for(i=0;i<3;i++)
	{
		if(i==0)
			  Duan=0x79; 				//ѡ����Ӧ����
		else 
				Duan=0x77; 				//ѡ����Ӧ����
		 wela&=~(0x01 << i);	//ѡ����һλ�������ʾ
		 Delay(1); 
		 wela|=(0x01 << i);		//ÿ����ʾ����Ҫ���һ����Ӧ��λ
	}
}
/*
	*�����ʵʱ��ʾ����
	*������int str_num[]��int input_num
	*int str_num[]:��ǰ�������ֵ����
	*int input_num:��ǰ�������ֵ���������±�
*/
void Display(int str_num[],int input_num)
{
	int i=0;
	if(input_num==0)											//û�������κ�ֵ
	{
		LED_displayOFF();							//����ܺ���
	}
	for(i=0;i<input_num;i++)							//С��������������
	{
		Duan=table_yin[str_num[i+1]]; //ѡ����Ӧ����
		wela&=~(0x01 << i);						//ѡ����һλ�������ʾ
		Delay(1);			
		wela|=(0x01 << i);						//ÿ����ʾ����Ҫ���һ����Ӧ��λ
	}
}

