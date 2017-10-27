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

/*
 * main.c
 *
 *  Created on: 07.11.2012
 *      Author: karl_ee
 */

#include "main.h"

int main()
{
	 unsigned int i,j;
//ռ�ձ�=duty/period;0x1388��5000��ϵͳʱ������,CPU��50MHz;duty1��ͨ��һ��enable��ʹ���źţ��ö����Ʊ�ʾ��11��������ͨ������
	 unsigned long phase1=0x0,phase2=0x000,duty1=0x3e8,duty2=0x9c4,period=0x1388,enable=0x3;//3e8\9c4\fa0��20%\50%\80%)
	while(1)
	{

		 motor_setting(phase1,duty1,phase2,duty2,period,enable);
			//phase1++;
			//phase2++;
			//duty1++;
			//duty2++;
			for(i=0;i<2550;i++)
				for(j=0;j<2550;j++);//delay

	}

}

//int main()
//{
//  while(1)
//  {
//	  int i=0;
//	  while(i<50)
//	  {
//		  IOWR(LED_BASE, 0, 0xff);
//		  IOWR(PIO_0_BASE, 0, 4);//4�Ķ�������100,��Ӧ����λ�ܽ�5,��ͨ
//		  i++;
//	  }
//	  while(i<100)
//	  {
//		  IOWR(LED_BASE, 0, 0);
//		  IOWR(PIO_0_BASE, 0, 0);
//		  i++;
//	  }
//  }
//
//  return 0;
//}

