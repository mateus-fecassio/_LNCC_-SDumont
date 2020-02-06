#include <mpi.h> 
#include <math.h> 
int main ( int argc, char * argv[ ] )
{ int n, myid, numprocs, i, rc; 
double mypi, pi, h, x,time,ini,end, sum = 0.0; 
 n=atoi(argv[1]);
MPI_Init(&argc, &argv); 
MPI_Comm_size(MPI_COMM_WORLD, &numprocs); 
MPI_Comm_rank(MPI_COMM_WORLD, &myid);

ini=MPI_Wtime();
//Cáulo de Pi
if(n != 0) { 
h=1.0/(double) n; 
for (i=myid +1; i <= n; i+=numprocs) { 

x = h * ((double) i - 0.5); 
sum += (4.0/(1.0 + x*x)); 

//Fim di Cáulo de PI

mypi = h* sum; 

MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
end=MPI_Wtime();
time=end-ini;
} 
if (myid==0) 
{
printf ("valor aproximado de pi: %.16f time=%f,myid=%d \n", pi,time,myid); 
}
MPI_Finalize( ); 
} 
}
