#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define SPI_CHANNEL 0
#define SPI_SPEED 500000

int main() {
 
    wiringPiSetup();
    
    int fd;

    if((fd = wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED)) < 0) {
         printf("Erreur lors de l'initialisation du SPI");
         return -1;
    }

    unsigned char data[2] = {0};
    if(wiringPiSPIDataRW (SPI_CHANNEL, data, sizeof(data)) == -1) {
         printf("Erreur SPI\n");
    }

//    int brightness = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;
    //printf("brightness: 0x%X 0x%X", data[0], data[1], data[2], data[3]);
 
    for(int i = 0; i < sizeof(data); i++) { 
         printf("0x%X ", data[i]);
    }

    int value = data[1] | data[0] << 8;
    printf("\nValue=%d",  value >> 4);
 
    close(fd);

    return 0;
}
