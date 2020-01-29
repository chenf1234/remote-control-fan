//ʹ��15ϵ��51��Ƭ���Ķ�ʱ��2�����жϼ�ʱ����PWM�����ź�
#include<STC15F2K60S2.h>
int flag=0;
sbit pwm1=P1^1;
sbit key1=P3^2;//����1����
unsigned int value[]={6,11,16,21,16,11};//���ԭ��6-0�㣬11-45�㣬16-90��,21-135�㣬26-180��
unsigned int percent = 0;//0.1ms������ʶ
int k=0,count=0;
void Timer0Initial()
{
	T2L = 0xAE;		
	T2H = 0xFB;
} //��ʱ0.1ms,11.0592MHz��0.01%�����
 
void initial_Timer()
{
	
		   
	
	AUXR |= 0x04;	//����1Tģʽ
		
	Timer0Initial();	
	
	AUXR |= 0x10;//��ʱ��2��ʼ��ʱ
	IE2 |=0x04;	//����ʱ��2�ж�
	EA = 1;	//�����ж�
}
 
void delay(unsigned int x)
{
	unsigned int i,j;
	for(i = 0;i< x; ++i)
		for(j = 120; j >0 ; --j)
			;
}//��ʱ������1ms

void keyscan()//����ɨ�躯��
{
	if(key1 == 0)
	{
		delay(3);//������������ʱ����
		if(key1 == 0)
		{
			
			while(!key1);//�����ſ�
			
			flag=!flag;
		}
		
	}
 
	
}
 
void initial()
{
	key1 = 1;
	
	
	initial_Timer();
}//��ʼ��

void Timer0() interrupt 12 //��ʱ��2
{
	if(flag==1){//���򿪿���
		percent += 1;//��������
		if(percent == 200)
		{
			count++;
			percent = 0;
			pwm1 = 1;
		}//�����͵�ƽ�����ﵽ20msʱ�������¸����ڣ����²�����ߵ�ƽ
		if(percent == value[k])
		{
			
			pwm1 = 0;
		}//�����ߵ�ƽ����ʱ��ﵽʱ����Ϊ�͵�ƽ
		if(count==20){
			count=0;
			k++;
			if(k==6)k=0;
		}//,13ms/60��,��ʱ�������ʱ��ת����ָ���ĽǶ�
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