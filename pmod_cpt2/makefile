CC = gcc
CFLAGS = -Wall
LDFLAGS = -lwiringPi
TARGET = pmod_cpt2

all: $(TARGET)

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o $(LDFLAGS)
	rm -f $(TARGET).o

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c

clean:
	rm -f $(TARGET) $(TARGET).o
