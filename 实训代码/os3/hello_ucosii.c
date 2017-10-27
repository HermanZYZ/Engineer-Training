#include <stdio.h>
#include <io.h>
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include "ultrasound.c"
#include "system.h"
#include <stdlib.h>
#include "includes.h"


/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];


/* Definition of Task Priorities */

#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2
#define TASK3_PRIORITY      3

#define Address0 NEW_ULTRASOUND0_BASE
#define Address1 NEW_ULTRASOUND1_BASE
#define Address2 NEW_ULTRASOUND2_BASE

//占空比=duty/period;0x1388是5000个系统时钟周期,CPU是50MHz;duty1是通道一；enable是使能信号，用二进制表示，11代表两个通道都打开
unsigned long phase1=0x0,phase2=0x000,duty1=0x9c4,duty2=0x9c4,period1=0x1388,enable1=0x1;//3e8\9c4\fa0（20%\50%\80%)
unsigned long phase3=0x0,phase4=0x000,duty3=0x9c4,duty4=0x9c4,period2=0x1388,enable2=0x1;

volatile static char mode = 1;

void callback_func();

OS_EVENT* sem;

INT8U status;

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
		mode = 2;
	else
		mode=1;
}

void turn_left()
{
	duty2=0x9c4;
	duty3=0x9c4;
	enable1=0x2;
	enable2=0x1;
}

void turn_right()
{
	duty1=0x9c4;
	duty3=0x9c4;
	enable1=0x1;
	enable2=0x2;
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


void Task1(void* pdata)
{
  while (1)
  { 

  }
}


void Task2(void* pdata)
{
  while (1)
  {

}

void Task3 (void* pdata)
{
	while(1);
}


//int main(void)
//{
//
//  OSInit();
//
//  sem = OSSemCreate(1);
//
//  OSTaskCreate(   Task1,
//                  (void*)0,
//                  (void *)&task1_stk[TASK_STACKSIZE-1],
//                  TASK1_PRIORITY);
//
//  OSTaskCreate(   Task2,
//                  (void*)0,
//                  (void *)&task2_stk[TASK_STACKSIZE-1],
//                  TASK2_PRIORITY);
////  //start multitasking
//  OSStart();
//  return 0;
//}

void main(void)
{
	int i;
	OSInit();
	for(i=0; i<67; i++){
		INT8U temp =	OSTaskCreate(
						Task3,
						NULL, //argument to Task2()
						&TaskStk[i][1024], //unique stack!
						i); //task priority must be unique!
	printf("%d temp=%u\n",i,temp);
	if(temp!=0)
		break;
}
	printf("%d\n",i);
	OSStart();
}

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/
