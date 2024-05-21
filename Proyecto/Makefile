CC=gcc
CFLAGS=-Wall -pthread
TARGETS=sensor monitor

all: $(TARGETS)

sensor: sensor.c
	$(CC) $(CFLAGS) -o sensor sensor.c

monitor: monitor.c
	$(CC) $(CFLAGS) -o monitor monitor.c

clean:
	rm -f $(TARGETS)
	rm -f *.o
