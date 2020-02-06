#################################
### Compiler configuration:
# module load intel_psxe/2019

CC=icc
CFLAGS=-no-vec -qopenmp -qopt-report=5 -Wall -Wextra -xHost -lm

all: 3DWave 3DWave_par

3DWave: prog_aux_3D.o 9-kernel.o prog_kernel_3D.o prog_MOD3D_testKERNEL.o
	$(CC) $^ -o $@ $(CFLAGS)

3DWave_par: prog_aux_3D.o 9.1-kernel.o prog_kernel_3D.o prog_MOD3D_testKERNEL.o
	$(CC) $^ -o $@ $(CFLAGS)

run:
	./3DWave TIPO_EQUACAO=0 N1_GLOBAL=1024 N2_GLOBAL=1024 N3_GLOBAL=1024 D1=10 D2=10 D3=10 DT=0.0005 NTSTEP=10 VP_DEF=3000 RHO_DEF=1.0 VERBOSE=5 FC=45 INT_SNAPSHOT=1000 SRC1=64 SRC2=64 SRC3=64

run_par:
	./3DWave_par TIPO_EQUACAO=0 N1_GLOBAL=1024 N2_GLOBAL=1024 N3_GLOBAL=1024 D1=10 D2=10 D3=10 DT=0.0005 NTSTEP=10 VP_DEF=3000 RHO_DEF=1.0 VERBOSE=5 FC=45 INT_SNAPSHOT=1000 SRC1=64 SRC2=64 SRC3=64

clean:
	rm -f *.bin *.txt *.o *.optrpt 3DWave 3DWave_par