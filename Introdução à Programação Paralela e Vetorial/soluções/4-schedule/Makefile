#COMPILER = clang-omp
COMPILER = gcc
FLAGS = -fopenmp -Wall -Wextra

all: 4-schedule.exec 4.1-schedule.exec 4.2-schedule.exec 4.3-schedule.exec
	@echo "Compiled!"

%.exec: %.o
	$(COMPILER) -o $@ $+ $(FLAGS)

%.o: %.c
	$(COMPILER) -o $@ -c $< $(FLAGS)

clean:
	rm -f *.o *.exec