#include <stdio.h>
#include <mpi.h>
#include <assert.h>
#include <sys/time.h>
#include <stdlib.h>

/*
int main(int argc, char *argv[]){
	int rank, buf;
	int root = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	if(rank == root){
		buf = 777;
	}
	cout << "Before bcast = " << buf << "\n";
	
	MPI_Bcast(&buf, 1, MPI_INT, root, MPI_COMM_WORLD);
	
	cout << "After bcast = " << buf << "\n";
	
	MPI_Finalize();
	return 0;
}
*/

int main(int argc, char *argv[]){
	int rank , p;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	int *buf = malloc(sizeof(int)*p);	
	printf("My rank = %d\n",rank);
	printf("Rank = %d num procs = %d\n",rank,p);
	assert(p>=2);	
	if(rank == 0){
		int i = 0;
		while (i<p){
			buf[i] = (i*i);
			i++;
		}
		
		MPI_Bcast(buf,p,MPI_INT,0,MPI_COMM_WORLD);
	}	
	else{
//	MPI_Barrier(MPI_COMM_WORLD);
		MPI_Bcast(buf,p,MPI_INT,0,MPI_COMM_WORLD);
//	cout << rank << " is my rank and " << buf[rank] << " is my rand number\n";
	printf("%d is my rank and %d is my rand number\n", rank, buf[rank]);
	}
	MPI_Finalize();
}

