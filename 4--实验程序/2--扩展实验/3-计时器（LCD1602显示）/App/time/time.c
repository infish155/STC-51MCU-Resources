#include "time.h"

//变量定义
u8 gmsec=0;//ms级定义
u8 gsec=0;//秒定义
u8 gmin=0;//分定义

/*******************************************************************************
* 函 数 名       : time0_init
* 函数功能		 : 定时器0中断配置函数，通过设置TH和TL即可确定定时时间
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void time0_init(void)
{
	TMOD|=0X01;//选择为定时器0模式，工作方式1
	TH0=0XDC;	//给定时器赋初值，定时10ms
	TL0=0X00;	
	ET0=1;//打开定时器0中断允许
	EA=1;//打开总中断
	TR0=0;//关闭定时器		
}

/*******************************************************************************
* 函 数 名       : time0_start
* 函数功能		 : 定时器0打开
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void time0_start(void)
{
	TR0=1;//打开定时器	
}

/*******************************************************************************
* 函 数 名       : time0_stop
* 函数功能		 : 定时器0关闭
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void time0_stop(void)
{
	TR0=0;//关闭定时器
	TH0=0X00;
	TL0=0X00;	
}

void time0() interrupt 1 //定时器0中断函数
{
	TH0=0XDC;	//给定时器赋初值，定时10ms
	TL0=0X00;
	gmsec++;//10ms加1次
	if(gmsec==100)//定时1秒
	{
		gmsec=0;
		gsec++;//对秒计数
		if(gsec==60)//到达60秒，即1分钟
		{
			gsec=0;
			gmin++;//对分计数
			if(gmin==60)gmin=0;
		}	
	}					
}