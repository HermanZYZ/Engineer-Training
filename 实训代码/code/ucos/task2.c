/*************************************************************************
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
* All rights reserved. All use of this software and documentation is     *
* subject to the License Agreement located at the end of this file below.*
**************************************************************************
* Description:                                                           *
* The following is a simple hello world program running MicroC/OS-II.The * 
* purpose of the design is to be a very simple application that just     *
* demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
* for issues such as checking system call return codes. etc.             *
*                                                                        *
* Requirements:                                                          *
*   -Supported Example Hardware Platforms                                *
*     Standard                                                           *
*     Full Featured                                                      *
*     Low Cost                                                           *
*   -Supported Development Boards                                        *
*     Nios II Development Board, Stratix II Edition                      *
*     Nios Development Board, Stratix Professional Edition               *
*     Nios Development Board, Stratix Edition                            *
*     Nios Development Board, Cyclone Edition                            *
*   -System Library Settings                                             *
*     RTOS Type - MicroC/OS-II                                           *
*     Periodic System Timer                                              *
*   -Know Issues                                                         *
*     If this design is run on the ISS, terminal output will take several*
*     minutes per iteration.                                             *
**************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include "includes.h"


/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
OS_STK    TaskStk[67][1024];


/* Definition of Task Priorities */

#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2

const char msg1[]={89,111,117,32,115,111,108,118,101,100,32,116,104,105,115,32,116,97,115,107,33,10};
const char msg2[]={71,111,111,100,32,106,111,98,33,32,89,111,117,32,97,114,101,32,97,119,101,115,111,109,101,33,10};

OS_EVENT* sem;

INT8U status;

void Task1(void* pdata)
{
  while (1)
  { 
	OSSemPend(sem, 0, &status);
	int i;
	for(i=0; i<sizeof(msg1); i++)
	{
		printf("%c",msg1[i]);
		OSTimeDlyHMSM(0, 0, 0, 250);
	}
	OSSemPost(sem);
  }
}


void Task2(void* pdata)
{
  while (1)
  {
	OSSemPend(sem, 10000, &status);
	int i;
	for(i=0; i<sizeof(msg2); i++)
	{
		printf("%c",msg2[i]);
		OSTimeDlyHMSM(0, 0, 0, 250);
	}
	OSSemPost(sem);
  }
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

