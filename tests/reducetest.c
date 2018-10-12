#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <sys/time.h>

int main(int argc, char *argv[]){

	int rank,p;

	struct timeval t1,t2;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	assert(p>=2);
	if(rank == 4){
		int x = 10;
		int dest = 0;
		gettimeofday(&t1,NULL);
		MPI_Send(&x,1,MPI_INT,dest,0,MPI_COMM_WORLD);
		gettimeofday(&t2,NULL);
		int tsend = (t2.tv_sec-t1.tv_sec)*1000 + (t2.tv_usec-t1.tv_usec)/1000;
		printf("send time = %d",tsend);
	}else if (rank == 0){
		int y = 0;
		MPI_Status status;
		MPI_Recv(&y,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
	}
/*	
	int x = 10;
	if (rank == (int)(p/3)+1) x = 100;
	printf("My rank = %d, my value = %d",rank,x);	
	int max;
	MPI_Reduce(&x,&max,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);
	if (rank==0) printf("My rank is %d and the max number is %d\n",rank,max);
*/


	MPI_Finalize();
}
