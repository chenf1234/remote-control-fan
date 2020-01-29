#include<STC15F2K60S2.h>
sbit Key1=P3 ^ 2; //���ȼ���
sbit Key2=P3 ^ 3; //���ȼ���
sbit Key3=P1 ^ 7; //����ֹͣ���߿�ʼ
sbit pwm1=P4^1;
sbit pwm2=P4^2;
bit fflag=0;//����ת����־λ
int speed = 50,count = 0;
/****************��ʱ����**********************/
void Delay( unsigned int xms )
{
    unsigned char i;
    for( ; xms > 0; xms-- )
        for( i = 114; i > 0; i-- )
        {
            ;
        }
}
/****************��ʼ��**********************/
void init(){
	P4M0=0x06;
	P4M1=0x00;//����P4.1,P4.2Ϊ�������
	pwm1=0;
	pwm2=0;
	AUXR |= 0x80;	//��ʱ��ʱ��1Tģʽ
	TMOD = 0x01; //���ö�ʱ��ģʽ��16λ�Զ���װ��
	TL0 = (65536-1000)%256;		
	TH0 = (65536-1000)/256;	
	TF0=0;
	TR0=1;//��ʱ��0��ʼ��ʱ
	ET0=1;//�򿪶�ʱ��0�ж�
	EA=1;//�����ж�
}
/************����������***************/
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
/***********�жϴ�����**********/
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