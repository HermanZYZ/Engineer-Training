/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��FTC_FlyControl.cpp
 * ����    �����п���
**********************************************************************************/
#include "FTC_FlyControl.h"

FTC_FlyControl fc;

FTC_FlyControl::FTC_FlyControl()
{
	Throw = false;
	Landed = false;
	Fly_Time = 0;
	target_throttle = 0;
	max_throttle = 0;
	
	rollPitchRate = 150;
	yawRate = 50;
	
	altHoldDeadband = 100;
	
	//����PID����
	PID_Reset();
}

//����PID����
void FTC_FlyControl::PID_Reset(void)
{
	pid[PIDROLL].set_pid(0.15, 0.15, 0.02, 200);
	pid[PIDPITCH].set_pid(0.15, 0.15, 0.02, 200);
	pid[PIDYAW].set_pid(0.8, 0.45, 0, 200);
	pid[PIDANGLE].set_pid(5, 0, 0, 0);
	pid[PIDMAG].set_pid(2, 0, 0, 0);
 	pid[PIDVELZ].set_pid(1.5, 0.5, 0.002, 150);
 	pid[PIDALT].set_pid(1.2, 0, 0, 200);
}

//��������̬�⻷����
void FTC_FlyControl::Attitude_Outter_Loop(void)
{
	int32_t	errorAngle[2];
	Vector3f Gyro_ADC;
	
	//����Ƕ����ֵ
	errorAngle[ROLL] = constrain_int32((rc.Command[ROLL] * 2) , -((int)FLYANGLE_MAX), +FLYANGLE_MAX) - imu.angle.x * 10; 
	errorAngle[PITCH] = constrain_int32((rc.Command[PITCH] * 2) , -((int)FLYANGLE_MAX), +FLYANGLE_MAX) - imu.angle.y * 10; 
	errorAngle[ROLL] = applyDeadband(errorAngle[ROLL], 2);
	errorAngle[PITCH] = applyDeadband(errorAngle[PITCH], 2);
	
	//��ȡ���ٶ�
	Gyro_ADC = imu.Gyro_lpf / 4.0f;
	
	//�õ��⻷PID
	RateError[ROLL] = pid[PIDANGLE].get_p(errorAngle[ROLL]) - Gyro_ADC.x;
	RateError[PITCH] = pid[PIDANGLE].get_p(errorAngle[PITCH]) - Gyro_ADC.y;
	RateError[YAW] = ((int32_t)(yawRate) * rc.Command[YAW]) / 32 - Gyro_ADC.z;		
}
//��������̬�ڻ�����
void FTC_FlyControl::Attitude_Inner_Loop(void)
{
	int32_t PIDTerm[3];
	float tiltAngle = constrain_float( max(abs(imu.angle.x), abs(imu.angle.y)), 0 ,20);
	
	for(u8 i=0; i<3;i++)
	{
		//���ŵ��ڼ��ֵʱ����Ϊ��
		if ((rc.rawData[THROTTLE]) < RC_MINCHECK)	
			pid[i].reset_I();
		
		//�õ��ڻ�PID
		PIDTerm[i] = pid[i].get_pid(RateError[i], PID_INNER_LOOP_TIME*1e-6);
	}
	
	PIDTerm[YAW] = -constrain_int32(PIDTerm[YAW], -300 - abs(rc.Command[YAW]), +300 + abs(rc.Command[YAW]));
	
	if(Throw && Fly_Time < 500)//�׷ɵ�ʱ����������t=1��
	{
		rc.Command[THROTTLE] = max_throttle;
		Fly_Time++;
	}
	else if(Throw && Fly_Time < 1000)//����ʱ��3*t������2��
	{
		rc.Command[THROTTLE] = 1380;
		Fly_Time++;
	}
	else if(Throw)//�ص����ţ�ȷ������
	{
		rc.Command[THROTTLE] = 1100;
		Throw = false;
		Landed = true;
		Fly_Time = 0;
		max_throttle = 0;
	}
	if(ftc.f.ARMED && !Throw && !Landed)
	{
		if(imu.Acc.z < 2000)
		{
			rc.Command[THROTTLE] = max_throttle;
			Throw = true;
		}
		else if(imu.Acc.z > 8000 && imu.Acc.z <= 10000)//��������ȷ������ֵ
		{
			target_throttle = 1500;
		}
		else if(imu.Acc.z > 10000 && imu.Acc.z <= 12000)
		{
			target_throttle = 1600;
		}
		else if(imu.Acc.z > 12000)
		{
			target_throttle = 1700;
		}
		if(target_throttle > max_throttle)//��¼����׷������Լ���Ӧ����ֵ
			max_throttle = target_throttle; 
	}
	
	//������б����
	if(!ftc.f.ALTHOLD)
		rc.Command[THROTTLE] = (rc.Command[THROTTLE] - 1000) / cosf(radians(tiltAngle)) + 1000;
	
	//PID
	motor.writeMotor(rc.Command[THROTTLE], PIDTerm[ROLL], PIDTerm[PITCH], PIDTerm[YAW]);
}

//�������߶��⻷����
void FTC_FlyControl::Altitude_Outter_Loop(void)
{
	//to do
}

//�������߶��ڻ�����
void FTC_FlyControl::Altitude_Inner_Loop(void)
{
	//to do
}

void FTC_FlyControl::AltHoldReset(void)
{
	AltHold = nav.position.z;
}

/************************ (C) COPYRIGHT 2015 FTC *****END OF FILE**********************/
