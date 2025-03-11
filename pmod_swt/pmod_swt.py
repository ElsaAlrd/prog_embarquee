from gpiozero import Button # type: ignore
import sys




def read_switches(port_pins):
    value = 0
    for i, pin in enumerate(port_pins):
        switch = Button(pin, pull_up=False)
        if switch.is_pressed:
            value |= (1 << i)
    return value

def main():
    if len(sys.argv) != 2:
        print("Usage: python pmod_swt.py <port>")
        sys.exit(1)
    
    port = sys.argv[1]
    port_pins = {
        "JA": [8,9,10,11,18,19,20,21]  # Exemple de correspondance
    }
    
    if port not in port_pins:
        print("Port inconnu !")
        sys.exit(1)
    
    switch_value = read_switches(port_pins[port])
    print(f"Valeur des interrupteurs: 0x{switch_value:02X}")

if __name__ == "__main__":
    main()
