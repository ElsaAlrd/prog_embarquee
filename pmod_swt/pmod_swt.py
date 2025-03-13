from gpiozero import Button
import sys

switch_pins = [8, 10, 9, 11, 19, 21, 20, 18]

switches = [Button(pin, pull_up=False) for pin in switch_pins]

def read_switches():
    """Lit l'état des interrupteurs et retourne la valeur en hexadécimal."""
    switch_state = sum(1 << i for i, switch in enumerate(switches) if switch.is_pressed)
    return switch_state

if __name__ == "__main__":
    port = sys.argv[1] if len(sys.argv) > 1 else "Inconnu"
    
    if port == "JA":
        print("Port JA sélectionné !")
    else:
        print(f"Port inconnu : {port}")

    switch_state = read_switches()
    print(f"État des interrupteurs : 0x{switch_state:X}")

    sys.exit(switch_state)
