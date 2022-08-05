
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "ad9833_lib.h"
#include "spi.h"
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <linux/spi/spidev.h>
spi_t *spi;

static void AD9833_Write(uint16_t val)
{
        uint16_t tx = (val >> 8) | (val << 8);
        uint16_t rx;
        spi_transfer(spi, &tx, &rx, sizeof(val));
}

void AD9833_Wave(uint16_t Wave,float frequency,float Phase)
{
#if 0

        AD9833_Write(0x0100); // Reset

        AD9833_Write(0x1000); // MSB
        AD9833_Write(0x4000); // Freq 0 reg = 2300hz

        AD9833_Write(0x0000); // LSB
        AD9833_Write(0x5100); // Freq 0 reg = 2300hz

        AD9833_Write(0x2000); // Blockwave output

#else
        uint16_t MSB = 0;    // MSB of Frequency Tuning Word
	uint16_t LSB = 0;    // LSB of Frequency Tuning Word
	uint32_t phaseVal= 0;  // Phase Tuning Value
	long freq=0;
	// ---------- Tuning Word for Phase ( 0 - 360 Degree )


	if(Phase<0)Phase=0; // Changing Phase Value to Positive
	else if(Phase>360)Phase =360; // Maximum value For Phase (In Degree)
	phaseVal  = ((int)(Phase*(4096/360)))|0XC000;  // 4096/360 = 11.37 change per Degree for Register And using 0xC000 which is Phase 0 Register Address

	// ---------- Tuning word for Frequency
	//if (freq >(FMCLK>>1)) freq = FMCLK>>1;
	//else if (freq <0) freq = 0;

	freq= ((frequency*pow(2,28))/FMCLK); // Tuning Word

	MSB = ((freq & 0xFFFC000)>>14);
	LSB = ((freq & 0x3FFF));
	LSB |= 0x4000;
	MSB |= 0x4000;

        uint16_t tx;

        AD9833_Write(0x0100);

        //tx = 0x2100;
        //spi_transfer(spi, &tx, NULL, 16);
        //asm("NOP");

        //tx = FRQLW;
        //spi_transfer(spi, &tx, NULL, 16);
        //asm("NOP");

        AD9833_Write(0x1000);
        AD9833_Write(MSB);

        AD9833_Write(0x0000);
        AD9833_Write(LSB);

        //tx = FRQHW;
        //spi_transfer(spi, &tx, NULL, 16);
        //asm("NOP");
        //spi_transfer(spi, (uint32_t)Phase, NULL, 16); ????
        //asm("NOP");

        AD9833_Write(Phase);

        switch(Wave){
  	      case 0:
  	      	 Wave = SIN;//SIN
  	        break;
  	      case 1:
  	      	Wave = SQR;//SQR
  	      	break;
  	      case 2:
  	      	Wave = TRI;//TRI
  	      	break;
  	      default:
  	        break;
  	      }
        printf("shifted in Wave: 0x%02x\n", Wave);
        printf("shifted in FRQ: 0x%02x\n", freq);
        printf("shifted in Phase: 0x%02x\n", Phase);

        AD9833_Write(Wave);

        //printf("shifted in: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", Data[0], Data[1], Data[2], Data[3], Data[4], Data[5]);
#endif
}

void AD9833_Freq_SIN(float data)
{
uint16_t data_h,data_l;
uint32_t data_new;

data_new = 10.73741824 * data;
data_h = ((data_new>>14)&0x3fff)|0x4000;
data_l = (data_new&0x3fff)|0x4000;

spi_transfer(spi, 0x0100, NULL, 16);
spi_transfer(spi, 0x2100, NULL, 16);
spi_transfer(spi, data_l, NULL, 16);
spi_transfer(spi, data_h, NULL, 16);
spi_transfer(spi, 0x2000, NULL, 16);
}

void AD9833_Init()
{
        unsigned int mode = SPI_MODE_3;
        spi_bit_order_t bit_order = MSB_FIRST;
        uint8_t bits_per_word = 16;
        //uint32_t max_speed = 1000000;
        uint32_t max_speed = 500000;

        spi_set_mode(spi, mode);
        spi_set_max_speed(spi, max_speed);
        spi_set_bit_order(spi, bit_order);
        spi_set_bits_per_word(spi, bits_per_word);

        printf("mode: %d\n", mode);
        printf("max speed: %d\n", max_speed);
        printf("bit_order: %d\n", bit_order);
        printf("bits per word: %d\n", bits_per_word);

        spi_open(spi, "/dev/spidev0.0", mode, max_speed);

 if (spi_open(spi, "/dev/spidev0.0", mode, max_speed) < 0) {
        printf("spi_open(): %s\n", spi_errmsg(spi));
        exit(1);
    }
    usleep(10);
}
