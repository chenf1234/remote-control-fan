#include<STC15F2K60S2.h>
#define uchar unsigned char
#define uint unsigned int
uchar arrSegSelect[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};//0-9
unsigned int ii=0;
/***************����ң�����*********/
sbit led_sel=P2^3;
sbit IRIN=P3^6;
uchar irtime = 0;
uchar startflag = 0;	// ��ʼ��־λ
uchar ir_rc_ok = 0;		// �����/�͵�ƽʱ�������ɱ�־
uchar bitnum = 0;
uchar ir_change_ok = 0;		// ת����ɱ�־
 
uchar irdata[33];		// ��ƽʱ������
uchar ircode[4]={0,0,0,0};	// ң�ؽ�������
uchar condition=0;//LED������ǰһ״̬
/***********������****************/
int flag=0;
sbit pwm1=P1^1;
//sbit key1=P3^2;//����1����
unsigned int value[]={6,11,16,11};//���ԭ��6-0�㣬11-45�㣬16-90��,21-135�㣬26-180��
unsigned int percent = 0;//0.1ms������ʶ
int k=0,count=0;
/*************ֱ��������*****************/
sbit pwm0=P4^1;
sbit pwm2=P4^2;
bit fflag=0;//����ת����־λ
int speed = 50,count0 = 0;
/*************����������*****************/
sbit sbtBeep = P3 ^ 4;                  //����������
uint xb=0;
/**********��ʱ������***********/
void delay( unsigned int xms )
{
    unsigned char i;
    for( ; xms > 0; xms-- )
        for( i = 114; i > 0; i-- )
        {
            ;
        }
}//1ms
/****************��������ʼ��**************/

void init_beep()
{
    P3M1 = 0x00;
    P3M0 = 0x10;                  //����P3^4Ϊ����ģʽ
    P0 = 0x00;                    //�ر�P0�˿�
    sbtBeep = 0;                  //������������0���Ա���������
}
/***********ֱ�������ʼ��************/
void init(){
	P4M0=0x06;
	P4M1=0x00;//����P4.1,P4.2Ϊ�������
	pwm0=0;
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
/**************�����ʼ��********/
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
void initial()
{
//	key1 = 1;
	
	
	initial_Timer();
}//��ʼ��
/***********����ң�س�ʼ�� **********/
void sysinit()
{
	// ��ʼ����ʱ��
	TMOD |= 0x20;	// ���ö�ʱ��1λ������ʽ2 (0~255)
	TH1 = 0x00;		// ����ֵ
	TL1 = 0x00;   ////256*(1/12m)*12=0.256ms
	TR1 = 1;	// ������ʱ��1
	ET1 = 1;	// ʹ�ܶ�ʱ��1
 
	// ��ʼ���ⲿ�ж�2
	INT_CLKO |= 0x10;//��INT2��
	EA = 1;		// �����ж�
}


/********* LED�Ƴ�ʼ�� **********/
void IOinit(){
	P0M0=0xff;
	P0M1=0x00;
	P2M1=0x00;
	P2M0=0x08;
	led_sel=1;
	P0=0x00;
}
/*---------��������������--------*/
void beeps(){
	for(xb=0;xb<255;xb++){
		sbtBeep=1;
		delay(10);
		sbtBeep=0;
		delay(10);
	}
}

/***************ֱ������жϴ�����***********/
/***********�жϴ�����**********/
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
/***********�����ʱ����**************/
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
		if(count==21){
			count=0;
			k++;
			if(k==4)k=0;
		}//,13ms/60��,��ʱ�������ʱ��ת����ָ���ĽǶ�
	}
	Timer0Initial();
 
}
/************ ת������ *************/
void changecode()
{
	
	uchar k = 1;
	uchar i,j;
	uchar value;
	
	if(ir_rc_ok ==1)		// ���ir������ɾͽ���ת��
	{
		for(i=0;i<4;i++)	// ���ǰ������4��
		{
			for(j=0;j<8;j++)//8λ
			{
				value = value>>1;	// 
				if(irdata[k]>6&&irdata[k]<10)		// �ж������Ƿ�Ϊ" 1 "
				{
					value |= 0x80;
				}
				k++;
			}
			ircode[i] = value;		// �洢 
		}
		if(ircode[2]==~ircode[3]&&ircode[1]==0xff){
			ir_change_ok = 1;		// ����ת����ɱ�־
			ir_rc_ok = 0;		// ir���ձ�־��λ	
		}
		else ir_rc_ok=0;  //���Ƿ��������ı���
	}
}
/********* ��ʱ��1�жϷ����� ***********/
void t0()interrupt 3
{
	irtime++;
}
/********* INT2������ ***********/
void exint1() interrupt 10
{
	
	if(startflag)	
	{
		if(irtime>32&&irtime<63) 		// ���ǰ����
		{
			bitnum=0;			
		}			
		
		irdata[bitnum]=irtime;		// ��ǰ���뿪ʼ��������
		irtime=0;
		bitnum++;
		
		if(bitnum==33)		// ǰ����+4�ֽ����ݣ���33���� 
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
/********��ֵ������**********/
void dealp(){
	switch(ircode[2]){
			case 0x45: flag=1;break;//�����ʼҡͷ
		  case 0x46: flag=0;break;//���ֹͣҡͷ
			case 0x44: fflag=1;break;//����ת��
		  case 0x40: fflag=0;break;//����ֹͣת��
			case 0x15: speed+=10;if(speed>100) speed=100;break;//���ȼ���
		  case 0x07: speed-=10;if(speed<0)  speed=0;break;//���ȼ���
			default: break;
	}
}
/********������********/
void main()
{
	init_beep();
	init();
	initial();
	IOinit();
	sysinit();		// ��ʼ��
	while(1)
	{
		changecode();	// ����
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