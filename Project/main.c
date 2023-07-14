#include "main.h"

/************************************************
 基于ATM89C52-门锁的设计与实现
 CSDN："枫叶"，免费获取程序资料。
 授课老师：汪琳霞
 实验地点：计算机学院  
 作者：刘锋
************************************************/


Lock_Status_T lock_T;									//定义锁状态全局结构体变量
unsigned int code pwd[5]={0,1,2,3,4}; //初始密码1234，从index1开始
unsigned int cur_pwd[5];							//存储当前密码

/*
	*重置密码锁密码 默认1234
*/
void Init_cur_pwd()
{
	int i;
	for(i=1;i<5;i++)
		cur_pwd[i]=pwd[i];
	/*设置完成后将密码写入EEPROM*/
	I2C_Write_pwd();
}

/*
	*清除输入的内容 
	*一个参数：Number_T *number_t
	*Number_T *number：实时输入数组结构体
*/
void cur_number_Clear(Number_T *number_t) 		
{
	int i;
	for(i=1;i<5;i++)
		number_t->numbers[i]=0;
	number_t->input_index=0;
}
/*
	*清除密码锁所有状态  
	*两个参数：Number_T *number_t、Lock_Status_T *lock_t
	*Number_T *number：			键盘输入数组结构体
	*Lock_Status_T *lock_t：门锁状态结构体
*/
void lock_status_clear(Number_T *number_t,Lock_Status_T *lock_t)
{
			LED_displayOFF();							//熄屏模
			Lock_OFF();										//关锁-绿色灯灭
			ALARM_OFF();									//关报警-红色报价单灭
			SETLED_OFF();									//关闭设置密码状态灯
	
		  lock_t->Nixie_tube_status=0;	//清除是否开锁成功数码管显示标志位
			lock_t->time_off_lock=0;			//清除锁的开启与关闭标志位
			lock_t->Set_pwd_status=0;			//清除设置密码标志
	
			time_off_lock_count=0; 				//开锁后的倒计时5s清除
			alarm_flag_time=0; 						//报警时间60s时间清除
			cur_number_Clear(number_t);		//清除输入的内容
}
/*
	*清除密码锁所有状态  
	*两个参数：Number_T *number_t、Lock_Status_T *lock_t
	*Number_T *number：			键盘输入数组结构体
	*Lock_Status_T *lock_t：门锁状态结构体
*/
void KeyBoard_Control(Number_T *number_t,Lock_Status_T *lock_T)
{	
		int i;
		int pwd_count=0;
		GetKey();//调用矩阵键盘
	
		//错误三次后报警标记
		if(lock_T->alarm_flag!=3)
		{
			//当前输入的内容少于四位
			if(number_t->input_index<4)
			{
					//选择对应键值，存入数组当中
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
			//手动清除所有状态以及报警
			if(key_flag==12)
			{
				lock_status_clear(number_t,lock_T);
				lock_T->alarm_flag=0;					//清除错误三次后报警标记
			}
			/*清除一位或重新输入密码键*/
			if(key_flag==13)
			{
				if(number_t->input_index>0)
				{
					number_t->input_index--;
					LED_displayOFF();
				}
			}
			
			/*在设置密码状态开锁无效*/
			if(lock_T->Set_pwd_status==0)
			{
				/*#号键验证密码*/
				if(key_flag==15)
				{
					number_t->input_index=0;
					if(pwd_count==0)
					{
						//比较密码
						for(i=1;i<5;i++)
						{
							if(cur_pwd[i]==number_t->numbers[i])
							{
								pwd_count=pwd_count+1;
							}
						}
						/*比较密码*/
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
			/*Set设置密码*/
			if(key_flag==4)
			{
				lock_status_clear(number_t,lock_T);
				/*Set设置密码*/
				if(lock_T->Set_pwd_status==0)
				{
					lock_T->Set_pwd_status=1;
					SETLED_ON();
				}
				/*再次按下Set取消设置密码*/
				else if(lock_T->Set_pwd_status==1)
				{
					lock_T->Set_pwd_status=0;
					SETLED_OFF();
				}
			}
			/*当前正在设置密码*/
			if((lock_T->Set_pwd_status==1)&&key_flag==16)
			{
				//输入完成按下OK键
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
						/*设置完成后将密码写入EEPROM*/
						I2C_Write_pwd();
				}
				//设置失败，未达到四位,清除然后在重新输入
				else
				{				
						SETLED_OFF();
						LED_displayOFF();
						lock_T->Set_pwd_status=0;
						lock_T->Nixie_tube_status=1;
						cur_number_Clear(number_t);
				}
			}
			/*重置密码*/
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
/*主程序Main函数*/
void main(void)
{
	/*定义结构体变量*/
	Number_T number_T;
	//定时器初始化
	Timer_Init();
	//首先初始化锁的状态
	lock_status_clear(&number_T,&lock_T);
	//上电读取EEPROM存储的密码
	I2C_Read_pwd();
	while(1)
	{
		//矩阵键盘操作函数
		KeyBoard_Control(&number_T,&lock_T);
		//数码管显示函数
		Display(number_T.numbers,number_T.input_index);
		
		/*根据锁状态动态显示*/
		if(lock_T.Nixie_tube_status==1)
		{
			DispLay_ERR(); 				//密码错误显示ERR
		}
		if(lock_T.Nixie_tube_status==2)
		{
			DispLay_ON();	 				//密码正确显示ON
		}
		if(alarm_flag_time>60) 	//报警时间60s，大于60s恢复默认状态
		{
			lock_T.alarm_flag=0;	//报警清除标志
			ALARM_OFF();					//在关闭一次警告
		}
		//每次开锁后清除状态
		if(time_off_lock_count>=5)
		{
			//清除密码的状态
			lock_status_clear(&number_T,&lock_T);	
		}
	}
}
