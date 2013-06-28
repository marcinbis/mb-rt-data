CC ?= gcc
CFLAGS = -DDEBUG -DMODEL -Wall -Os -s
LDFLAGS = -lrt

sources = $(wildcard *.c)
objects = $(sources:.c=.o)

laser1: $(objects)
	$(CC) $(CFLAGS) -c controller.c
	$(CC) $(CFLAGS) -c fsm.c
	$(CC) $(CFLAGS) -c io.c
	$(CC) $(CFLAGS) -c main.c
	$(CC) $(CFLAGS) -c msg.c
	$(CC) $(CFLAGS) -c pwm.c
	$(CC) -o $@ $(objects) $(LDFLAGS) 

.PHONY: clean

clean:
	rm -rf $(objects) laser1
