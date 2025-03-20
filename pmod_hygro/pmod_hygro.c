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
#define SERIAL_REGISTER_1 0xFC
#define SERIAL_REGISTER_2 0xFD
#define SERIAL_REGISTER_3 0xFE

void read_register(int i2c_fd, uint8_t reg, uint8_t *buffer, size_t length) {
    if (write(i2c_fd, &reg, 1) != 1) {
        perror("Erreur lors de l'écriture de l'adresse du registre");
        return;
    }
    if (read(i2c_fd, buffer, length) != (ssize_t)length) {
        perror("Erreur lors de la lecture du registre");
        return;
    }
}

void read_sensor(int i2c_fd, uint8_t reg, uint16_t *data) {
    uint8_t buffer[2];
    usleep(6500);
    if (read(i2c_fd, buffer, 2) != 2) {
        perror("Erreur lors de la lecture des données");
        return;
    }
    *data = (buffer[0] << 8) | buffer[1];
}

void read_config_register(int i2c_fd) {
    uint8_t buffer[2];
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
    uint8_t serial1[2], serial2[2], serial3[2];
    read_register(i2c_fd, SERIAL_REGISTER_1, serial1, 2);
    read_register(i2c_fd, SERIAL_REGISTER_2, serial2, 2);
    read_register(i2c_fd, SERIAL_REGISTER_3, serial3, 2);
    uint64_t serial_id = ((uint64_t)(serial1[0] << 8 | serial1[1]) << 32) |
                          ((uint64_t)(serial2[0] << 8 | serial2[1]) << 16) |
                          (uint64_t)(serial3[0] << 8 | serial3[1]);
    printf("ID Capteur: %012llX\n", serial_id);
}

void read_manufacturer_id(int i2c_fd) {
    uint8_t manufacturer_id[2];
    read_register(i2c_fd, 0xFE, manufacturer_id, 2);
    uint16_t id = (manufacturer_id[0] << 8) | manufacturer_id[1];
    printf("Manufacturer ID: 0x%04X\n", id);
}

int main() {
    int i2c_fd;
    const char *i2c_device = "/dev/i2c-1";
    uint16_t temp_data, humidity_data;
    if ((i2c_fd = open(i2c_device, O_RDWR)) < 0) {
        perror("Erreur d'ouverture du bus I2C");
        return 1;
    }
    if (ioctl(i2c_fd, I2C_SLAVE, HYGRO_I2C_ADDRESS) < 0) {
        perror("Erreur de configuration de l'adresse I2C");
        close(i2c_fd);
        return 1;
    }
    uint8_t reset_command[]= { CONFIG_REGISTER, 0x80, 0x00};
    write(i2c_fd, reset_command,3);
    usleep(150000);
    read_sensor_id(i2c_fd);
    read_manufacturer_id(i2c_fd);
    read_sensor(i2c_fd, TEMP_REGISTER, &temp_data);
    printf("Température: %.2f °C\n", convert_temperature(temp_data));
    read_sensor(i2c_fd, HUMIDITY_REGISTER, &humidity_data);
    printf("Humidité: %.2f %%\n", convert_humidity(humidity_data));
    close(i2c_fd);
    return 0;
}