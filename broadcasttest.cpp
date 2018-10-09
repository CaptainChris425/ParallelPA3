#include <stdio.h>
#include <mpi.h>
#include <assert.h>
#include <sys/time.h>

int main(int argc, char *argv[]){
	int rank , p;
	int buf[4];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	
	printf("My rank = %d\n", rank);
	printf("Rank = %d num procs = %d\n", rank, p);
	assert(p>=2);	
	if(rank == 0){
		buf[0] = 2;
		buf[1] = 4;
		buf[2] = 6;
		buf[3] = 8;
	}	
	MPI_Bcast(buf,4,MPI_INT,0,MPI_COMM_WORLD);
	printf("%d is my rank and %d is my rand number\n", rank, buf[rank]);
	MPI_Finalize();
}

