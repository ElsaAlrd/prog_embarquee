import spidev

SPI_CHANNEL = 1
SPI_SPEED = 500000

spi = spidev.SpiDev()
spi.open(0, SPI_CHANNEL)
spi.max_speed_hz = SPI_SPEED

def read_light_sensor():
    """Lit la valeur de luminosité du PMOD ALS via SPI."""
    buffer = [0x00, 0x00]
    response = spi.xfer2(buffer)  # Envoie et reçoit les données SPI
    light_value = (response[0] << 8) | response[1]
    return light_value >> 4

light_value = read_light_sensor()
print(f"Luminosité : {light_value}")

spi.close()
