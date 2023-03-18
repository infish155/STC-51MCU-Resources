#include "ds1302.h"
#include "intrins.h"

//---DS1302写入和读取时分秒的地址命令---//
//---秒分时日月周年 最低位读写位;-------//
u8 gREAD_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
u8 gWRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};

//---DS1302时钟初始化2021年5月20日星期四13点51分47秒。---//
//---存储顺序是秒分时日月周年,存储格式是用BCD码---//
u8 gDS1302_TIME[7] = {0x47, 0x51, 0x13, 0x20, 0x04, 0x05, 0x21};


/*******************************************************************************
* 函 数 名       : ds1302_write_byte
* 函数功能		 : DS1302写单字节
* 输    入       : addr：地址/命令
				   dat：数据
* 输    出    	 : 无
*******************************************************************************/
void ds1302_write_byte(u8 addr,u8 dat)
{
	u8 i=0;
	
	DS1302_RST=0;
	_nop_();	
	DS1302_CLK=0;//CLK低电平
	_nop_();
	DS1302_RST=1;//RST由低到高变化
	_nop_();

	for(i=0;i<8;i++)//循环8次，每次写1位，先写低位再写高位
	{
		DS1302_IO=addr&0x01;
		addr>>=1;
		DS1302_CLK=1;
		_nop_();
		DS1302_CLK=0;//CLK由低到高产生一个上升沿，从而写入数据
		_nop_();		
	}
	for(i=0;i<8;i++)//循环8次，每次写1位，先写低位再写高位
	{
		DS1302_IO=dat&0x01;
		dat>>=1;
		DS1302_CLK=1;
		_nop_();
		DS1302_CLK=0;
		_nop_();		
	}
	DS1302_RST=0;//RST拉低
	_nop_();	
}

/*******************************************************************************
* 函 数 名       : ds1302_read_byte
* 函数功能		 : DS1302读单字节
* 输    入       : addr：地址/命令
* 输    出    	 : 读取的数据
*******************************************************************************/
u8 ds1302_read_byte(u8 addr)
{
	u8 i=0;
	u8 temp=0;
	u8 value=0;

	DS1302_RST=0;
	_nop_();	
	DS1302_CLK=0;//CLK低电平
	_nop_();
	DS1302_RST=1;//RST由低到高变化
	_nop_();
	
	for(i=0;i<8;i++)//循环8次，每次写1位，先写低位再写高位
	{
		DS1302_IO=addr&0x01;
		addr>>=1;	
		DS1302_CLK=1;
		_nop_();
		DS1302_CLK=0;//CLK由低到高产生一个上升沿，从而写入数据
		_nop_();		
	}
	for(i=0;i<8;i++)//循环8次，每次读1位，先读低位再读高位
	{
		temp=DS1302_IO;
		value=(temp<<7)|(value>>1);//先将value右移1位，然后temp左移7位，最后或运算
		DS1302_CLK=1;
		_nop_();
		DS1302_CLK=0;
		_nop_();		
	}
//	DS1302_RST=0;//RST拉低，当DS1302与DS18B20同时使用时，注释掉该调语句
	_nop_();	
	DS1302_CLK=1;//对于实物中，P3.4口没有外接上拉电阻的，此处代码需要添加，使数据口有一个上升沿脉冲。
	_nop_();
	DS1302_IO = 0;
	_nop_();
	DS1302_IO = 1;
	_nop_();	
	return value;		
}

/*******************************************************************************
* 函 数 名       : ds1302_init
* 函数功能		 : DS1302初始化时间
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void ds1302_init(void)
{
	u8 i=0;
	ds1302_write_byte(0x8E,0X00);
	for(i=0;i<7;i++)
	{
		ds1302_write_byte(gWRITE_RTC_ADDR[i],gDS1302_TIME[i]);	
	}
	ds1302_write_byte(0x8E,0X80);	
}

/*******************************************************************************
* 函 数 名       : ds1302_read_time
* 函数功能		 : DS1302读取时间
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void ds1302_read_time(void)
{
	u8 i=0;
	for(i=0;i<7;i++)
	{
		gDS1302_TIME[i]=ds1302_read_byte(gREAD_RTC_ADDR[i]);	
	}	
}
