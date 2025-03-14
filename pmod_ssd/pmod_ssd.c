#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <unistd.h>

#define SSD_PIN_COUNT 8

int ssd_pins[SSD_PIN_COUNT] = {8, 10, 9, 11, 19, 21, 20, 18};

// Table des codes binaires pour les chiffres 0-9 (anode commune)
int digit_codes[10] = {
    0b0111111, // 0
    0b0000110, // 1
    0b1011011, // 2
    0b1001111, // 3
    0b1100110, // 4
    0b1101101, // 5
    0b1111101, // 6
    0b0000111, // 7
    0b1111111, // 8
    0b1101111  // 9
};

void display_digit(int number, int duration) {
    if (number < 0 || number > 99) return;

    int digit_1 = number / 10;
    int digit_2 = number % 10;

    printf("%d\n", digit_1);
    printf("%d\n", digit_2);
    

    int pattern_1 = 0b10000000 | digit_codes[digit_1];
    int pattern_2 = 0b00000000 | digit_codes[digit_2];

    printf("0b%b\n", pattern_1);
    printf("0b%b\n", pattern_2);

    for (int i = 0; i < duration*1000/2; i++) {
        for (int i = 0; i < SSD_PIN_COUNT; i++) {
            digitalWrite(ssd_pins[i], (pattern_1 >> i) & 1);
        }
        delay(1);
        for (int i = 0; i < SSD_PIN_COUNT; i++) {
            digitalWrite(ssd_pins[i], (pattern_2 >> i) & 1);
        }
        delay(1);
    }

}

void clear() {
    for (int i = 0; i < SSD_PIN_COUNT; i++) {
        digitalWrite(ssd_pins[i], 0);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <port> <valeur> <duree>\n", argv[0]);
        return 1;
    }

    int value = atoi(argv[2]);
    int duration = atoi(argv[3]);

    if (value < 0 || value > 99) {
        printf("Valeur initiale invalide (doit être entre 0 et 9)\n");
        return 1;
    }

    if (wiringPiSetupGpio() == -1) {
        printf("Erreur : impossible d'initialiser WiringPi\n");
        return 1;
    }

    // Définition des pins comme des output
    for (int i = 0; i < SSD_PIN_COUNT; i++) {
        pinMode(ssd_pins[i], OUTPUT);
    }

    display_digit(value, duration);

    // Éteindre l'afficheur
    clear();
    printf("Fin du programme.\n");

    return 0;
}
