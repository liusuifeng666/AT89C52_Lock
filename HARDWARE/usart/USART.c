#include "USART.h"


//串口初始化程序
void USART_Init(void)
{
		SCON = 0x50;  //串口方式1，允许接收，TI和RI请零
    TMOD = 0x20;	//T1设定为定时方式2
		TH1 = TL1 = -(FOSC/12/32/BAUD); // 时钟频率/12/32/波特率-取反码=即可得到寄存器的值
    TR1 = 1;      //开启定时器    
    ES = 1;       //启用UART中断
    EA = 1;   		//开总中断
}
//串口发送字节函数
void SendData(BYTE dat)
{
    SBUF = dat;      //将数据写入SBUF        
		while (!TI);   //等待当前数据发送完毕       
		TI = 0;				//清楚TI发送标志位
}
//串口发送字符串函数
void SendString(char *s)
{
	while (*s!='\0')            //当缓冲区发送的数据不为空时
    {
			SendData(*s++);    //调用串口发送字节函数 指针++输出每一个字符 
    }
}



