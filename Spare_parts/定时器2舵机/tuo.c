//Ê¹ÓÃ15ÏµÁĞ51µ¥Æ¬»úµÄ¶¨Ê±Æ÷2½øĞĞÖĞ¶Ï¼ÆÊ±²úÉúPWM²¨ĞÎĞÅºÅ
#include<STC15F2K60S2.h>
int flag=0;
sbit pwm1=P1^1;
sbit key1=P3^2;//¿ª¹Ø1Òı½Å
unsigned int value[]={6,11,16,21,16,11};//Îó²îÔ­Òò£¬6-0¡ã£¬11-45¡ã£¬16-90¡ã,21-135¡ã£¬26-180¡ã
unsigned int percent = 0;//0.1ms´ÎÊı±êÊ¶
int k=0,count=0;
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
 
void delay(unsigned int x)
{
	unsigned int i,j;
	for(i = 0;i< x; ++i)
		for(j = 120; j >0 ; --j)
			;
}//ÑÓÊ±º¯Êı£¬1ms

void keyscan()//°´¼üÉ¨Ãèº¯Êı
{
	if(key1 == 0)
	{
		delay(3);//°´¼üÏû¶¶£¬ÑÓÊ±Ïû¶¶
		if(key1 == 0)
		{
			
			while(!key1);//°´¼ü·Å¿ª
			
			flag=!flag;
		}
		
	}
 
	
}
 
void initial()
{
	key1 = 1;
	
	
	initial_Timer();
}//³õÊ¼»¯

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
		if(count==20){
			count=0;
			k++;
			if(k==6)k=0;
		}//,13ms/60¡ã,ÑÓÊ±£¬¸ø¶æ»úÊ±¼ä×ª¶¯µ½Ö¸¶¨µÄ½Ç¶È
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