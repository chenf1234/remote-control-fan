#include<STC15F2K60S2.h>
#define uchar unsigned char
#define uint unsigned int
uchar arrSegSelect[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};//0-9
unsigned int ii=0;
/***************红外遥控相关*********/
sbit led_sel=P2^3;
sbit IRIN=P3^6;
uchar irtime = 0;
uchar startflag = 0;	// 起始标志位
uchar ir_rc_ok = 0;		// 红外高/低电平时间接收完成标志
uchar bitnum = 0;
uchar ir_change_ok = 0;		// 转码完成标志
 
uchar irdata[33];		// 电平时间数组
uchar ircode[4]={0,0,0,0};	// 遥控解码数组
uchar condition=0;//LED灯亮的前一状态
/***********舵机相关****************/
int flag=0;
sbit pwm1=P1^1;
//sbit key1=P3^2;//开关1引脚
unsigned int value[]={6,11,16,11};//误差原因，6-0°，11-45°，16-90°,21-135°，26-180°
unsigned int percent = 0;//0.1ms次数标识
int k=0,count=0;
/*************直流电机相关*****************/
sbit pwm0=P4^1;
sbit pwm2=P4^2;
bit fflag=0;//风扇转动标志位
int speed = 50,count0 = 0;
/*************蜂鸣器发声*****************/
sbit sbtBeep = P3 ^ 4;                  //蜂鸣器引脚
uint xb=0;
/**********延时处理函数***********/
void delay( unsigned int xms )
{
    unsigned char i;
    for( ; xms > 0; xms-- )
        for( i = 114; i > 0; i-- )
        {
            ;
        }
}//1ms
/****************蜂鸣器初始化**************/

void init_beep()
{
    P3M1 = 0x00;
    P3M0 = 0x10;                  //设置P3^4为推挽模式
    P0 = 0x00;                    //关闭P0端口
    sbtBeep = 0;                  //蜂鸣器引脚置0，以保护蜂鸣器
}
/***********直流电机初始化************/
void init(){
	P4M0=0x06;
	P4M1=0x00;//设置P4.1,P4.2为推挽输出
	pwm0=0;
	pwm2=0;
	AUXR |= 0x80;	//定时器时钟1T模式
	TMOD = 0x01; //设置定时器模式，16位自动重装载
	TL0 = (65536-1000)%256;		
	TH0 = (65536-1000)/256;	
	TF0=0;
	TR0=1;//定时器0开始计时
	ET0=1;//打开定时器0中断
	EA=1;//打开总中断
}
/**************舵机初始化********/
void Timer0Initial()
{
	T2L = 0xAE;		
	T2H = 0xFB;
} //定时0.1ms,11.0592MHz，0.01%的误差
 
void initial_Timer()
{
	
		   
	
	AUXR |= 0x04;	//设置1T模式
		
	Timer0Initial();	
	
	AUXR |= 0x10;//定时器2开始计时
	IE2 |=0x04;	//允许定时器2中断
	EA = 1;	//打开总中断
}
void initial()
{
//	key1 = 1;
	
	
	initial_Timer();
}//初始化
/***********红外遥控初始化 **********/
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
/*---------蜂鸣器发声函数--------*/
void beeps(){
	for(xb=0;xb<255;xb++){
		sbtBeep=1;
		delay(10);
		sbtBeep=0;
		delay(10);
	}
}

/***************直流电机中断处理函数***********/
/***********中断处理函数**********/
void timer11()interrupt 1
{
	TR0 = 0;
	TL0 = (65536-1000)%256;		
	TH0 = (65536-1000)/256;	
	TR0 = 1;
	if(fflag==1){
		count0++;
		if(count0>100){
			count0=0;
		
		}
		if(count0<=speed){
				pwm0=1;
		}
		else pwm0=0;
	}
	else{
		pwm0=0,pwm2=0;
	}
}
/***********舵机定时函数**************/
void Timer0() interrupt 12 //定时器2
{
	if(flag==1){//若打开开关
		percent += 1;//设置周期
		if(percent == 200)
		{
			count++;
			percent = 0;
			pwm1 = 1;
		}//产生低电平，当达到20ms时，进入下个周期，重新产生蹈叩缙�
		if(percent == value[k])
		{
			
			pwm1 = 0;
		}//产生高电平，当时间达到时，变为低电平
		if(count==21){
			count=0;
			k++;
			if(k==4)k=0;
		}//,13ms/60°,延时，给舵机时间转动到指定的角度
	}
	Timer0Initial();
 
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
		if(ircode[2]==~ircode[3]&&ircode[1]==0xff){
			ir_change_ok = 1;		// 编码转换完成标志
			ir_rc_ok = 0;		// ir接收标志复位	
		}
		else ir_rc_ok=0;  //不是符合条件的编码
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
/********键值处理函数**********/
void dealp(){
	switch(ircode[2]){
			case 0x45: flag=1;break;//舵机开始摇头
		  case 0x46: flag=0;break;//舵机停止摇头
			case 0x44: fflag=1;break;//风扇转动
		  case 0x40: fflag=0;break;//风扇停止转动
			case 0x15: speed+=10;if(speed>100) speed=100;break;//风扇加速
		  case 0x07: speed-=10;if(speed<0)  speed=0;break;//风扇减速
			default: break;
	}
}
/********主函数********/
void main()
{
	init_beep();
	init();
	initial();
	IOinit();
	sysinit();		// 初始化
	while(1)
	{
		changecode();	// 解码
		if(ir_change_ok == 1) {	
			dealp();
			ir_change_ok =0;
			beeps();
			condition=ircode[2];
		}
			if(fflag==1){
				led_sel=0;
				for(ii=6;ii<8;ii++){
					P0=0;
					P2=ii;
				
					if(ii==6)P0=arrSegSelect[speed/100];
					else P0=arrSegSelect[(speed/10)%10];
					delay(1);
				}
				P0=0;
			}
			led_sel=1;
			P0=condition;
			delay(10);	
	}
}