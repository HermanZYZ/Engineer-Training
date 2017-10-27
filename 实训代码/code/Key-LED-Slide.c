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

/*
 SWITCH LED ACCORDING by KEYs
 */

#include "system.h"
#include "io.h"

int main()
{
  unsigned char keys;
  // turn on the first led
  IOWR(LED_BASE,0,0x01);
//  IOWR(LED_BASE,0,0x02);
//  IOWR(LED_BASE,0,0x04);
//  IOWR(LED_BASE,0,0x08);
//  IOWR(LED_BASE,0,0x10);
//  IOWR(LED_BASE,0,0x20);
//  IOWR(LED_BASE,0,0x40);
//  IOWR(LED_BASE,0,0x80);
  while(1)
  {
	  keys=IORD(KEY_BASE,0);
	  int keys_state = IORD(KEY_BASE,0);
	  while(keys_state==1)//press key1,state is 0;do not press ,that is 1;same as key0;
	  {
		  IOWR(LED_BASE,0,0xaa);
		  keys_state = IORD(KEY_BASE,0);
	  }
	  while(keys_state==2)
	  {
		  IOWR(LED_BASE,0,0x55);
		  keys_state = IORD(KEY_BASE,0);
	  }


	  int i;
	  i=0;
	  while(i<50000)
	  {
		  IOWR(LED_BASE,0,0x80);
		  i++;
	  }
	  i=0;
	  while(i<50000)
	  {
		  IOWR(LED_BASE,0,0x40);
		  i++;
	  }
	  i=0;
	  while(i<50000)
	  {
		  IOWR(LED_BASE,0,0x20);
		  i++;
	  }
	  i=0;
	  while(i<50000)
	  {
		  IOWR(LED_BASE,0,0x10);
		  i++;
	  }
	  i=0;
	  while(i<50000)
	  {
		  IOWR(LED_BASE,0,0x08);
		  i++;
	  }
	  i=0;
	  while(i<50000)
	  {
		  IOWR(LED_BASE,0,0x04);
		  i++;
	  }
	  i=0;
	  while(i<50000)
	  {
		  IOWR(LED_BASE,0,0x02);
		  i++;
	  }
	  i=0;
	  while(i<50000)
	  {
		  IOWR(LED_BASE,0,0x01);
		  i++;
	  }
	  // TODO: check if key 1 is pressed by checking if bit #1 is 1

      // TODO: if key 1 is pressed, make the next LED glow, such that
      //       your LEDs behave like the BIT slider from the exercises of day 2

      // TODO: make sure that one key press only causes one slide step,
      //       by reading KEY_BASE again and waiting the bit #1 to turn 0 again
  }
  return 0;
}

