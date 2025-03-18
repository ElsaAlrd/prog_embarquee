#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <unistd.h>
#include <pthread.h>

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

// Variables globales
int current_value = 0;   // Valeur affichée sur l'écran
int running = 1;         // Indique si le thread doit continuer à tourner

// Fonction qui gère l'affichage sur l'afficheur 7 segments
void *display_thread(void *arg) {
    while (running) {
        int digit_1 = current_value / 10;
        int digit_2 = current_value % 10;

        int pattern_1 = 0b10000000 | digit_codes[digit_1];
        int pattern_2 = 0b00000000 | digit_codes[digit_2];

        // Affichage alterné des 2 chiffres
        for (int i = 0; i < SSD_PIN_COUNT; i++) {
            digitalWrite(ssd_pins[i], (pattern_1 >> i) & 1);
        }
        delay(5);
        
        for (int i = 0; i < SSD_PIN_COUNT; i++) {
            digitalWrite(ssd_pins[i], (pattern_2 >> i) & 1);
        }
        delay(5);
    }

    return NULL;
}

void clear() {
    for (int i = 0; i < SSD_PIN_COUNT; i++) {
        digitalWrite(ssd_pins[i], 0);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <valeur>\n", argv[0]);
        return 1;
    }

    int max_value = atoi(argv[1]);

    if (max_value < 0 || max_value > 99) {
        printf("Valeur initiale invalide (doit être entre 0 et 99)\n");
        return 1;
    }

    if (wiringPiSetupGpio() == -1) {
        printf("Erreur : impossible d'initialiser WiringPi\n");
        return 1;
    }

    // Définition des pins comme output
    for (int i = 0; i < SSD_PIN_COUNT; i++) {
        pinMode(ssd_pins[i], OUTPUT);
    }

    // Création du thread d'affichage
    pthread_t display_tid;
    pthread_create(&display_tid, NULL, display_thread, NULL);

    // Boucle de comptage
    for (current_value = 0; current_value <= max_value; current_value++) {
        sleep(1);  // Attente d'1 seconde entre chaque incrémentation
    }

    // Arrêter le thread d'affichage
    running = 0;
    pthread_join(display_tid, NULL);

    // Éteindre l'afficheur
    clear();
    printf("Fin du programme.\n");
    return 0;
}
