#ifndef __IIC_H
#define __IIC_H
#include "delay.h"
#include "main.h"
#include "IIC.h"                       

void Delay1(void);
//����I2C���ߵ���ʼ�ź�
void Start(void); 
//����I2C���ߵ�ֹͣ�ź�
void Stop(void); 			
//��I2C����д1���ֽڵ�����
void Write(uint dat);		
//�ɴӻ���ȡ1���ֽڵ�����
uchar Read(void);		
//��ȡ�ӻ�Ӧ���ź�
void GetAck(void);
//����Ӧ����Ӧ���źţ�ack=0;Ӧ���źţ�ack=1;��Ӧ���ź�)
void PutAck(bit ack);
//e2prom��д���� 
void Write_E2prom(uchar SlaveAddr,uchar SubAddr,uchar Size,uint *dat);
//e2prom�Ķ�����
void Read_E2prom(uchar SlaveAddr, uchar SubAddr,uchar Size,uint *dat);
//��EEPROM������뵽����������
void I2C_Read_pwd(void);
//������д�뵽EEPROM��
void I2C_Write_pwd(void);
#endif