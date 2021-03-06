//使用15系列51单片机的定时器2进行中断计时产生PWM波形信号
#include<STC15F2K60S2.h>
int flag=0;
sbit pwm1=P1^1;
sbit key1=P3^2;//开关1引脚
unsigned int value[]={6,11,16,21,16,11};//误差原因，6-0°，11-45°，16-90°,21-135°，26-180°
unsigned int percent = 0;//0.1ms次数标识
int k=0,count=0;
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
 
void delay(unsigned int x)
{
	unsigned int i,j;
	for(i = 0;i< x; ++i)
		for(j = 120; j >0 ; --j)
			;
}//延时函数，1ms

void keyscan()//按键扫描函数
{
	if(key1 == 0)
	{
		delay(3);//按键消抖，延时消抖
		if(key1 == 0)
		{
			
			while(!key1);//按键放开
			
			flag=!flag;
		}
		
	}
 
	
}
 
void initial()
{
	key1 = 1;
	
	
	initial_Timer();
}//初始化

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
		if(count==20){
			count=0;
			k++;
			if(k==6)k=0;
		}//,13ms/60°,延时，给舵机时间转动到指定的角度
	}
	Timer0Initial();
 
}

void main()
{
	initial();
	while(1)
	{
		keyscan();
		delay(10);
	}
}