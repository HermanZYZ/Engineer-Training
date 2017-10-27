#include "terasic_includes.h"
#include "adc_spi_read.h"
#include <stdio.h>
#include <stdint.h>
#include <io.h>
#include <unistd.h>

#define START_FLAG  0x8000
#define DONE_FLAG   0x8000

double ADC_Read(alt_u8 Channel) {
	IOWR(ADC_SPI_READ_BASE, 0, Channel);
	IOWR(ADC_SPI_READ_BASE, 0, Channel|START_FLAG);

	int done = 0;
	uint16_t spi_response;
	while (!done) {
		spi_response = IORD(ADC_SPI_READ_BASE, 0);
		done = (spi_response & DONE_FLAG);
	}
	uint16_t adc_value = spi_response & 0xFFF;
	return (double) adc_value * 3.3f / 4096.0f;
}

const uint16_t adc_channel = 2;

int main() {
	while (1) {
		int key = IORD(KEY_BASE,0);
		if (key != 3) {
			double ave=0;
			int i=5;
			while(i--)
				ave+=ADC_Read(adc_channel);

			printf("%lf\n", ave/5);
			//while (IORD(KEY_BASE,0) != 3);
		}
		usleep(500000);
	}
}

