#include<STC15F2K60S2.h>
int flag=0;
sbit pwm1=P1^1;
sbit key1=P3^2;
unsigned int value[]={6,9,15,21,15,9};//6-0°；15-90°；24-180°
unsigned int percent = 0;
int k=0,count=0;
void Timer0Initial()
{
	TH0 = 0xFF;
	TL0 = 0xA4;
} //0.1ms产生一次中断
 
void initial_Timer()
{
	
	EA = 1;		   
	ET0 = 1;	
	AUXR &= 0x7F;	
	TMOD &= 0xF0;
	TMOD |= 0x01;	
	Timer0Initial();	 
	TR0 = 1;			  
}
 
void delay(unsigned int x)
{
	unsigned int i,j;
	for(i = 0;i< x; ++i)
		for(j = 120; j >0 ; --j)
			;
}

void keyscan()
{
	if(key1 == 0)
	{
		delay(3);
		if(key1 == 0)
		{
			
			while(!key1);
			
			flag=!flag;
		}
		
	}
 
	
}
 
void initial()
{
	key1 = 1;
	
	
	initial_Timer();
}

void Timer0() interrupt 1 using 0
{
	if(flag==1){
		percent += 1;
		if(percent == 200)
		{
			count++;
			percent = 0;
			pwm1 = 1;
		}
		if(percent == value[k])
		{
			
			pwm1 = 0;
		}
		if(count==20){
			count=0;
			k++;
			if(k==6)k=0;
		}
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