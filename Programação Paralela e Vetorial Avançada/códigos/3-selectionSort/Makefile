#CC = clang-omp
CC = gcc
CFLAGS = -fopenmp

OBJSUM := $(patsubst %.c,%.o,$(wildcard src/*.c))
OBJSUMF := $(patsubst %.c,%.exec,$(wildcard src/*.c))

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: main.o selection-sort
	@echo "Compiled!"
	mv src/*.exec $(SCRATCH)

%.exec: %.o
	$(CC) $(CFLAGS) -o $@ $< main.o

selection-sort: $(OBJSUMF)

clean:
	rm -rf main.o $(SCRATCH)/*.exec $(OBJSUM) $(OBJSUMF)
