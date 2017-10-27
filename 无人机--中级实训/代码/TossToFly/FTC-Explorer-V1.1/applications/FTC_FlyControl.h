#ifndef __FTC_FLYCONTROL_H
#define __FTC_FLYCONTROL_H

#include "FTC_Config.h"

#define FLYANGLE_MAX 150  //最大飞行倾角35度，原本350

enum {
    PIDROLL,
    PIDPITCH,
    PIDYAW,
    PIDANGLE,
    PIDMAG,
    PIDVELZ,
    PIDALT,
		PIDITEMS
};

class FTC_FlyControl
{

public:
	
	FTC_PID pid[PIDITEMS];

	bool Toss;
	bool Landed;
	uint16_t FlyTime;
	int16_t TargetThrottle;

	Vector3i setVelocity;//高度传出的pid
	uint8_t velocityControl;//高度pid
	int32_t errorVelocityI;//高度pid

	Vector3i velPIDTerm;//高度传出的pid

	int32_t AltHold;//高度保持
	FTC_FlyControl();

	void PID_Reset(void);
	void AltHoldReset(void);

	//姿态外环控制
	void Attitude_Outter_Loop(void);

	//姿态内环控制
	void Attitude_Inner_Loop(void);

	//高度外环控制
	void Altitude_Outter_Loop(void);

	//高度内环控制
	void Altitude_Inner_Loop(void);

private:
	

	uint8_t rollPitchRate;//角速度
	uint8_t yawRate;
	int32_t RateError[3];//角速度误差
	Vector3i attPIDTerm;

	float Compensation;

	Vector3i velError;//高度误差
	int16_t altHoldDeadband;
	Vector3i AngleError;//角度误差

};

extern FTC_FlyControl fc;

#endif























