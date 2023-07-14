#include "IIC.h"
 //I2C_IO口定义
sbit SCL=P3^0;
sbit SDA=P3^1;                         
uint pwd_numbers=0;  		//用于暂存数字密码 后存入密码数组

void Delay1(void)
{
	uchar Delay_t=20;
	while( -- Delay_t !=0);
}

void Start(void)	 //产生I2C总线的起始信号
	{
		SCL= 1;
		Delay1();
		SDA = 1;
		Delay1(); //起始条件建立时间大于4.7us延时
		SDA = 0;	 //发送起始信号
		Delay1();
		SCL = 0;   //钳住工2C总线，准备发送或接收数据
		Delay1();
}
void Stop(void) 			//产生I2C总线的停止信号
{
		uchar t =20;	//I2C总线停止后在下一次开始之前的等待时间
		SDA = 0 ;			//发送结束条件的数据信号
		Delay1();
		SCL= 1;				//发送结束条件的时钟信号
		Delay1();
		SDA = 1;			//发送I2C总线结束信号
		Delay1 ();
		while ( --t !=0 );	//在下一次产生start之前，要加一定的延时
}
void Write(uint dat)		//向I2C总线写1个字节的数据
{
		uchar t ;
		for(t=0;t<8;t++)
		{
			SDA =(bit)(dat & 0x80);
			Delay1();
			SCL= 1;//置时钟线为高，通知被控器开始接收数据位
			Delay1();
			SCL = 0;
			Delay1();
			dat <<= 1;
		} 
}
	
uchar Read(void)		//由从机读取1个字节的数据
{
			uint dat=0,t;
			bit temp;
			Delay1();
			Delay1();
			SDA = 1;						//在读取数据之前，要把SDA拉高
			Delay1();
			for (t=0;t<8;t++)
			{
				SCL = 0;		/*接受数据*/
				Delay1();
				SCL =1;			//置时钟线为高使数据线上升沿数据有效
				Delay1();
				temp =SDA;
				dat <<=1;
				if(temp==1)
					dat |= 0x01;
			}
			SCL=0;
			Delay1();
			return dat;
}

void GetAck(void)//读取从机应答信号
{
		uchar i;
		Delay1();
		SDA = 1; //8位发送完后释放数据线，准备接收应答信号释放总线
		Delay1();
		SCL =1; //接受数据
		Delay1();
		while ((SDA==1)&&(i<250))i++;//在规定时间内等待SDA 变为0
		SCL =0; //清时钟线,钳住12C总线以便继续接收
		Delay1();
}
void PutAck(bit ack) //主机应答或非应答信号（ack=0;应答信号，ack=1;非应答信号)
{
		SDA =ack;	//发出应答或非应答信号
		Delay1();
		SCL = 1;	//应答
		Delay1();
		SCL= 0;		//清时钟线,钳住I2C总线以便继续接收，继续占用
		Delay1(); //等待时钟线的释放
}

//主机通过I2c总线向从机发送多个字节的数据
//形参依次为:从机SLA地址、从机子地址、数据字节数、要发送的数据
void Write_E2prom(uchar SlaveAddr,uchar SubAddr,uchar Size,uint *dat)
{
		SlaveAddr &= 0xFE;	//确保从机地址最低位是0
		Start();//启动I2C总线
		Write(SlaveAddr);	//发送从机地址
		GetAck() ;//读取从机应答信号
		Write(SubAddr);//发送子地址
		GetAck();	//读取从机应答信号
		do //发送数据
		{
				Write(*dat++);
				GetAck();//读取从机应管信号
		}while ( --Size !=0 );//如发送完毕，结束发送
		Stop(); //停止I2c总线
}

//主机通过I2C总线从从机接收多个字节的教据
//形参依次为:从机SLA 地址、从机子地址、数据字节数、保存接收到的数据
void Read_E2prom(uchar SlaveAddr, uchar SubAddr,uchar Size, uint *dat)
{
		SlaveAddr &= 0xFE;	//确保最低位是0
		Start();						//启动I2c总线
		Write(SlaveAddr);		//发送从机写地址
		GetAck(); 					//读取从机应答信号
		Write(SubAddr);			//发送子地址
		GetAck();						//读取从机应答信号
		Start();						//发送重复起始条件
		SlaveAddr |=0x01;		//改为读地址
		Write(SlaveAddr) ;	//发送从机读地址
		GetAck();						//读取从机应答信号
		for (;;)//接收数据
		{
				*dat++ = Read();
				if ( --Size == 0 )//如接收完毕,结束接收
				{
					PutAck(1);//通知从机的非应答信号
					break;
				}
				PutAck(0);//通知从机的应答信号	
		}
		Stop();//停止I2c总线
}

void I2C_Read_pwd(void)
{
	 Read_E2prom(SLAVE_ADDR,E2PROM_ADDR_1,1,&pwd_numbers);
	 cur_pwd[1]=pwd_numbers/10;   //十位，
	 cur_pwd[2]=pwd_numbers%10;   //个位，
	
	 Read_E2prom(SLAVE_ADDR,E2PROM_ADDR_2,1,&pwd_numbers);
	 cur_pwd[3]=pwd_numbers/10;   //十位
	 cur_pwd[4]=pwd_numbers%10;   //个位 
}
void I2C_Write_pwd(void)
{
		pwd_numbers=cur_pwd[1];
		pwd_numbers=pwd_numbers*10+cur_pwd[2];
		Write_E2prom(SLAVE_ADDR,E2PROM_ADDR_1,1,&pwd_numbers);
	
		pwd_numbers=cur_pwd[3];
  	pwd_numbers=pwd_numbers*10+cur_pwd[4];
		Write_E2prom(SLAVE_ADDR,E2PROM_ADDR_2,1,&pwd_numbers);
}
