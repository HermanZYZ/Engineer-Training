/******************** (C) COPYRIGHT 2014 ANO Tech ***************************
 * ����		 �������ƴ�
 * �ļ���  ��ANO_Filter.cpp
 * ����    ���˲�����غ���
 * ����    ��www.anotc.com
 * �Ա�    ��anotc.taobao.com
 * ����QȺ ��190169595
**********************************************************************************/
#include "ANO_Filter.h"


/*----------------------��ͨ�˲���ϵ������-------------------------*/
float ANO_Filter::LPF_1st_Factor_Cal(float deltaT, float Fcut)
{
	return deltaT / (deltaT + 1 / (2 * M_PI * Fcut));
}

/*----------------------һ�׵�ͨ�˲���------------------------*/
Vector3f ANO_Filter::LPF_1st(Vector3f oldData, Vector3f newData, float lpf_factor)
{
	return oldData * (1 - lpf_factor) + newData * lpf_factor;
}

/*----------------------�����˲���ϵ������-------------------------*/
float ANO_Filter::CF_Factor_Cal(float deltaT, float tau)
{
	return tau / (deltaT + tau);
}

/*----------------------һ�׻����˲���-----------------------------*/
Vector3f ANO_Filter::CF_1st(Vector3f gyroData, Vector3f accData, float cf_factor)
{ 
	return (gyroData * cf_factor + accData *(1 - cf_factor));	
}





/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
