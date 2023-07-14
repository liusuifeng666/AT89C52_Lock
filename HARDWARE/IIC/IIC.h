#ifndef __IIC_H
#define __IIC_H
#include "delay.h"
#include "main.h"
#include "IIC.h"                       

void Delay1(void);
//产生I2C总线的起始信号
void Start(void); 
//产生I2C总线的停止信号
void Stop(void); 			
//向I2C总线写1个字节的数据
void Write(uint dat);		
//由从机读取1个字节的数据
uchar Read(void);		
//读取从机应答信号
void GetAck(void);
//主机应答或非应答信号（ack=0;应答信号，ack=1;非应答信号)
void PutAck(bit ack);
//e2prom的写函数 
void Write_E2prom(uchar SlaveAddr,uchar SubAddr,uchar Size,uint *dat);
//e2prom的读函数
void Read_E2prom(uchar SlaveAddr, uchar SubAddr,uchar Size,uint *dat);
//将EEPROM密码读入到密码数组中
void I2C_Read_pwd(void);
//将密码写入到EEPROM中
void I2C_Write_pwd(void);
#endif