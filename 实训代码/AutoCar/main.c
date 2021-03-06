/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include <io.h>
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
//#include "ultrasound.c"
#include "system.h"
#include <stdlib.h>

/*
 * main.c
 *
 *  Created on: 07.11.2012
 *      Author: karl_ee
 */

#include "main.h"
#include <unistd.h>

#define Address0 NEW_ULTRASOUND0_BASE
#define Address1 NEW_ULTRASOUND1_BASE
#define Address2 NEW_ULTRASOUND2_BASE

//占空比=duty/period;0x1388是5000个系统时钟周期,CPU是50MHz;duty1是通道一；enable是使能信号，用二进制表示，11代表两个通道都打开
unsigned long phase1=0x0,phase2=0x000,duty1=0x9c4,duty2=0x9c4,period1=0x1388,enable1=0x1;//3e8\9c4\fa0（20%\50%\80%)
unsigned long phase3=0x0,phase4=0x000,duty3=0x9c4,duty4=0x9c4,period2=0x1388,enable2=0x1;

volatile static char mode = 1;

void callback_func();

void initialize(void)
{
    //TODO set up interrupt handler for button presses
    //Do not use a while loop here!
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEY_BASE, 0x02);
	alt_ic_isr_register(KEY_IRQ_INTERRUPT_CONTROLLER_ID, KEY_IRQ, callback_func, NULL, 0 );
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEY_BASE,1);
}


//TODO create interrupt callback function that reverses the sliding pattern
void callback_func(){
   //Now handle the actual button press
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEY_BASE, 1);
	if(mode == 1)
	{
		duty1=0xdac;
		duty2=0xdac;
		duty3=0xdac;
		duty4=0xdac;
		mode = 2;
	}
	else
	{
		duty1=0x9c4;
		duty2=0x9c4;
		duty3=0x9c4;
		duty4=0x9c4;
		mode=1;
	}
}

void turn_left()
{
	duty2=0x9c4;
	duty3=0x9c4;
	enable1=0x0;
	enable2=0x1;
}

void turn_right()
{
	duty1=0x9c4;
	duty3=0x9c4;
	enable1=0x1;
	enable2=0x0;
}

void forward()
{
	duty1=0x9c4;
	duty3=0x9c4;
	enable1=0x1;
	enable2=0x1;
}

void backward()
{
	duty1=0x9c4;
	duty3=0x9c4;
	enable1=0x2;
	enable2=0x2;
}

void standby()
{
	enable1=0x0;
	enable2=0x0;
}

int main()
{
	 ultrasound_init(Address0);
	 ultrasound_init(Address1);
	 ultrasound_init(Address2);

	 initialize();//interrupts;
//	 while(1)
//	 {
//		 left_motor_setting(phase1,duty1,phase2,duty2,period1,enable1);//保留状态，一直按最近的状态执行
//		 right_motor_setting(phase3,duty3,phase4,duty4,period2,enable2);
//		 turn_left();
//
//	 }

	while(1)
	{
		left_motor_setting(phase1,duty1,phase2,duty2,period1,enable1);//保留状态，一直按最近的状态执行
		right_motor_setting(phase3,duty3,phase4,duty4,period2,enable2);
		forward();

		volatile double right = (double)ultrasound_read(Address0, 2)*340/1000.0f/1000.0f;
		volatile double left = (double)ultrasound_read(Address2, 2)*340/1000.0f/1000.0f;
		volatile double front = (double)ultrasound_read(Address1, 2)*340/1000.0f/1000.0f;

		printf("0: %f\n1: %f\n2: %f\n",left,front,right);

		if(mode==1)
		{
			if(front - 0.5 < 0.00001)
			{
				if(front - 0.2< 0.05)
				{
					backward();
					usleep(1000);
				}
				else if(left - 0.5 > 0.00001)
				{
					turn_left();
					usleep(1000);
//					forward();
				}
				else if(right - 0.5 > 0.00001)
				{
					turn_right();
					printf("*");
					usleep(1000);
//					forward();
				}
				else
				{
					if(left - 0.5 < 0.00001 && right - 0.5 < 0.00001)
					{
						backward();
					}
					if(left - 0.5 > 0.00001)
					{
						turn_left();
						printf("******");
						usleep(1000);
//						forward();
					}
					else if(right - 0.5 > 0.00001)
					{
						turn_right();
						printf("***");
						usleep(1000);
//						forward();
					}

				}
			}

			else if(left - 0.5 < 0.00001)
			{
				turn_right();
				printf("******");
				usleep(500);
//				forward();
			}
			else if(right - 0.5 <0.00001)
			{
				turn_left();
				usleep(500);
//				forward();
			}

		}
		if(mode==2)
		{
			if(front -0.5 < 0.1)
			{
				if(0.5 - front <0.1)
				{
					standby();
				}
				else
				{
					backward();
				}
			}

			if(front - 0.5 >0.1)
			{
				if(left - 0.8 <0.1)
				{
					turn_left();
				}
				else if(right - 0.8 <0.1)
				{
					turn_right();
				}
				else
				{
					forward();
				}
			}
		}


	}

}

