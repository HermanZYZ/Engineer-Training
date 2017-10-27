/*
 * main.c
 *
 * Press Key0 to change the direction of the LED
 *
 *  Created on: Jul 1, 2013
 *  Modified on: 8 Aug, 2014
 *      Author: Gang
 */

#include "sys/alt_irq.h"
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "io.h"
#include <stdlib.h>

volatile static char key = 0;

void initialize(void);
void callback_func();

//DO NOT MODIFY THE MAIN FUNCTION!
int main()
{
	unsigned char led = 0x01;
	int delay = 0;

	IOWR(LED_BASE,0,0xff);

    initialize();

	while(1)
	{
		if (key) {
			if(led==0x80)
				led=0x01;
	        else
				led=led<<1;
		} else {
			if(led==0x01)
				led=0x80;
		    else
				led=led>>1;
		}
	    IOWR(LED_BASE,0,led);
	    delay=0;
        while(delay++ < 200000);
	}

  return 0;
}



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
	if(key==0x00)
		key=0x01;
	else
		key=0x00;
}


