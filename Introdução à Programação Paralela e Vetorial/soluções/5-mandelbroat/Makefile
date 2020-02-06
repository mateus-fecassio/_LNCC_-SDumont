#COMPILER = clang-omp
COMPILER = gcc
FLAGS = -lm -fopenmp -Wall -Wextra

all: 5-mandelbroat.exec 5.1-mandelbroat.exec
	@echo "Compiled!"

%.exec: %.o
	$(COMPILER) -o $@ $+ $(FLAGS)

%.o: %.c
	$(COMPILER) -o $@ -c $< $(FLAGS)

clean:
	rm -f *.o *.exec *.ppm