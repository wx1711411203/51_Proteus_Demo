//*********************************************************************************************************
//*********************************************************************************************************
//***<程序名>：跑马灯																					***
//***<功能>：通过2个按钮实现8个LED灯以5种不同闪烁模式和4种速度的切换。									***
//***<芯片>：AT89C51																					***
//***<作者>：LastRitter																					***
//***<完成时间>：2007年7月25日																			***
//***<联系方式>：E-mail:314665354@163.com																***
//*********************************************************************************************************
//*********************************************************************************************************


//*********************************************************************************************************
//*																										  *
//*			************************************头文件以及宏定义************************************	  *
//*																										  *
//*********************************************************************************************************

#include<reg51.h>

#define LEDPort P1

//*********************************************************************************************************
//*																										  *
//*			************************************全局变量定义************************************		  *
//*																										  *
//*********************************************************************************************************
unsigned char aucLED_SELECT_01[9]=
				{0xFF,0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F,};
											//方式0，方式1灯开关数组。
unsigned char aucLED_SELECT_23[5]=
				{0xFF,0x7E,0xBD,0xDB,0xE7};
											//方式2，方式3灯开关数组。

unsigned char _i=0;						    //数据选择计数。
											
unsigned char uc_Mode=0;					//模式选择，默认为模式0。

unsigned char uc_Count=1;					//定时器中断计数。

unsigned char uc_Delay=1;					//延迟计数，通过改变改数值改变灯状态延迟时间。
											//灯延迟时间=uc_Dalay*定时器溢出时间。



//*********************************************************************************************************
//*																										  *
//*			************************************主函数************************************				  *
//*																										  *
//*********************************************************************************************************
void main()
{													
	TMOD=0x01;								//定时器0模式一。
	TH0=0x3C;
	TL0=0xB0;								//溢出时间：50ms。
	TR0=1;									//定时器0开启。

	IT0=1;									//外部中断0下降沿触发。
	IT1=1;									//外部中断1下降沿触发。

	EX0=1;									//外部中断0开。
	EX1=1;									//外部中断1开。
	ET0=1;									//定时器0中断开。
	EA=1;									//总中断开。

	while(1);
}


//*********************************************************************************************************
//*																										  *
//*		************************************定时器0中断函数************************************			  *
//*																										  *
//*<函数功能>：当定时器0溢出时执行此函数，首先执行2个判断：												  *
//*				（1）if判断是否改变灯的状态，可用于改变延时时间；										  *
//*				（2）switch判断闪烁模式，根据不同模式对灯进行不同的通断。								  *
//*			最后对定时器TH0,TL0重新赋值，溢出时间为50ms。												  *
//*																										  *
//*********************************************************************************************************

void vTimer0(void) interrupt 1			    
{
	
	if(uc_Count%uc_Delay==0)			  //通过此判断语句来改变延时。，例如：当uc_Delay=6时，定时器										 
	{									  //溢出5次时灯状态改变1次。同时uc_Count重新赋值1.否则，uc_Count++
		uc_Count=1;						  //等待下一次溢出。因此可以改变延时。
		switch(uc_Mode)
			{
 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<模式0，灯从上往下依次点亮，不断循环。>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				case 0:					 
						if(_i>=8)
							_i=0;
						else _i++;
						LEDPort=aucLED_SELECT_01[_i];
						break;
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<模式1，灯从下往上依次点亮，不断循环。>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>		
				case 1:				
						if(i<=0)
							i=8;
						else _i--;

						LEDPort=aucLED_SELECT_01[_i];
						break;
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<模式2，灯从两边往中间依次点亮，不断循环。>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				case 2:				   	 
						if(_i>=4)
							_i=0;
						else _i++;

						LEDPort=aucLED_SELECT_23[_i];
						break;	
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<模式3，灯从中间往两边依次点亮，不断循环。>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				case 3:					 
						if(_i<=0)
							_i=4;
						else _i--;

						LEDPort=aucLED_SELECT_23[_i];
						break;
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<所有等以一定时间间隔同时闪烁。>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				case 4:				   
						if(LEDPort==0xFF)
							LEDPort=0x00;
						else LEDPort=0xFF;
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<其他>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				default:
						break;
			}
		}
		else uc_Count++;

	TH0=0x3C;	//重新给定时器赋值。
	TL0=0xB0;	//定时时间：50ms。
}





//*********************************************************************************************************
//*																										  *
//*		************************************外部中断0中断函数************************************		  *
//*																										  *
//*	<函数功能>：当外部中断0产生下降沿信号时执行此函数，将闪灯模式从模式0到4循环切换。						  *
//*																										  *
//*********************************************************************************************************

void vModeChange(void) interrupt 0
{
	if(uc_Mode>=4)
		uc_Mode=0;
	else uc_Mode++;
}





//*********************************************************************************************************
//*																										  *
//*		************************************外部中断1中断函数************************************		  *
//*																										  *
//*<函数功能>：当外部中断1产生下降沿信号时执行此函数，将闪灯时间从50ms，250ms，500ms，750ms循环切换。		  *
//*																										  *
//*********************************************************************************************************

void vDelayChange(void) interrupt 2
{
	if(uc_Delay>=16)
		uc_Delay=1;
	else uc_Delay+=5;
}




