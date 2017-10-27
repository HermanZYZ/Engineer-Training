/******************** (C) COPYRIGHT 2015 FTC *******************************
 * 作者		 ：FTC
 * 文件名  ：FTC_IMU.cpp
 * 描述    ：飞行器姿态计算
**********************************************************************************/
#include "FTC_IMU.h"

FTC_IMU imu; 

FTC_IMU::FTC_IMU()
{
}

//IMU初始化
void FTC_IMU::Init()
{
	//滤波器参数初始化
	filter_Init();
	//传感器初始化
	sensor_Init();	
}

//更新传感器数据
void FTC_IMU::updateSensor()
{
	//读取加速度
	mpu6050.Read_Acc_Data();
	//读取角速度
	mpu6050.Read_Gyro_Data();	
	//获取角速度，单位为度每秒
	Gyro = mpu6050.Get_Gyro();
	//获取加速度采样值
	Acc = mpu6050.Get_Acc();
}


//计算飞行器姿态
void FTC_IMU::getAttitude()
{
	float deltaT;
	Vector3d accTemp, gyroTemp;
	
#ifdef FTC_IMU_USE_LPF_1st	
	//加速度数据一阶低通滤波
	Acc_lpf = LPF_1st(Acc_lpf, Acc, ftc.factor.acc_lpf);
#endif	
	
#ifdef FTC_IMU_USE_LPF_2nd	
	//加速度数据二阶低通滤波
	Acc_lpf = LPF_2nd(&Acc_lpf_2nd, Acc);
#endif
	
	//陀螺仪数据二阶低通滤波
	Gyro_lpf = LPF_2nd(&Gyro_lpf_2nd, Gyro);
	
#ifdef FTC_IMU_USE_LPF_4th
	//加速度数据四阶低通滤波
	accTemp(double(Acc.x), double(Acc.y), double(Acc.z));
	accTemp = LPF_Butterworth_4th(accTemp, &Acc_lpf_4th);
	Acc_lpf(float(accTemp.x),float(accTemp.y),float(accTemp.z));
#endif
	

	//计算实际测量的加速度和重力加速度的比值
	accRatio = Acc_lpf.length_squared() * 100 / (ACC_1G * ACC_1G);		
	
	deltaT = getDeltaT(GetSysTime_us());
	
#ifdef FTC_IMU_USE_DCM_CF
	DCM_CF(mpu6050.Get_Gyro_in_dps(Gyro_lpf),Acc_lpf,deltaT);
#endif
#ifdef FTC_IMU_USE_Quaternions_CF
	Quaternion_CF(mpu6050.Get_Gyro_in_dps(Gyro_lpf),Acc_lpf,deltaT);
#endif
}

//获取飞行器的加速度在地理坐标系的投影
Vector3f FTC_IMU::Get_Accel_Ef(void)
{
	Matrix3f dcm;
	Vector3f anglerad;
	
	//姿态角转弧度
	anglerad(-radians(angle.x), -radians(angle.y), radians(angle.z));	//没写错，z轴符号为+才是对的

	//计算表示旋转的余弦矩阵
	dcm.from_euler(anglerad);
	
	return dcm * Acc_lpf;
}

//余弦矩阵更新姿态
void FTC_IMU::DCM_CF(Vector3f gyro,Vector3f acc, float deltaT)
{
	//to do
	//acc.get_rollpitch(angle);
	//acc.get_yaw(angle);
	Vector3f gyroint;
	static Vector3f pre_gyro;
	
	gyroint = (pre_gyro + gyro)*0.5*deltaT;
	pre_gyro = gyro;
	
	Matrix3f co;
	co.from_euler(gyroint);
	
	static Vector3f vector_gravity(0,0,4096);
  static Vector3f vector_yaw(200,0,0);
	vector_gravity = co*vector_gravity;
	vector_gravity = CF_1st(vector_gravity, acc, ftc.factor.gyro_cf);
	vector_yaw = co*vector_yaw;
	
	vector_gravity.get_rollpitch(angle);
	vector_yaw.get_yaw(angle);
	//angle.x = atan2(acc.x,sqrt(acc.y*acc.y + acc.z*acc.z));
	//angle.y = atan2(acc.y,sqrt(acc.x*acc.x + acc.z*acc.z));
	//angle.z = atan2(sqrt(acc.x*acc.x + acc.y*acc.y),acc.z);
}

#define Kp 0.1f        //加速度权重，越大则向加速度测量值收敛越快
#define Ki 0.001f      //误差积分增益
//四元数更新姿态
void FTC_IMU::Quaternion_CF(Vector3f gyro,Vector3f acc, float deltaT)
{
	//to do
	Vector3f V_gravity, V_error;
	static Vector3f	V_error_I(0,0,0);
	
	//重力加速度归一化
	acc.normalize();//向量的方向没有变，归一化的目的是符合单位四元数才能用于旋转方向的约束性质
	
	//提取四元数的等效余弦矩阵中的重力分量
	Q.vector_gravity(V_gravity);//上一次的姿态，保存在等效余弦矩阵
	
	//向量叉积得出姿态误差
	V_error = acc % V_gravity;//和当前的姿态差
	
	//对误差进行积分	
	V_error_I += V_error * Ki;
	
	//互补滤波，姿态误差补偿到角速度上，修正角速度积分漂移
	gyro += V_error * Kp + V_error_I;	//梯度下降法
	
	//一阶龙格库塔法更新四元数
	Q.Runge_Kutta_1st(gyro, deltaT);
	
	//四元数归一化
	Q.normalize();//单位
	
	//四元数转欧拉角
	Q.to_euler(&angle.x, &angle.y, &angle.z);
}

void FTC_IMU::filter_Init()
{
	//加速度一阶低通滤波器系数计算
	ftc.factor.acc_lpf = LPF_1st_Factor_Cal(IMU_LOOP_TIME * 1e-6, ACC_LPF_CUT);
	
	//加速度二阶低通滤波器系数计算
	LPF_2nd_Factor_Cal(IMU_LOOP_TIME * 1e-6, ACC_LPF_CUT, &Acc_lpf_2nd);
	
	//陀螺仪二阶低通滤波器系数计算	
	LPF_2nd_Factor_Cal(IMU_LOOP_TIME * 1e-6, GYRO_LPF_CUT, &Gyro_lpf_2nd);	
	
	//互补滤波器系数计算
	ftc.factor.gyro_cf = CF_Factor_Cal(IMU_LOOP_TIME * 1e-6, GYRO_CF_TAU);	
}

void FTC_IMU::sensor_Init()
{
	//初始化MPU6050，1Khz采样率，98Hz低通滤波
	mpu6050.Init(1000,98);
}

float FTC_IMU::getDeltaT(uint32_t currentT)
{
	static uint32_t previousT;
	float	deltaT = (currentT - previousT) * 1e-6;	
	previousT = currentT;
	
	return deltaT;
}

/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
