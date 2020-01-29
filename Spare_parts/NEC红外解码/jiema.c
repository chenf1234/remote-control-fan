#include<STC15F2K60S2.h>
#define uchar unsigned char
#define uint unsigned int
sbit led_sel=P2^3;
sbit IRIN=P3^6;
uchar irtime = 0;
uchar startflag = 0;	// ��ʼ��־λ
uchar ir_rc_ok = 0;		// �����/�͵�ƽʱ�������ɱ�־
uchar bitnum = 0;
uchar ir_change_ok = 0;		// ת����ɱ�־
 
uchar irdata[33];		// ��ƽʱ������
uchar ircode[4]={0,0,0,0};	// ң�ؽ�������
/********* ��ʱ���� **********/
void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}	  
/********** ϵͳ��ʼ�� ********/
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
		if(ircode[2]==~ircode[3]){
			ir_change_ok = 1;		// ����ת����ɱ�־
			ir_rc_ok = 0;		// ir���ձ�־��λ	
		}
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
void main()
{
	IOinit();
	sysinit();		// ��ʼ��
	while(1)
	{
		changecode();	// ����
		if(ir_change_ok == 1) {
			P0=ircode[2];
			ir_change_ok =0;
		}
	}
}