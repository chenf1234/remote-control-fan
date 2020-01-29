#include<STC15F2K60S2.h>
#define uchar unsigned char
#define uint unsigned int
sbit led_sel=P2^3;
sbit IRIN=P3^6;
uchar irtime = 0;
uchar startflag = 0;	// 起始标志位
uchar ir_rc_ok = 0;		// 红外高/低电平时间接收完成标志
uchar bitnum = 0;
uchar ir_change_ok = 0;		// 转码完成标志
 
uchar irdata[33];		// 电平时间数组
uchar ircode[4]={0,0,0,0};	// 遥控解码数组
/********* 延时函数 **********/
void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}	  
/********** 系统初始化 ********/
void sysinit()
{
	// 初始化定时器
	TMOD |= 0x20;	// 设置定时器1位工作方式2 (0~255)
	TH1 = 0x00;		// 赋初值
	TL1 = 0x00;   ////256*(1/12m)*12=0.256ms
	TR1 = 1;	// 启动定时器1
	ET1 = 1;	// 使能定时器1
 
	// 初始化外部中断2
	INT_CLKO |= 0x10;//开INT2；
	EA = 1;		// 开总中断
}


/********* LED灯初始化 **********/
void IOinit(){
	P0M0=0xff;
	P0M1=0x00;
	P2M1=0x00;
	P2M0=0x08;
	led_sel=1;
	P0=0x00;
}
/************ 转换编码 *************/
void changecode()
{
	uchar k = 1;
	uchar i,j;
	uchar value;
	if(ir_rc_ok ==1)		// 如果ir接受完成就进行转码
	{
		for(i=0;i<4;i++)	// 获得前导码后的4个
		{
			for(j=0;j<8;j++)//8位
			{
				value = value>>1;	// 
				if(irdata[k]>6&&irdata[k]<10)		// 判断数据是否为" 1 "
				{
					value |= 0x80;
				}
				k++;
			}
			ircode[i] = value;		// 存储 
		}
		if(ircode[2]==~ircode[3]){
			ir_change_ok = 1;		// 编码转换完成标志
			ir_rc_ok = 0;		// ir接收标志复位	
		}
	}
}
/********* 定时器1中断服务函数 ***********/
void t0()interrupt 3
{
	irtime++;
}
/********* INT2处理函数 ***********/
void exint1() interrupt 10
{
	if(startflag)	
	{
		if(irtime>32&&irtime<63) 		// 检测前导码
		{
			bitnum=0;			
		}			
		
		irdata[bitnum]=irtime;		// 从前导码开始接收数据
		irtime=0;
		bitnum++;
		
		if(bitnum==33)		// 前导码+4字节数据，共33比特 
		{
			bitnum=0;
			ir_rc_ok = 1;
		}		
 
	}
	else
	{
		startflag = 1;
		irtime = 0;
	}
}
void main()
{
	IOinit();
	sysinit();		// 初始化
	while(1)
	{
		changecode();	// 解码
		if(ir_change_ok == 1) {
			P0=ircode[2];
			ir_change_ok =0;
		}
	}
}