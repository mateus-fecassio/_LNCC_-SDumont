#COMPILER = clang-omp
COMPILER = gcc
FLAGS = -fopenmp -Wall -Wextra

all: 1-hello.exec
	@echo "Compiled!"
	mv *.exec $(SCRATCH)

%.exec: %.o
	$(COMPILER) -o $@ $+ $(FLAGS)

%.o: %.c
	$(COMPILER) -o $@ -c $< $(FLAGS)

clean:
	rm -f *.o $(SCRATCH)/*.exec
