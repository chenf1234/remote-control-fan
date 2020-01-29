#include<STC15F2K60S2.h>
sbit Key1=P3 ^ 2; //风扇减速
sbit Key2=P3 ^ 3; //风扇加速
sbit Key3=P1 ^ 7; //风扇停止或者开始
sbit pwm1=P4^1;
sbit pwm2=P4^2;
bit fflag=0;//风扇转动标志位
int speed = 50,count = 0;
/****************延时处理**********************/
void Delay( unsigned int xms )
{
    unsigned char i;
    for( ; xms > 0; xms-- )
        for( i = 114; i > 0; i-- )
        {
            ;
        }
}
/****************初始化**********************/
void init(){
	P4M0=0x06;
	P4M1=0x00;//设置P4.1,P4.2为推挽输出
	pwm1=0;
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
/************按键处理函数***************/
void keys(){
	if(Key3==0){
		Delay(10);
		if(Key3==0){
			
			fflag=~fflag;
		}
		while(!Key3);
	}
	if(Key1==0){
		Delay(10);
		if(Key1==0){
			
			speed-=5;
			if(speed<=0) speed=0;
		}
		while(!Key1);
	}
	if(Key2==0){
		Delay(10);
		if(Key2==0){
			
			speed+=5;
			if(speed>=100)speed=100;
		}
		while(!Key2);
	}
}
/***********中断处理函数**********/
void timer0()interrupt 1
{
	TR0 = 0;
	TL0 = (65536-1000)%256;		
	TH0 = (65536-1000)/256;	
	TR0 = 1;
	if(fflag==1){
		count++;
		if(count>100){
			count=0;
		
		}
		if(count<=speed){
				pwm1=1;
		}
		else pwm1=0;
	}
}
void main()
{
	init();
	while(1){
		keys();
	}
}