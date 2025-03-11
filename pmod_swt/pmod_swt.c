#include <stdio.h>
#include <gpiod.h>
#include <string.h>

#define GPIO_CHIP "/dev/gpiochip0" // Le fichier du contrôleur GPIO
#define SWITCH_COUNT 8             // Nombre d'interrupteurs
int switch_pins[8] = {8, 9, 10, 11, 18, 19, 20, 21};

int main(int argc, char *argv[]) {
    struct gpiod_chip *chip;
    struct gpiod_line *lines[SWITCH_COUNT];
    int switch_state = 0;

    if (argc > 1 && strcmp(argv[1], "JA") == 0) {
        printf("Port JA sélectionné !\n");
    }
    else {
        printf("Port inconnu : %s\n", argv[1]);
    }

    // Ouvrir le contrôleur GPIO
    chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        perror("Erreur : impossible d'ouvrir le contrôleur GPIO");
        return 1;
    }

    // Initialisation des lignes GPIO
    for (int i = 0; i < SWITCH_COUNT; i++) {
        lines[i] = gpiod_chip_get_line(chip, switch_pins[i]);
        if (!lines[i] || gpiod_line_request_input(lines[i], "pmod_swt") < 0) {
            perror("Erreur : impossible d'accéder à une broche GPIO");
            gpiod_chip_close(chip);
            return 1;
        }
    }

    // Lire l'état des interrupteurs
    for (int i = 0; i < SWITCH_COUNT; i++) {
        if (gpiod_line_get_value(lines[i]) == 1) {
            switch_state |= (1 << i);
        }
    }

    printf("État des interrupteurs : 0x%X\n", switch_state);

    // Libération des ressources
    gpiod_chip_close(chip);
    return switch_state;
}
