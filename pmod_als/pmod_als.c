#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define SPI_CHANNEL 1
#define SPI_SPEED 500000

// Fonction pour lire la valeur de luminosité du PMOD ALS
uint8_t read_light_sensor() {
    uint8_t buffer[2] = {0x00, 0x00};

    if (wiringPiSPIDataRW(SPI_CHANNEL, buffer, 2) < 0) {
        perror("Erreur SPI");
        return 0;
    }

    uint16_t light_value = ((buffer[0]) << 8) | buffer[1];

    return light_value>>4;
}

int main() {

    // Initialiser le SPI
    if (wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) < 0) {
        perror("Erreur SPI initialisation");
        return 1;
    }

    // Lire et afficher la valeur de luminosité
    uint8_t light_value = read_light_sensor();
    printf("Luminosité : %d\n", light_value);

    wiringPiSPIClose(0);
    wiringPiSPIClose(1);
    return 0;
}
