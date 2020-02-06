CC = icc
CFLAGS = -qopt-report=5 -no-vec -qopenmp -xHost -Wall -Wextra

# module load intel_psxe/2019

OBJSUM := $(patsubst %.c,%.o,$(wildcard src/*.c))
OBJSUMF := $(patsubst %.c,%.exec,$(wildcard src/*.c))

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: main.o matrix
	@echo "Compiled!"
	mv src/*.exec $(SCRATCH)

%.exec: %.o
	$(CC) $(CFLAGS) -o $@ $< main.o

matrix: $(OBJSUMF)

clean:
	rm -rf main.o $(SCRATCH)/*.exec *.optrpt src/*.optrpt $(OBJSUM) $(OBJSUMF)

