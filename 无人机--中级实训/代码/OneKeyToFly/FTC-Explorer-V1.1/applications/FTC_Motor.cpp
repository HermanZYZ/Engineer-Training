/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��FTC_Motor.cpp
 * ����    �����������غ���
**********************************************************************************/
#include "FTC_Motor.h"

FTC_Motor motor;

void FTC_Motor::writeMotor(uint16_t throttle, int32_t pidTermRoll, int32_t pidTermPitch, int32_t pidTermYaw)
{
	static int counter = 0;
	static int fly = 0;	
	static bool last = false;
	if(rc.rawData[AUX2] < 1600 && rc.rawData[AUX2] > 1400 && !last)
	{
		fly = 1;
		last = true;
	}
	if(fly){
		if(counter < 500)//����2s�ڣ�������������
		{	
				counter++;
				throttle = 1520; 
				//if(throttle > MAXTHROTTLE) throttle = MAXTHROTTLE;
		}
		else if(counter <= 1050)//2��ʱ��
		{//����2s-6s  ���٣�
			counter++;
			if(counter % 50 == 0)
				throttle = 1500 - 20*(counter-500)/50;
			if(throttle <= 1400)
				throttle = 1400;
			//if(throttle < MINTHROTTLE ) throttle = MINTHROTTLE;
		}
		else if(counter > 1000)
		{//6s�� �ر�����
			fly = 0;
			counter++;//һֱ��������>300�������һֱ��ǰ�������
		}
	}

	//to do
	//����X��	
	/*motorPWM[0] = throttle + 0.5 * pidTermRoll - 0.866 *  pidTermPitch - pidTermYaw; //ǰ��
	motorPWM[1] = throttle - 0.5 * pidTermRoll + 0.866 *  pidTermPitch + pidTermYaw; //����
	motorPWM[2] = throttle - 0.5 * pidTermRoll - 0.866 *  pidTermPitch + pidTermYaw; //ǰ��
	motorPWM[3] = throttle + 0.5 * pidTermRoll + 0.866 *  pidTermPitch - pidTermYaw; //����	
	motorPWM[4] = throttle + pidTermRoll + pidTermYaw;	//��
	motorPWM[5] = throttle - pidTermRoll - pidTermYaw;	//��*/
	
	motorPWM[2] = throttle - 0.5 * pidTermRoll + 0.866 *  pidTermPitch + pidTermYaw; //
	motorPWM[1] = throttle - 0.5 * pidTermRoll - 0.866 *  pidTermPitch + pidTermYaw; //
	motorPWM[0] = throttle + 0.5 * pidTermRoll + 0.866 *  pidTermPitch - pidTermYaw; //
	motorPWM[3] = throttle + 0.5 * pidTermRoll - 0.866 *  pidTermPitch - pidTermYaw; //
	motorPWM[5] = throttle - pidTermRoll - pidTermYaw;	//
	motorPWM[4] = throttle + pidTermRoll + pidTermYaw;	//*/
		
	int16_t maxMotor = motorPWM[0];
	for (u8 i = 1; i < MAXMOTORS; i++)
	{
		if (motorPWM[i] > maxMotor)
					maxMotor = motorPWM[i];				
	}
	
	for (u8 i = 0; i < MAXMOTORS; i++) 
	{
		if (maxMotor > MAXTHROTTLE)    
			motorPWM[i] -= maxMotor - MAXTHROTTLE;	
		//���Ƶ��PWM����С�����ֵ
		motorPWM[i] = constrain_uint16(motorPWM[i], MINTHROTTLE, MAXTHROTTLE);
	}

	//���δ�������򽫵���������Ϊ���
	if(!ftc.f.ARMED)	
		ResetPWM();

	if(!ftc.f.ALTHOLD && rc.rawData[THROTTLE] < RC_MINCHECK && fly == 0)
		ResetPWM();

	//д����PWM
	pwm.SetPwm(motorPWM);
		
}

void FTC_Motor::getPWM(int16_t* pwm)
{
	*(pwm) = motorPWM[0];
	*(pwm+1) = motorPWM[1];
	*(pwm+2) = motorPWM[2];
	*(pwm+3) = motorPWM[3];
	*(pwm+4) = motorPWM[4];
	*(pwm+5) = motorPWM[5];	
}

void FTC_Motor::ResetPWM(void)
{
	for(u8 i=0; i< MAXMOTORS ; i++)
		motorPWM[i] = 1000;
}

/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
