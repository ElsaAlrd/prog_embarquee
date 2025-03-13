import gpiod
import time
import RPi.GPIO as GPIO

# Définition des broches GPIO des interrupteurs
GPIO_CHIP = "/dev/gpiochip0"
SWITCH_COUNT = 8
SWITCH_PINS = [8, 10, 9, 11, 19, 21, 20, 18]
LED_PINS = [16, 14, 15, 17, 4, 12, 5, 6]

def lire_interrupteurs():
    """Lit l'état des interrupteurs et retourne une valeur entière correspondant aux LEDs."""
    switch_state = 0
    
    with gpiod.Chip(GPIO_CHIP) as chip:
        lines = chip.get_lines(SWITCH_PINS)
        lines.request(consumer="pmod_swt", type=gpiod.LINE_REQ_DIR_IN)
        values = lines.get_values()
        
        for i, val in enumerate(values):
            if val == 1:
                switch_state |= (1 << i)
    
    return switch_state

def afficher_leds(switches):
    """Affiche l'état des LEDs en binaire."""
    print(f"LEDs: {format(switches, '08b')}")

def main():
    while True:
        switch_state = lire_interrupteurs()
        afficher_leds(switch_state)
        time.sleep(1)  # Rafraîchissement toutes les secondes

if __name__ == "__main__":
    main()
