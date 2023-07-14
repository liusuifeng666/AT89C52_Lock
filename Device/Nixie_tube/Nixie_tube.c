#include "Nixie_tube.h"
#include "main.h"
#define wela P0  	//申明P3口前四位位位选
#define Duan P1		//申明P1为段选

uchar code table_yan[]={	0xC0,0xF9,0xA4,0xB0,0x99,
													0x92,0x82,0xF8,0x80,0x90}; //共阳数码管码表
uchar code table_yin[]={	0x3f,0x06,0x5b,0x4f,0x66,
													0x6d,0x7d,0x07,0x7f,0x6f}; //共阴数码管码表
/*显示ON字库函数*/
void DispLay_ON(void)
{
	int i=0;
	for(i=0;i<2;i++)
	{
		if(i==0)
			  Duan=0x5C; 				//选择相应的码
		else if(i==1)
				Duan=0x54; 				//选择相应的码
		 wela&=~(0x01 << i);	//选择哪一位数码管显示
		 Delay(1); 
		 wela|=(0x01 << i);		//每次显示后需要清除一下相应的位
	}
}
/*显示ERR字库函数*/
void DispLay_ERR()
{
	int i=0;
	for(i=0;i<3;i++)
	{
		if(i==0)
			  Duan=0x79; 				//选择相应的码
		else 
				Duan=0x77; 				//选择相应的码
		 wela&=~(0x01 << i);	//选择哪一位数码管显示
		 Delay(1); 
		 wela|=(0x01 << i);		//每次显示后需要清除一下相应的位
	}
}
/*
	*数码管实时显示函数
	*参数：int str_num[]、int input_num
	*int str_num[]:当前输入的数值数组
	*int input_num:当前输入的数值数组索引下标
*/
void Display(int str_num[],int input_num)
{
	int i=0;
	if(input_num==0)											//没有输入任何值
	{
		LED_displayOFF();							//数码管黑屏
	}
	for(i=0;i<input_num;i++)							//小于输入的密码个数
	{
		Duan=table_yin[str_num[i+1]]; //选择相应的码
		wela&=~(0x01 << i);						//选择哪一位数码管显示
		Delay(1);			
		wela|=(0x01 << i);						//每次显示后需要清除一下相应的位
	}
}

