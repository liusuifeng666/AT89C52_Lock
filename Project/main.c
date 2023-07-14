#include "main.h"

/************************************************
 ����ATM89C52-�����������ʵ��
 CSDN��"��Ҷ"����ѻ�ȡ�������ϡ�
 �ڿ���ʦ������ϼ
 ʵ��ص㣺�����ѧԺ  
 ���ߣ�����
************************************************/


Lock_Status_T lock_T;									//������״̬ȫ�ֽṹ�����
unsigned int code pwd[5]={0,1,2,3,4}; //��ʼ����1234����index1��ʼ
unsigned int cur_pwd[5];							//�洢��ǰ����

/*
	*�������������� Ĭ��1234
*/
void Init_cur_pwd()
{
	int i;
	for(i=1;i<5;i++)
		cur_pwd[i]=pwd[i];
	/*������ɺ�����д��EEPROM*/
	I2C_Write_pwd();
}

/*
	*������������ 
	*һ��������Number_T *number_t
	*Number_T *number��ʵʱ��������ṹ��
*/
void cur_number_Clear(Number_T *number_t) 		
{
	int i;
	for(i=1;i<5;i++)
		number_t->numbers[i]=0;
	number_t->input_index=0;
}
/*
	*�������������״̬  
	*����������Number_T *number_t��Lock_Status_T *lock_t
	*Number_T *number��			������������ṹ��
	*Lock_Status_T *lock_t������״̬�ṹ��
*/
void lock_status_clear(Number_T *number_t,Lock_Status_T *lock_t)
{
			LED_displayOFF();							//Ϩ��ģ
			Lock_OFF();										//����-��ɫ����
			ALARM_OFF();									//�ر���-��ɫ���۵���
			SETLED_OFF();									//�ر���������״̬��
	
		  lock_t->Nixie_tube_status=0;	//����Ƿ����ɹ��������ʾ��־λ
			lock_t->time_off_lock=0;			//������Ŀ�����رձ�־λ
			lock_t->Set_pwd_status=0;			//������������־
	
			time_off_lock_count=0; 				//������ĵ���ʱ5s���
			alarm_flag_time=0; 						//����ʱ��60sʱ�����
			cur_number_Clear(number_t);		//������������
}
/*
	*�������������״̬  
	*����������Number_T *number_t��Lock_Status_T *lock_t
	*Number_T *number��			������������ṹ��
	*Lock_Status_T *lock_t������״̬�ṹ��
*/
void KeyBoard_Control(Number_T *number_t,Lock_Status_T *lock_T)
{	
		int i;
		int pwd_count=0;
		GetKey();//���þ������
	
		//�������κ󱨾����
		if(lock_T->alarm_flag!=3)
		{
			//��ǰ���������������λ
			if(number_t->input_index<4)
			{
					//ѡ���Ӧ��ֵ���������鵱��
					switch(key_flag) 
					{
						case 1:
								number_t->input_index++;
								number_t->numbers[number_t->input_index]=1;
							break;
						case 2:
								number_t->input_index++;
								number_t->numbers[number_t->input_index]=2;
							break;
						case 3:
								number_t->input_index++;
								number_t->numbers[number_t->input_index]=3;
							break;
						case 5:
								number_t->input_index++;
								number_t->numbers[number_t->input_index]=4;
							break;
						case 6:
								number_t->input_index++;
								number_t->numbers[number_t->input_index]=5;
							break;
						case 7:
								number_t->input_index++;
								number_t->numbers[number_t->input_index]=6;
							break;
						case 9: 
								number_t->input_index++;
								number_t->numbers[number_t->input_index]=7;
							break;
						case 10:
								number_t->input_index++;
								number_t->numbers[number_t->input_index]=8;
							break;
						case 11:
								number_t->input_index++;
								number_t->numbers[number_t->input_index]=9;
							break;
						case 14:
								number_t->input_index++;
								number_t->numbers[number_t->input_index]=0;
							break;
					}
			}
			//�ֶ��������״̬�Լ�����
			if(key_flag==12)
			{
				lock_status_clear(number_t,lock_T);
				lock_T->alarm_flag=0;					//����������κ󱨾����
			}
			/*���һλ���������������*/
			if(key_flag==13)
			{
				if(number_t->input_index>0)
				{
					number_t->input_index--;
					LED_displayOFF();
				}
			}
			
			/*����������״̬������Ч*/
			if(lock_T->Set_pwd_status==0)
			{
				/*#�ż���֤����*/
				if(key_flag==15)
				{
					number_t->input_index=0;
					if(pwd_count==0)
					{
						//�Ƚ�����
						for(i=1;i<5;i++)
						{
							if(cur_pwd[i]==number_t->numbers[i])
							{
								pwd_count=pwd_count+1;
							}
						}
						/*�Ƚ�����*/
						if(pwd_count==4)
						{
							Lock_ON();
							lock_T->Nixie_tube_status=2;
						}
						else
						{
							lock_T->alarm_flag++; 
							lock_T->Nixie_tube_status=1;
						}
						lock_T->time_off_lock=1;
						pwd_count=0;
					}
				}
			}
			/*Set��������*/
			if(key_flag==4)
			{
				lock_status_clear(number_t,lock_T);
				/*Set��������*/
				if(lock_T->Set_pwd_status==0)
				{
					lock_T->Set_pwd_status=1;
					SETLED_ON();
				}
				/*�ٴΰ���Setȡ����������*/
				else if(lock_T->Set_pwd_status==1)
				{
					lock_T->Set_pwd_status=0;
					SETLED_OFF();
				}
			}
			/*��ǰ������������*/
			if((lock_T->Set_pwd_status==1)&&key_flag==16)
			{
				//������ɰ���OK��
				if(number_t->input_index==4) 
				{
						for(i=1;i<5;i++)
						{
							cur_pwd[i]=number_t->numbers[i];
						}
						SETLED_OFF(); 
						lock_T->Set_pwd_status=0;  
						lock_T->Nixie_tube_status=0;
						cur_number_Clear(number_t);
						/*������ɺ�����д��EEPROM*/
						I2C_Write_pwd();
				}
				//����ʧ�ܣ�δ�ﵽ��λ,���Ȼ������������
				else
				{				
						SETLED_OFF();
						LED_displayOFF();
						lock_T->Set_pwd_status=0;
						lock_T->Nixie_tube_status=1;
						cur_number_Clear(number_t);
				}
			}
			/*��������*/
			if(key_flag==8)
			{
				Init_cur_pwd();
				for(i=0;i<6;i++)
				{
					LED_Set=~LED_Set;
					Delay(300);
				}
			}
		}
		key_flag=0;
}
/*������Main����*/
void main(void)
{
	/*����ṹ�����*/
	Number_T number_T;
	//��ʱ����ʼ��
	Timer_Init();
	//���ȳ�ʼ������״̬
	lock_status_clear(&number_T,&lock_T);
	//�ϵ��ȡEEPROM�洢������
	I2C_Read_pwd();
	while(1)
	{
		//������̲�������
		KeyBoard_Control(&number_T,&lock_T);
		//�������ʾ����
		Display(number_T.numbers,number_T.input_index);
		
		/*������״̬��̬��ʾ*/
		if(lock_T.Nixie_tube_status==1)
		{
			DispLay_ERR(); 				//���������ʾERR
		}
		if(lock_T.Nixie_tube_status==2)
		{
			DispLay_ON();	 				//������ȷ��ʾON
		}
		if(alarm_flag_time>60) 	//����ʱ��60s������60s�ָ�Ĭ��״̬
		{
			lock_T.alarm_flag=0;	//���������־
			ALARM_OFF();					//�ڹر�һ�ξ���
		}
		//ÿ�ο��������״̬
		if(time_off_lock_count>=5)
		{
			//��������״̬
			lock_status_clear(&number_T,&lock_T);	
		}
	}
}
