#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

int main(int argc, char *argv[]){
	int i;
	for(i=0;i<10;i++)
		printf("%d\n",i);
	int rank, p;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	assert(p>=2);
	int *toprow = malloc(sizeof(int)*p);
	int *bottomrow = malloc(sizeof(int)*p);
	
	if(rank == 0){
		int i = 0;
		while (i<p){
			toprow[i] = i;
			bottomrow[i] = p-i;
			i++;
		}
		MPI_Status status;
		MPI_Request request;
		MPI_Isend(toprow, p, MPI_INT, p-1, 0, MPI_COMM_WORLD, &request);
		MPI_Isend(bottomrow, p, MPI_INT, rank+1, 0, MPI_COMM_WORLD, &request);
	}else if (rank == p-1){
		MPI_Status status;
		int recB = rank + 1;
		if (recB >= p){
			recB = 0;
		}
		MPI_Recv(toprow, p, MPI_INT, recB, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		int i = 0;
		while(i<p){
			printf("my rank is %d and my %d number is %d\n",rank,i,toprow[i]);	
			i++;
		}	
	}else if (rank == 1){
		MPI_Status status;
		int recT = rank - 1;
		if (recT < 0){
			recT = p-1;
		}
		MPI_Recv(bottomrow, p, MPI_INT, recT, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		int i = 0;
		while(i<p){
			printf("my rank is %d and my %d number is %d\n",rank,i,bottomrow[i]);	
			i++;
		}	
	}
	MPI_Finalize();

}
