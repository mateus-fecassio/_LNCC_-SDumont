
def:
	@echo make e1-pi-omp
	@echo make e1-pi-acc
	@echo make e2-laplace2d-omp
	@echo make e2-laplace2d-acc
	@echo make e3-laplace2d-acc
	@echo make e4-laplace2d-omptarget

e1-pi-omp: Makefile e1-pi.c
	pgc++ -o e1-pi-omp -mp e1-pi.c

e1-pi-acc: Makefile e1-pi.c
	pgc++ -Minfo -o e1-pi-acc -acc e1-pi.c

e2-laplace2d-omp: Makefile e2-laplace2d.c
	pgc++ -Minfo -o e2-laplace2d-omp -mp e2-laplace2d.c

e2-laplace2d-acc: Makefile e2-laplace2d.c
	pgc++ -Minfo -o e2-laplace2d-acc -acc e2-laplace2d.c

e3-laplace2d-acc: Makefile e3-laplace2d.c
	pgc++ -Minfo -o e3-laplace2d-acc -acc e3-laplace2d.c

e4-laplace2d-omptarget: Makefile e4-laplace2d.c
	clang -O2 -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda --cuda-path=$(CUDA_PATH) -o e4-laplace2d-omptarget e4-laplace2d.c

clean:
	rm -f e1-pi-omp e1-pi-acc e2-laplace2d-omp e2-laplace2d-acc e3-laplace2d-acc e4-laplace2d-omptarget *.o
