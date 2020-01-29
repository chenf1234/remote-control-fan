#include<STC15F2K60S2.h>
#define uchar unsigned char
#define uint unsigned int
uchar arrSegSelect[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};//0-9
unsigned int ii=0;
/***************ºìÍâÒ£¿ØÏà¹Ø*********/
sbit led_sel=P2^3;
sbit IRIN=P3^6;
uchar irtime = 0;
uchar startflag = 0;	// ÆğÊ¼±êÖ¾Î»
uchar ir_rc_ok = 0;		// ºìÍâ¸ß/µÍµçÆ½Ê±¼ä½ÓÊÕÍê³É±êÖ¾
uchar bitnum = 0;
uchar ir_change_ok = 0;		// ×ªÂëÍê³É±êÖ¾
 
uchar irdata[33];		// µçÆ½Ê±¼äÊı×é
uchar ircode[4]={0,0,0,0};	// Ò£¿Ø½âÂëÊı×é
uchar condition=0;//LEDµÆÁÁµÄÇ°Ò»×´Ì¬
/***********¶æ»úÏà¹Ø****************/
int flag=0;
sbit pwm1=P1^1;
//sbit key1=P3^2;//¿ª¹Ø1Òı½Å
unsigned int value[]={6,11,16,11};//Îó²îÔ­Òò£¬6-0¡ã£¬11-45¡ã£¬16-90¡ã,21-135¡ã£¬26-180¡ã
unsigned int percent = 0;//0.1ms´ÎÊı±êÊ¶
int k=0,count=0;
/*************Ö±Á÷µç»úÏà¹Ø*****************/
sbit pwm0=P4^1;
sbit pwm2=P4^2;
bit fflag=0;//·çÉÈ×ª¶¯±êÖ¾Î»
int speed = 50,count0 = 0;
/*************·äÃùÆ÷·¢Éù*****************/
sbit sbtBeep = P3 ^ 4;                  //·äÃùÆ÷Òı½Å
uint xb=0;
/**********ÑÓÊ±´¦Àíº¯Êı***********/
void delay( unsigned int xms )
{
    unsigned char i;
    for( ; xms > 0; xms-- )
        for( i = 114; i > 0; i-- )
        {
            ;
        }
}//1ms
/****************·äÃùÆ÷³õÊ¼»¯**************/

void init_beep()
{
    P3M1 = 0x00;
    P3M0 = 0x10;                  //ÉèÖÃP3^4ÎªÍÆÍìÄ£Ê½
    P0 = 0x00;                    //¹Ø±ÕP0¶Ë¿Ú
    sbtBeep = 0;                  //·äÃùÆ÷Òı½ÅÖÃ0£¬ÒÔ±£»¤·äÃùÆ÷
}
/***********Ö±Á÷µç»ú³õÊ¼»¯************/
void init(){
	P4M0=0x06;
	P4M1=0x00;//ÉèÖÃP4.1,P4.2ÎªÍÆÍìÊä³ö
	pwm0=0;
	pwm2=0;
	AUXR |= 0x80;	//¶¨Ê±Æ÷Ê±ÖÓ1TÄ£Ê½
	TMOD = 0x01; //ÉèÖÃ¶¨Ê±Æ÷Ä£Ê½£¬16Î»×Ô¶¯ÖØ×°ÔØ
	TL0 = (65536-1000)%256;		
	TH0 = (65536-1000)/256;	
	TF0=0;
	TR0=1;//¶¨Ê±Æ÷0¿ªÊ¼¼ÆÊ±
	ET0=1;//´ò¿ª¶¨Ê±Æ÷0ÖĞ¶Ï
	EA=1;//´ò¿ª×ÜÖĞ¶Ï
}
/**************¶æ»ú³õÊ¼»¯********/
void Timer0Initial()
{
	T2L = 0xAE;		
	T2H = 0xFB;
} //¶¨Ê±0.1ms,11.0592MHz£¬0.01%µÄÎó²î
 
void initial_Timer()
{
	
		   
	
	AUXR |= 0x04;	//ÉèÖÃ1TÄ£Ê½
		
	Timer0Initial();	
	
	AUXR |= 0x10;//¶¨Ê±Æ÷2¿ªÊ¼¼ÆÊ±
	IE2 |=0x04;	//ÔÊĞí¶¨Ê±Æ÷2ÖĞ¶Ï
	EA = 1;	//´ò¿ª×ÜÖĞ¶Ï
}
void initial()
{
//	key1 = 1;
	
	
	initial_Timer();
}//³õÊ¼»¯
/***********ºìÍâÒ£¿Ø³õÊ¼»¯ **********/
void sysinit()
{
	// ³õÊ¼»¯¶¨Ê±Æ÷
	TMOD |= 0x20;	// ÉèÖÃ¶¨Ê±Æ÷1Î»¹¤×÷·½Ê½2 (0~255)
	TH1 = 0x00;		// ¸³³õÖµ
	TL1 = 0x00;   ////256*(1/12m)*12=0.256ms
	TR1 = 1;	// Æô¶¯¶¨Ê±Æ÷1
	ET1 = 1;	// Ê¹ÄÜ¶¨Ê±Æ÷1
 
	// ³õÊ¼»¯Íâ²¿ÖĞ¶Ï2
	INT_CLKO |= 0x10;//¿ªINT2£»
	EA = 1;		// ¿ª×ÜÖĞ¶Ï
}


/********* LEDµÆ³õÊ¼»¯ **********/
void IOinit(){
	P0M0=0xff;
	P0M1=0x00;
	P2M1=0x00;
	P2M0=0x08;
	led_sel=1;
	P0=0x00;
}
/*---------·äÃùÆ÷·¢Éùº¯Êı--------*/
void beeps(){
	for(xb=0;xb<255;xb++){
		sbtBeep=1;
		delay(10);
		sbtBeep=0;
		delay(10);
	}
}

/***************Ö±Á÷µç»úÖĞ¶Ï´¦Àíº¯Êı***********/
/***********ÖĞ¶Ï´¦Àíº¯Êı**********/
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
/***********¶æ»ú¶¨Ê±º¯Êı**************/
void Timer0() interrupt 12 //¶¨Ê±Æ÷2
{
	if(flag==1){//Èô´ò¿ª¿ª¹Ø
		percent += 1;//ÉèÖÃÖÜÆÚ
		if(percent == 200)
		{
			count++;
			percent = 0;
			pwm1 = 1;
		}//²úÉúµÍµçÆ½£¬µ±´ïµ½20msÊ±£¬½øÈëÏÂ¸öÖÜÆÚ£¬ÖØĞÂ²úÉúµ¸ßµçÆ½
		if(percent == value[k])
		{
			
			pwm1 = 0;
		}//²úÉú¸ßµçÆ½£¬µ±Ê±¼ä´ïµ½Ê±£¬±äÎªµÍµçÆ½
		if(count==21){
			count=0;
			k++;
			if(k==4)k=0;
		}//,13ms/60¡ã,ÑÓÊ±£¬¸ø¶æ»úÊ±¼ä×ª¶¯µ½Ö¸¶¨µÄ½Ç¶È
	}
	Timer0Initial();
 
}
/************ ×ª»»±àÂë *************/
void changecode()
{
	
	uchar k = 1;
	uchar i,j;
	uchar value;
	
	if(ir_rc_ok ==1)		// Èç¹ûir½ÓÊÜÍê³É¾Í½øĞĞ×ªÂë
	{
		for(i=0;i<4;i++)	// »ñµÃÇ°µ¼ÂëºóµÄ4¸ö
		{
			for(j=0;j<8;j++)//8Î»
			{
				value = value>>1;	// 
				if(irdata[k]>6&&irdata[k]<10)		// ÅĞ¶ÏÊı¾İÊÇ·ñÎª" 1 "
				{
					value |= 0x80;
				}
				k++;
			}
			ircode[i] = value;		// ´æ´¢ 
		}
		if(ircode[2]==~ircode[3]&&ircode[1]==0xff){
			ir_change_ok = 1;		// ±àÂë×ª»»Íê³É±êÖ¾
			ir_rc_ok = 0;		// ir½ÓÊÕ±êÖ¾¸´Î»	
		}
		else ir_rc_ok=0;  //²»ÊÇ·ûºÏÌõ¼şµÄ±àÂë
	}
}
/********* ¶¨Ê±Æ÷1ÖĞ¶Ï·şÎñº¯Êı ***********/
void t0()interrupt 3
{
	irtime++;
}
/********* INT2´¦Àíº¯Êı ***********/
void exint1() interrupt 10
{
	
	if(startflag)	
	{
		if(irtime>32&&irtime<63) 		// ¼ì²âÇ°µ¼Âë
		{
			bitnum=0;			
		}			
		
		irdata[bitnum]=irtime;		// ´ÓÇ°µ¼Âë¿ªÊ¼½ÓÊÕÊı¾İ
		irtime=0;
		bitnum++;
		
		if(bitnum==33)		// Ç°µ¼Âë+4×Ö½ÚÊı¾İ£¬¹²33±ÈÌØ 
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
/********¼üÖµ´¦Àíº¯Êı**********/
void dealp(){
	switch(ircode[2]){
			case 0x45: flag=1;break;//¶æ»ú¿ªÊ¼Ò¡Í·
		  case 0x46: flag=0;break;//¶æ»úÍ£Ö¹Ò¡Í·
			case 0x44: fflag=1;break;//·çÉÈ×ª¶¯
		  case 0x40: fflag=0;break;//·çÉÈÍ£Ö¹×ª¶¯
			case 0x15: speed+=10;if(speed>100) speed=100;break;//·çÉÈ¼ÓËÙ
		  case 0x07: speed-=10;if(speed<0)  speed=0;break;//·çÉÈ¼õËÙ
			default: break;
	}
}
/********Ö÷º¯Êı********/
void main()
{
	init_beep();
	init();
	initial();
	IOinit();
	sysinit();		// ³õÊ¼»¯
	while(1)
	{
		changecode();	// ½âÂë
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