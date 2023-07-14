#include "IIC.h"
 //I2C_IO�ڶ���
sbit SCL=P3^0;
sbit SDA=P3^1;                         
uint pwd_numbers=0;  		//�����ݴ��������� �������������

void Delay1(void)
{
	uchar Delay_t=20;
	while( -- Delay_t !=0);
}

void Start(void)	 //����I2C���ߵ���ʼ�ź�
	{
		SCL= 1;
		Delay1();
		SDA = 1;
		Delay1(); //��ʼ��������ʱ�����4.7us��ʱ
		SDA = 0;	 //������ʼ�ź�
		Delay1();
		SCL = 0;   //ǯס��2C���ߣ�׼�����ͻ��������
		Delay1();
}
void Stop(void) 			//����I2C���ߵ�ֹͣ�ź�
{
		uchar t =20;	//I2C����ֹͣ������һ�ο�ʼ֮ǰ�ĵȴ�ʱ��
		SDA = 0 ;			//���ͽ��������������ź�
		Delay1();
		SCL= 1;				//���ͽ���������ʱ���ź�
		Delay1();
		SDA = 1;			//����I2C���߽����ź�
		Delay1 ();
		while ( --t !=0 );	//����һ�β���start֮ǰ��Ҫ��һ������ʱ
}
void Write(uint dat)		//��I2C����д1���ֽڵ�����
{
		uchar t ;
		for(t=0;t<8;t++)
		{
			SDA =(bit)(dat & 0x80);
			Delay1();
			SCL= 1;//��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ
			Delay1();
			SCL = 0;
			Delay1();
			dat <<= 1;
		} 
}
	
uchar Read(void)		//�ɴӻ���ȡ1���ֽڵ�����
{
			uint dat=0,t;
			bit temp;
			Delay1();
			Delay1();
			SDA = 1;						//�ڶ�ȡ����֮ǰ��Ҫ��SDA����
			Delay1();
			for (t=0;t<8;t++)
			{
				SCL = 0;		/*��������*/
				Delay1();
				SCL =1;			//��ʱ����Ϊ��ʹ������������������Ч
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

void GetAck(void)//��ȡ�ӻ�Ӧ���ź�
{
		uchar i;
		Delay1();
		SDA = 1; //8λ��������ͷ������ߣ�׼������Ӧ���ź��ͷ�����
		Delay1();
		SCL =1; //��������
		Delay1();
		while ((SDA==1)&&(i<250))i++;//�ڹ涨ʱ���ڵȴ�SDA ��Ϊ0
		SCL =0; //��ʱ����,ǯס12C�����Ա��������
		Delay1();
}
void PutAck(bit ack) //����Ӧ����Ӧ���źţ�ack=0;Ӧ���źţ�ack=1;��Ӧ���ź�)
{
		SDA =ack;	//����Ӧ����Ӧ���ź�
		Delay1();
		SCL = 1;	//Ӧ��
		Delay1();
		SCL= 0;		//��ʱ����,ǯסI2C�����Ա�������գ�����ռ��
		Delay1(); //�ȴ�ʱ���ߵ��ͷ�
}

//����ͨ��I2c������ӻ����Ͷ���ֽڵ�����
//�β�����Ϊ:�ӻ�SLA��ַ���ӻ��ӵ�ַ�������ֽ�����Ҫ���͵�����
void Write_E2prom(uchar SlaveAddr,uchar SubAddr,uchar Size,uint *dat)
{
		SlaveAddr &= 0xFE;	//ȷ���ӻ���ַ���λ��0
		Start();//����I2C����
		Write(SlaveAddr);	//���ʹӻ���ַ
		GetAck() ;//��ȡ�ӻ�Ӧ���ź�
		Write(SubAddr);//�����ӵ�ַ
		GetAck();	//��ȡ�ӻ�Ӧ���ź�
		do //��������
		{
				Write(*dat++);
				GetAck();//��ȡ�ӻ�Ӧ���ź�
		}while ( --Size !=0 );//�緢����ϣ���������
		Stop(); //ֹͣI2c����
}

//����ͨ��I2C���ߴӴӻ����ն���ֽڵĽ̾�
//�β�����Ϊ:�ӻ�SLA ��ַ���ӻ��ӵ�ַ�������ֽ�����������յ�������
void Read_E2prom(uchar SlaveAddr, uchar SubAddr,uchar Size, uint *dat)
{
		SlaveAddr &= 0xFE;	//ȷ�����λ��0
		Start();						//����I2c����
		Write(SlaveAddr);		//���ʹӻ�д��ַ
		GetAck(); 					//��ȡ�ӻ�Ӧ���ź�
		Write(SubAddr);			//�����ӵ�ַ
		GetAck();						//��ȡ�ӻ�Ӧ���ź�
		Start();						//�����ظ���ʼ����
		SlaveAddr |=0x01;		//��Ϊ����ַ
		Write(SlaveAddr) ;	//���ʹӻ�����ַ
		GetAck();						//��ȡ�ӻ�Ӧ���ź�
		for (;;)//��������
		{
				*dat++ = Read();
				if ( --Size == 0 )//��������,��������
				{
					PutAck(1);//֪ͨ�ӻ��ķ�Ӧ���ź�
					break;
				}
				PutAck(0);//֪ͨ�ӻ���Ӧ���ź�	
		}
		Stop();//ֹͣI2c����
}

void I2C_Read_pwd(void)
{
	 Read_E2prom(SLAVE_ADDR,E2PROM_ADDR_1,1,&pwd_numbers);
	 cur_pwd[1]=pwd_numbers/10;   //ʮλ��
	 cur_pwd[2]=pwd_numbers%10;   //��λ��
	
	 Read_E2prom(SLAVE_ADDR,E2PROM_ADDR_2,1,&pwd_numbers);
	 cur_pwd[3]=pwd_numbers/10;   //ʮλ
	 cur_pwd[4]=pwd_numbers%10;   //��λ 
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
