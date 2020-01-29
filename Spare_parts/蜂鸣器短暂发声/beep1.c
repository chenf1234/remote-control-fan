#include <STC15F2K60S2.H>
#define uint  unsigned int
#define uchar unsigned char

/*---------���ű�������---------*/
sbit sbtBeep = P3 ^ 4;                  //����������
sbit sbtKey1 = P3 ^ 2;                  //����1����
/*---------��ʼ������--------*/
void init()
{
    P3M1 = 0x00;
    P3M0 = 0x10;                  //����P3^4Ϊ����ģʽ
    P0 = 0x00;                    //�ر�P0�˿�
    sbtBeep = 0;                  //������������0���Ա���������
}
uint xb=0;
/*---------��ʱ�Ӻ���--------*/
void delay( uint xms )
{
    uchar i;
    for( ; xms > 0; xms-- )
        for( i = 114; i > 0; i-- )
        {
            ;
        }
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
/*---------������--------*/
void main()
{
    init();
    while( 1 )
    {
        if( sbtKey1 == 0 )
        {
            delay( 10 );                     //��ʱ����
            if( sbtKey1 == 0 )
            {
                while( !sbtKey1 );       //���ֺ����
                beeps();
            }
        }
    }
}