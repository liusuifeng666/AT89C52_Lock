#include "keyboard.h"
#include "Nixie_tube.h"
#include "main.h"

#define KEY_Value P2  		//����ֵ��Ӧ�ļĴ���IO
#define KEY_Column 0x10   //�����λ 0001 0000

int key_flag=0;

/*KEY��ֵ	11101110 ~ 01110111*/
char key_buf[]=	{0xee,0xde,0xbe,0x7e,0xed,0xdd,0xbd,0x7d, 
								 0xeb,0xdb,0xbb,0x7b,0xe7,0xd7,0xb7,0x77};	 
//���󰴼�ɨ�躯��
void GetKey(void)
{
	char i=0,j=0;
	while(i<4)
	{
		/*
			*�������λ�𽥱��������λ,
			*���ڰ����ǵ͵�ƽ������ǽ���ȡ��Ҳ���� 0xef-7f 11101111-01111111
		*/
		KEY_Value=~(KEY_Column<<i); 
		for(j=0;j<16;j++)	
		{
			if(key_buf[j]==KEY_Value)		//�鵽��Ӧ��ֵ
			{	
				Delay(10);							//�������� 
				if(key_buf[j]!=KEY_Value)		//�ڶ��β�ѯ �ȴ��ͷ�	
				{
					key_flag=j+1;  					//������ĿҪ��K1-K16 J��0��ʼ ���+1����
					i=4;
					break;
				}
			}
		}
		i++;
	}
}


