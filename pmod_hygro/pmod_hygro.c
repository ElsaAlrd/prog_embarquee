#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define HYGRO_I2C_ADDRESS 0x40
#define TEMP_REGISTER 0x00
#define HUMIDITY_REGISTER 0x01
#define CONFIG_REGISTER 0x02

void read_sensor(int i2c_fd, uint8_t reg, uint16_t *data) {
    uint8_t buffer[2];

    // Écriture du registre cible
    if (write(i2c_fd, &reg, 1) != 1) {
        perror("Erreur lors de l'écriture du registre");
        return;
    }

    // Attente pour la conversion (6.50 ms pour 14 bits)
    usleep(6500);

    // Lecture des 2 octets de données
    if (read(i2c_fd, buffer, 2) != 2) {
        perror("Erreur lors de la lecture des données");
        return;
    }

    *data = (buffer[0] << 8) | buffer[1];
}

void read_config_register(int i2c_fd) {
    uint8_t reg = 0x02; // Registre de configuration
    uint8_t buffer[2];

    if (write(i2c_fd, &reg, 1) != 1) {
        perror("Erreur lors de l'écriture du registre");
        return;
    }

    if (read(i2c_fd, buffer, 2) != 2) {
        perror("Erreur lors de la lecture du registre");
        return;
    }

    printf("Config Register: %02X %02X\n", buffer[0], buffer[1]);
}

float convert_temperature(uint16_t raw_data) {
    return ((165.0 * raw_data) / 65536.0) - 40.0;
}

float convert_humidity(uint16_t raw_data) {
    return ((100.0 * raw_data) / 65536.0);
}

void read_sensor_id(int i2c_fd) {
    uint8_t cmd[2] = {0xFC, 0xC9}; // Commande pour lire l'ID
    uint8_t buffer[8];

    if (write(i2c_fd, cmd, 2) != 2) {
        perror("Erreur lors de l'écriture pour l'ID");
        return;
    }

    if (read(i2c_fd, buffer, 8) != 8) {
        perror("Erreur lors de la lecture de l'ID");
        return;
    }

    printf("ID Capteur: %02X %02X %02X %02X\n", buffer[0], buffer[1], buffer[2], buffer[3]);
}

int main() {

    int i2c_fd;
    const char *i2c_device = "/dev/i2c-1"; // Modifier si autre bus I2C
    uint16_t temp_data, humidity_data;

    // Ouverture du bus I2C
    if ((i2c_fd = open(i2c_device, O_RDWR)) < 0) {
        perror("Erreur d'ouverture du bus I2C");
        return 1;
    }

    // Configuration de l'adresse I2C du capteur
    if (ioctl(i2c_fd, I2C_SLAVE, HYGRO_I2C_ADDRESS) < 0) {
        perror("Erreur de configuration de l'adresse I2C");
        close(i2c_fd);
        return 1;
    }

    // Réveil du capteur (soft reset)
    uint8_t reset_command[]= { CONFIG_REGISTER, 0x80, 0x00}; // Commande pour réveiller le capteur
    write(i2c_fd, reset_command,3);
    usleep(150000); // Délai pour le réveil du capteur 

    // Lire l'ID du capteur
    read_sensor_id(i2c_fd);

    // Lecture de la température
    read_sensor(i2c_fd, TEMP_REGISTER, &temp_data);
    printf("Température: %.2f °C\n", convert_temperature(temp_data));

    // Lecture de l'humidité
    read_sensor(i2c_fd, HUMIDITY_REGISTER, &humidity_data);
    printf("Humidité: %.2f %%\n", convert_humidity(humidity_data));

    // Fermeture du bus I2C
    close(i2c_fd);

    return 0;
}
