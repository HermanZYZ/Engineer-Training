/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Motor.cpp
 * 描述    ：电机控制相关函数
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
		if(counter < 500)//按键2s内，油门匀速上涨
		{	
				counter++;
				throttle = 1520; 
				//if(throttle > MAXTHROTTLE) throttle = MAXTHROTTLE;
		}
		else if(counter <= 1050)//2秒时间
		{//按键2s-6s  减速；
			counter++;
			if(counter % 50 == 0)
				throttle = 1500 - 20*(counter-500)/50;
			if(throttle <= 1400)
				throttle = 1400;
			//if(throttle < MINTHROTTLE ) throttle = MINTHROTTLE;
		}
		else if(counter > 1000)
		{//6s后 关闭油门
			fly = 0;
			counter++;//一直让她保持>300，否则会一直在前面的条件
		}
	}

	//to do
	//六轴X型	
	/*motorPWM[0] = throttle + 0.5 * pidTermRoll - 0.866 *  pidTermPitch - pidTermYaw; //前左
	motorPWM[1] = throttle - 0.5 * pidTermRoll + 0.866 *  pidTermPitch + pidTermYaw; //后右
	motorPWM[2] = throttle - 0.5 * pidTermRoll - 0.866 *  pidTermPitch + pidTermYaw; //前右
	motorPWM[3] = throttle + 0.5 * pidTermRoll + 0.866 *  pidTermPitch - pidTermYaw; //后左	
	motorPWM[4] = throttle + pidTermRoll + pidTermYaw;	//左
	motorPWM[5] = throttle - pidTermRoll - pidTermYaw;	//右*/
	
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
		//限制电机PWM的最小和最大值
		motorPWM[i] = constrain_uint16(motorPWM[i], MINTHROTTLE, MAXTHROTTLE);
	}

	//如果未解锁，则将电机输出设置为最低
	if(!ftc.f.ARMED)	
		ResetPWM();

	if(!ftc.f.ALTHOLD && rc.rawData[THROTTLE] < RC_MINCHECK && fly == 0)
		ResetPWM();

	//写入电机PWM
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
