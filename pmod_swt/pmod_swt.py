import wiringpi
import sys

# Liste des broches en mode BCM
switch_pins = [8, 10, 9, 11, 19, 21, 20, 18]

# Initialisation de WiringPi en mode BCM (Broadcom)
wiringpi.wiringPiSetupGpio()

# Configurer les broches en entrée
for pin in switch_pins:
    wiringpi.pinMode(pin, 0)  # 0 = mode INPUT
    wiringpi.pullUpDnControl(pin, 0)  # 0 = Pas de résistance interne

def read_switches():
    """Lit l'état des interrupteurs et retourne la valeur en hexadécimal."""
    switch_state = sum(1 << i for i, pin in enumerate(switch_pins) if wiringpi.digitalRead(pin))
    return switch_state

if __name__ == "__main__":
    port = sys.argv[1] if len(sys.argv) > 1 else "Inconnu"

    if port == "JA":
        print("Port JA sélectionné !")
    else:
        print(f"Port inconnu : {port}")

    switch_state = read_switches()
    print(f"État des interrupteurs : 0x{switch_state:X}")

    sys.exit()
