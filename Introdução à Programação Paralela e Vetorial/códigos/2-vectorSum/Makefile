#CC = clang-omp
CC = gcc
CFLAGS = -Wall -Wextra -fopenmp

OBJSUM := $(patsubst %.c,%.o,$(wildcard src/*.c))
OBJSUMF := $(patsubst %.c,%.exec,$(wildcard src/*.c))

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: main.o vsum
	@echo "Compiled!"
	mv src/*.exec $(SCRATCH)

%.exec: %.o
	$(CC) $(CFLAGS) -o $@ $< main.o

vsum: $(OBJSUMF)

clean:
	rm -rf main.o $(SCRATCH)/*.exec $(OBJSUM) $(OBJSUMF)
