
def:
	@echo make s1-pi-omp
	@echo make s1-pi-acc
	@echo make s2-laplace2d-omp
	@echo make s2-laplace2d-acc
	@echo make s3-laplace2d-acc
	@echo make s4-laplace2d-omptarget


s1-pi-omp: Makefile s1-pi.c
	pgc++ -o s1-pi-omp -mp s1-pi.c

s1-pi-acc: Makefile s1-pi.c
	pgc++ -Minfo -o s1-pi-acc -acc s1-pi.c

s2-laplace2d-omp: Makefile s2-laplace2d.c
	pgc++ -Minfo -o s2-laplace2d-omp -mp s2-laplace2d.c

s2-laplace2d-acc: Makefile s2-laplace2d.c
	pgc++ -Minfo -o s2-laplace2d-acc -acc s2-laplace2d.c

s3-laplace2d-acc: Makefile s3-laplace2d.c
	pgc++ -Minfo -o s3-laplace2d-acc -acc s3-laplace2d.c

s4-laplace2d-omptarget: Makefile s4-laplace2d.c
	clang -O2 -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda --cuda-path=$(CUDA_PATH) -o s4-laplace2d-omptarget s4-laplace2d.c

clean:
	rm -f s1-pi-omp s1-pi-acc s2-laplace2d-omp s2-laplace2d-acc s3-laplace2d-acc s4-laplace2d-omptarget *.o
