#include <stdio.h>
#include <mpi.h>
#include <assert.h>
#include <sys/time.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
	int rank, p;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	assert(p>=2);
	

	int i;
	for(i=0; i<p;i++){
		MPI_Barrier(MPI_COMM_WORLD);	
		if(i == rank)
			printf("My rank is %d\n",rank);
	}
	MPI_Finalize();
}
