/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_FlyControl.cpp
 * 描述    ：飞行控制
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
	
	//重置PID参数
	PID_Reset();
}

//重置PID参数
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

//飞行器姿态外环控制
void FTC_FlyControl::Attitude_Outter_Loop(void)
{
	int32_t	errorAngle[2];
	Vector3f Gyro_ADC;
	
	//计算角度误差值
	errorAngle[ROLL] = constrain_int32((rc.Command[ROLL] * 2) , -((int)FLYANGLE_MAX), +FLYANGLE_MAX) - imu.angle.x * 10; 
	errorAngle[PITCH] = constrain_int32((rc.Command[PITCH] * 2) , -((int)FLYANGLE_MAX), +FLYANGLE_MAX) - imu.angle.y * 10; 
	errorAngle[ROLL] = applyDeadband(errorAngle[ROLL], 2);
	errorAngle[PITCH] = applyDeadband(errorAngle[PITCH], 2);
	
	//获取角速度
	Gyro_ADC = imu.Gyro_lpf / 4.0f;
	
	//得到外环PID
	RateError[ROLL] = pid[PIDANGLE].get_p(errorAngle[ROLL]) - Gyro_ADC.x;
	RateError[PITCH] = pid[PIDANGLE].get_p(errorAngle[PITCH]) - Gyro_ADC.y;
	RateError[YAW] = ((int32_t)(yawRate) * rc.Command[YAW]) / 32 - Gyro_ADC.z;		
}
//飞行器姿态内环控制
void FTC_FlyControl::Attitude_Inner_Loop(void)
{
	int32_t PIDTerm[3];
	float tiltAngle = constrain_float( max(abs(imu.angle.x), abs(imu.angle.y)), 0 ,20);
	
	for(u8 i=0; i<3;i++)
	{
		//油门低于检查值时积分为零
		if ((rc.rawData[THROTTLE]) < RC_MINCHECK)	
			pid[i].reset_I();
		
		//得到内环PID
		PIDTerm[i] = pid[i].get_pid(RateError[i], PID_INNER_LOOP_TIME*1e-6);
	}
	
	PIDTerm[YAW] = -constrain_int32(PIDTerm[YAW], -300 - abs(rc.Command[YAW]), +300 + abs(rc.Command[YAW]));
	
	if(Throw && Fly_Time < 500)//抛飞的时候，上升飞行t=1秒
	{
		rc.Command[THROTTLE] = max_throttle;
		Fly_Time++;
	}
	else if(Throw && Fly_Time < 1000)//降落时间3*t，就是2秒
	{
		rc.Command[THROTTLE] = 1380;
		Fly_Time++;
	}
	else if(Throw)//关掉油门，确定降落
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
		else if(imu.Acc.z > 8000 && imu.Acc.z <= 10000)//根据力度确定油门值
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
		if(target_throttle > max_throttle)//记录最大抛飞力度以及相应油门值
			max_throttle = target_throttle; 
	}
	
	//油门倾斜补偿
	if(!ftc.f.ALTHOLD)
		rc.Command[THROTTLE] = (rc.Command[THROTTLE] - 1000) / cosf(radians(tiltAngle)) + 1000;
	
	//PID
	motor.writeMotor(rc.Command[THROTTLE], PIDTerm[ROLL], PIDTerm[PITCH], PIDTerm[YAW]);
}

//飞行器高度外环控制
void FTC_FlyControl::Altitude_Outter_Loop(void)
{
	//to do
}

//飞行器高度内环控制
void FTC_FlyControl::Altitude_Inner_Loop(void)
{
	//to do
}

void FTC_FlyControl::AltHoldReset(void)
{
	AltHold = nav.position.z;
}

/************************ (C) COPYRIGHT 2015 FTC *****END OF FILE**********************/
