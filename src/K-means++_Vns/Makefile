MAIN = kmeans++
CC = gcc
CFLAGS = -Wall -std=c99
LIBS = -lm

OBJECTS := $(wildcard *.c)

all: $(MAIN)

$(MAIN): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

.PHONY: clean

clean:
	rm -f *.o $(MAIN) 

