#include "keyboard.h"
#include "Nixie_tube.h"
#include "main.h"

#define KEY_Value P2  		//按键值对应的寄存器IO
#define KEY_Column 0x10   //列最低位 0001 0000

int key_flag=0;

/*KEY键值	11101110 ~ 01110111*/
char key_buf[]=	{0xee,0xde,0xbe,0x7e,0xed,0xdd,0xbd,0x7d, 
								 0xeb,0xdb,0xbb,0x7b,0xe7,0xd7,0xb7,0x77};	 
//矩阵按键扫描函数
void GetKey(void)
{
	char i=0,j=0;
	while(i<4)
	{
		/*
			*从列最低位逐渐遍历到最高位,
			*由于按键是低电平检测我们进行取反也就是 0xef-7f 11101111-01111111
		*/
		KEY_Value=~(KEY_Column<<i); 
		for(j=0;j<16;j++)	
		{
			if(key_buf[j]==KEY_Value)		//查到对应键值
			{	
				Delay(10);							//抖动处理 
				if(key_buf[j]!=KEY_Value)		//第二次查询 等待释放	
				{
					key_flag=j+1;  					//按照题目要求K1-K16 J以0开始 结果+1即可
					i=4;
					break;
				}
			}
		}
		i++;
	}
}


