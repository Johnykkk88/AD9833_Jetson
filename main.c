#include <stdlib.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include "spi.h"
#include "gpio.h"
#include "ad9833_lib.h"

spi_t *spi;

int main(void) {

 spi = spi_new();
 AD9833_Init();

 float frequency = 0;
 /*char signal_form [3];
 printf("Signal form: , frequency: \n");
 scanf("%s %f", signal_form, &frequency);
 printf("Passing this data to the function: %s %f\n", signal_form, frequency);
 int comparison_result = strcmp(signal_form, "SIN");
 printf("The string comparison result is: %d\n", comparison_result);
 if (signal_form == "SIN")
  AD9833_Wave(SIN, frequency, 0.0);
  else if (signal_form == "TRI")
   AD9833_Wave(TRI, frequency, 0.0);
   else if (signal_form == "SQR")
     AD9833_Wave(SQR, frequency, 0.0);
    else
    AD9833_Wave(SIN, frequency, 0.0);*/

AD9833_Wave(SQR, 1000.0, 0.0);
//   printf("shifted in: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", Data[0], Data[1], Data[2], Data[3], Data[4], Data[5]);

 spi_close(spi);
 spi_free(spi);

    return 0;
}
