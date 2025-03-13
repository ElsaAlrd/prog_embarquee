#include <stdio.h>
#include <wiringPi.h>
#include <string.h>

#define SWITCH_COUNT 8  // Nombre d'interrupteurs
#define LED_COUNT 8     // Nombre de LED

int switch_pins[SWITCH_COUNT] = {8, 10, 9, 11, 19, 21, 20, 18};
int led_pins[LED_COUNT] = {16, 14, 15, 17, 4, 12, 5, 6};

int read_switches(int argc, char *argv[]) {
    int switch_state = 0;

    if (argc > 1 && strcmp(argv[1], "JA") == 0) {
        printf("Port JA sélectionné !\n");
    } else {
        printf("Port inconnu : %s\n", argv[1]);
    }

    // Initialisation de WiringPi
    if (wiringPiSetupGpio() == -1) { // Utilisation de la numérotation BCM
        perror("Erreur : impossible d'initialiser WiringPi");
        return 1;
    }

    // Configuration des broches en entrée
    for (int i = 0; i < SWITCH_COUNT; i++) {
        pinMode(switch_pins[i], INPUT);
        pullUpDnControl(switch_pins[i], PUD_DOWN); // Activation de la résistance pull-down
    }

    // Lire l'état des interrupteurs
    for (int i = 0; i < SWITCH_COUNT; i++) {
        if (digitalRead(switch_pins[i]) == HIGH) {
            switch_state |= (1 << i);
        }
    }

    printf("État des interrupteurs : 0x%X\n", switch_state);
    
    return switch_state;
}

void control_leds(unsigned char hex_value) {
    // Initialisation des broches des LEDs
    for (int i = 0; i < LED_COUNT; i++) {
        pinMode(led_pins[i], OUTPUT);
    }

    // Mise à jour des LEDs selon la valeur hexadécimale
    for (int i = 0; i < LED_COUNT; i++) {
        int state = (hex_value >> i) & 1;  // Récupération du bit correspondant
        digitalWrite(led_pins[i], state);
    }
}

int main(int argc, char *argv[]) {
    wiringPiSetup();  // Initialisation de la bibliothèque WiringPi
    unsigned char hex_value = read_switches(argc, argv);
    control_leds(hex_value);
    return 0;
}
