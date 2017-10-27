/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Drv_LED.cpp
 * 描述    ：LED
**********************************************************************************/
#include "FTC_Drv_LED.h"

FTC_LED led;

void FTC_LED::Init(void)
{
	//To do
	GPIO_InitTypeDef  GPIO_InitStructure;//定义管脚初始化结构体
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE  );//开启PC口时钟
	GPIO_InitStructure.GPIO_Pin  = FTC_Pin_LED1 | FTC_Pin_LED2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//注意大小写
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
}

void FTC_LED::ON1(void)
{
	//To do	
	GPIO_SetBits(GPIOC, FTC_Pin_LED1);//输出高电平亮	
}

void FTC_LED::ON2(void)
{		
	//To do	
	GPIO_SetBits(GPIOC, FTC_Pin_LED2);//输出高电平亮	
}

void FTC_LED::OFF1(void)
{
	//To do
	GPIO_ResetBits(GPIOC, FTC_Pin_LED1);//输出高电平亮	
}

void FTC_LED::OFF2(void)
{
	//To do	
	GPIO_ResetBits(GPIOC, FTC_Pin_LED2);//输出高电平亮	
}


/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/

