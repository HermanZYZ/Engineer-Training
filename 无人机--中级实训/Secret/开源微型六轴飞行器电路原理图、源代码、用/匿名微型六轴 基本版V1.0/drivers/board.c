/******************** (C) COPYRIGHT 2014 ANO Tech ***************************
 * ����		 �������ƴ�
 * �ļ���  ��board.c
 * ����    ��Ӳ����ʼ��
 * ����    ��www.anotc.com
 * �Ա�    ��anotc.taobao.com
 * ����QȺ ��190169595
**********************************************************************************/
#include "board.h"

void SysTick_IRQ(void)
{
	scheduler.cnt_1ms++;
	scheduler.cnt_2ms++;
	scheduler.cnt_5ms++;
	scheduler.cnt_10ms++;
	scheduler.cnt_20ms++;
}

void ANO_Hexacopter_board_Init(void)
{
	//�ж����ȼ��������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	//��ʼ��ϵͳ�δ�ʱ��
	SysTick_Config(SystemCoreClock / 1000);	
	
	//��ʼ��ģ��I2C
	ANO_I2C_Soft::Init();
	//��ʼ��SPI2
	ANO_SPI2::Init();
	//��ʼ��NRF
	nrf.Init(MODEL_TX2,80);
	
	//��ʼ����ʱ�����PWM,24KHz
	ANO_PWM::out_Init(24000);
	
	//����flash
	FLASH_Unlock();	
	//��ʼ������eeprom����
	EE_Init();	
	
}



/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
