#ifndef __FTC_SCHEDULER_H
#define __FTC_SCHEDULER_H

#include "board.h"
#include "FTC_Config.h"

class FTC_Scheduler
{
public:
	//构造函数
	FTC_Scheduler();
	//任务时基计数变量
	uint16_t cnt_1ms,cnt_2ms,cnt_5ms,cnt_10ms,cnt_20ms,cnt_500ms;

	int16_t pre_TURNNING_ANGLE;
	int16_t cur_TURNNING_ANGLE;
	uint8_t COUNT;
	bool THROW;//判断是否甩出去

};

void FTC_Loop(void);

extern FTC_Scheduler scheduler;


#endif
