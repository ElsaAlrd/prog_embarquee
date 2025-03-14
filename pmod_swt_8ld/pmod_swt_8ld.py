import wiringpi
import sys

# Définition des pins des interrupteurs et des LEDs
switch_pins = [8, 10, 9, 11, 19, 21, 20, 18]
led_pins = [16, 14, 15, 17, 4, 12, 5, 6]

def read_switches():
    """Lit l'état des interrupteurs et retourne la valeur en hexadécimal."""
    switch_state = 0
    
    for i, pin in enumerate(switch_pins):
        if wiringpi.digitalRead(pin) == 1:  # HIGH
            switch_state |= (1 << i)
    
    print(f"État des interrupteurs : 0x{switch_state:X}")
    return switch_state

def control_leds(hex_value):
    """Met à jour l'état des LEDs en fonction de la valeur hexadécimale fournie."""
    for i, pin in enumerate(led_pins):
        state = (hex_value >> i) & 1
        wiringpi.digitalWrite(pin, state)

def main():
    port = sys.argv[1] if len(sys.argv) > 1 else "Inconnu"
    if port == "JA":
        print("Port JA sélectionné !")
    else:
        print(f"Port inconnu : {port}")

    if wiringpi.wiringPiSetupGpio() == -1:
        print("Erreur : impossible d'initialiser WiringPi")
        sys.exit(1)

    for pin in switch_pins:
        wiringpi.pinMode(pin, 0)
        wiringpi.pullUpDnControl(pin, 0)

    for pin in led_pins:
        wiringpi.pinMode(pin, 1)

    switch_state = read_switches()
    control_leds(switch_state)

if __name__ == "__main__":
    main()
