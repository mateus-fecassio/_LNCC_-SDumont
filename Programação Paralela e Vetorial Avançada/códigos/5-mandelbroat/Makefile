#COMPILER = clang-omp
COMPILER = gcc
FLAGS = -lm -fopenmp -Wall -Wextra

all: 5-mandelbroat.exec
	@echo "Compiled!"
	mv *.exec $(SCRATCH)

%.exec: %.o
	$(COMPILER) -o $@ $+ $(FLAGS)

%.o: %.c
	$(COMPILER) -o $@ -c $< $(FLAGS)

clean:
	rm -f *.o $(SCRATCH)/*.exec *.ppm
