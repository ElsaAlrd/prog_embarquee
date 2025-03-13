#include <stdio.h>
#include <wiringPi.h>
#include <string.h>

#define SWITCH_COUNT 8 // Nombre d'interrupteurs
int switch_pins[SWITCH_COUNT] = {8, 10, 9, 11, 19, 21, 20, 18};

int main(int argc, char *argv[]) {
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
