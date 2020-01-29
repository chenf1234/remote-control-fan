#include <STC15F2K60S2.H>
#define uint  unsigned int
#define uchar unsigned char

/*---------引脚别名定义---------*/
sbit sbtBeep = P3 ^ 4;                  //蜂鸣器引脚
sbit sbtKey1 = P3 ^ 2;                  //按键1引脚
/*---------初始化函数--------*/
void init()
{
    P3M1 = 0x00;
    P3M0 = 0x10;                  //设置P3^4为推挽模式
    P0 = 0x00;                    //关闭P0端口
    sbtBeep = 0;                  //蜂鸣器引脚置0，以保护蜂鸣器
}
uint xb=0;
/*---------延时子函数--------*/
void delay( uint xms )
{
    uchar i;
    for( ; xms > 0; xms-- )
        for( i = 114; i > 0; i-- )
        {
            ;
        }
}
/*---------蜂鸣器发声函数--------*/
void beeps(){
	for(xb=0;xb<255;xb++){
		sbtBeep=1;
		delay(10);
		sbtBeep=0;
		delay(10);
	}
}
/*---------主函数--------*/
void main()
{
    init();
    while( 1 )
    {
        if( sbtKey1 == 0 )
        {
            delay( 10 );                     //延时消抖
            if( sbtKey1 == 0 )
            {
                while( !sbtKey1 );       //松手后才响
                beeps();
            }
        }
    }
}