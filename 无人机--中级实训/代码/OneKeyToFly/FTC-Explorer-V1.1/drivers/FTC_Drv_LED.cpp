/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��FTC_Drv_LED.cpp
 * ����    ��LED
**********************************************************************************/
#include "FTC_Drv_LED.h"

FTC_LED led;

void FTC_LED::Init(void)
{
	//To do
	GPIO_InitTypeDef  GPIO_InitStructure;//����ܽų�ʼ���ṹ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE  );//����PC��ʱ��
	GPIO_InitStructure.GPIO_Pin  = FTC_Pin_LED1 | FTC_Pin_LED2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//ע���Сд
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
}

void FTC_LED::ON1(void)
{
	//To do	
	GPIO_SetBits(GPIOC, FTC_Pin_LED1);//����ߵ�ƽ��	
}

void FTC_LED::ON2(void)
{		
	//To do	
	GPIO_SetBits(GPIOC, FTC_Pin_LED2);//����ߵ�ƽ��	
}

void FTC_LED::OFF1(void)
{
	//To do
	GPIO_ResetBits(GPIOC, FTC_Pin_LED1);//����ߵ�ƽ��	
}

void FTC_LED::OFF2(void)
{
	//To do	
	GPIO_ResetBits(GPIOC, FTC_Pin_LED2);//����ߵ�ƽ��	
}


/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/

