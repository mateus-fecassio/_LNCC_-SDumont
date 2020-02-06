#################################
### Compiler configuration:
# module load intel_psxe/2019

CC=icc
CFLAGS=-no-vec -qopenmp -qopt-report=5 -Wall -Wextra -xHost -lm

all: 9-petroleo.exec
	mv *.exec $(SCRATCH)

9-petroleo.exec: prog_aux_3D.o 9-kernel.o prog_kernel_3D.o prog_MOD3D_testKERNEL.o
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm -f *.bin *.txt *.o *.optrpt $(SCRATCH)/*.exec
