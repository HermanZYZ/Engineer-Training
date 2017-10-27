
#include "system.h"
#include "io.h"

/*
*Correct the below mentioned code to get an expected result: If the switch is pressed, LEDs should turn on
*/


int * key_ptr;

int main()
{
    key_ptr = (int *) KEY_BASE;
 
	IOWR(LED_BASE,0,0x00);

    
	while (*key_ptr == 3);
 
	IOWR(LED_BASE,0,0xff);

  return 0;
}
