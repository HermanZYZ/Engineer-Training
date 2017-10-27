#ifndef __FTC_FLYCONTROL_H
#define __FTC_FLYCONTROL_H

#include "FTC_Config.h"

#define FLYANGLE_MAX 150  //���������35�ȣ�ԭ��350

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

	Vector3i setVelocity;//�߶ȴ�����pid
	uint8_t velocityControl;//�߶�pid
	int32_t errorVelocityI;//�߶�pid

	Vector3i velPIDTerm;//�߶ȴ�����pid

	int32_t AltHold;//�߶ȱ���
	FTC_FlyControl();

	void PID_Reset(void);
	void AltHoldReset(void);

	//��̬�⻷����
	void Attitude_Outter_Loop(void);

	//��̬�ڻ�����
	void Attitude_Inner_Loop(void);

	//�߶��⻷����
	void Altitude_Outter_Loop(void);

	//�߶��ڻ�����
	void Altitude_Inner_Loop(void);

private:
	

	uint8_t rollPitchRate;//���ٶ�
	uint8_t yawRate;
	int32_t RateError[3];//���ٶ����
	Vector3i attPIDTerm;

	float Compensation;

	Vector3i velError;//�߶����
	int16_t altHoldDeadband;
	Vector3i AngleError;//�Ƕ����

};

extern FTC_FlyControl fc;

#endif























