#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int rank,p;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	assert(p>=2);
	int n = 32;	
	if(rank == 0){
		int x = 0;
	}else{
		int ** matrix = (int**)malloc(sizeof(int*)*(n/p));
		int i,j;
		for (i = 0; i< n/p; i++){
			matrix[i] = (int*)malloc(sizeof(int)*n);
			for(j==0;j<n;j++){
				matrix[i][j] = rank;
			}
		}
		
		int *** matricies = (int ***)malloc(sizeof(int**)*(p));
		//MPI_Gather(matrix, n/p, MPI_INT, matricies, p-1, MPI_INT, 1, MPI_COMM_WORLD);
		if(rank == 1){
			int proc,pi,pj;
			for(proc = 0;proc<p-1;proc++){
				for(pi =0; pi<n/p;pi++){		
					for(pj=0;pj<n;pj++){
						printf("| %d |", matrix[pi][pj]);
					}
					printf("\n");
				}	
			}			
		}
	}
	MPI_Finalize();


}
