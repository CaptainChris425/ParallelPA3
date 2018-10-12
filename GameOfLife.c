#include <stdio.h>
#include <mpi.h>
#include <assert.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>	

int main(int argc, char *argv[]){
	srand(time(0));
	int i;
	int n = 100;
	int g = 10; 
	int rank, p;
	MPI_Init(&argc,&argv);	
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);	
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	assert(p>=2);
	assert(n%p == 0);
	int *randseeds = malloc(sizeof(int)*p);
	if (rank == 0){
		for(i=0;i<p;i++){
			randseeds[i] = rand();	
		}
		MPI_Bcast(randseeds, p , MPI_INT , 0 , MPI_COMM_WORLD);	
	}
	MPI_Bcast(randseeds, p , MPI_INT , 0 , MPI_COMM_WORLD);	
	int *toprow = malloc(sizeof(int)*n);
	int *bottomrow = malloc(sizeof(int)*n);	
	int **board = (int **)malloc(sizeof(int *)*(n/p));
	int **newboard = (int **)malloc(sizeof(int *)*(n/p));

	if (board){
		for(i = 0;i<n/p;i++){
			board[i] = (int *)malloc(sizeof(int)*n);
			newboard[i] = (int *)malloc(sizeof(int)*n);

		}
	}	
	
//make the array all randomly 1 or 0
	srand(randseeds[i]);
	int j;
	int randn;
	for(i=0;i<n;i++){
		for(j=0;j<(n/p);j++){
			board[j][i] = rand()%2;
		}
	}
		
//For all the generations

	int topdest, bottomdest;
	int aliveneighbors;
	MPI_Status status;
	MPI_Request request;
	for(i=0;i<g;i++){
		MPI_Barrier(MPI_COMM_WORLD);
//Split off the top and the bottom rows on the gameboard
		toprow = board[0];
		bottomrow = board[(n/p)-1];
		topdest = rank-1;
		bottomdest = rank+1;
//if bottom > last proccess
	//then send to the first proccess
//if top < first proccess
	//then send to the last process
		if(topdest < 0)
			topdest = p-1;
		if(bottomdest >= p)
			bottomdest = 0;	
		MPI_Isend(toprow, n, MPI_INT, topdest, 0, MPI_COMM_WORLD, &request);
		MPI_Isend(bottomrow, n, MPI_INT, bottomdest, 0, MPI_COMM_WORLD, &request);
		MPI_Recv(toprow, n, MPI_INT, topdest, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		MPI_Recv(bottomrow, n, MPI_INT, bottomdest, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

//At this point the matrix looks like
// 	
//[	toprow	] 		
//[	0	] 
//[	...	]
//[	(n/p)-1	]	
//[	bottomrow]
//(0 - - - - - n-1)

//board[n/p][n]
//toprow[n]
		for(j=0; j<(n/p); j++){
		for(i=0; i<n; i++){
			aliveneighbors = 0;
		//if it is the left most element
			if (i == 0 && j==0){
				if (toprow[n-1] == 1){
					aliveneighbors++;
			}}else if(j==0){
				if (toprow[i-1] == 1){
					aliveneighbors++;
				if (board[j][i-1]
			}}
		//if it is the right most element
			if (i == (n-1) && j==0){
				if (toprow[0] == 1){
					aliveneighbors++;				
			}}else if(j==0){
				if (toprow[i+1] == 1){
					aliveneighbors+++;
			}}
			if(j == 0){
				//right above matrix on the top row
				if (toprow[i] == 1)
					aliveneighbors++;
				if (i == 0){
					if (board[j][n-1] == 1)
						aliveneighbors++;
				}else{
					if (board[j][i-1] == 1)
						aliveneighbors++;
				}
				if (i == (n-1)){
					if (board[j][0] == 1)
						aliveneighbors++;
				}else{
					if (board[j][i+1] == 1)
						aliveneighbors++;
				}

			}
									
		}}	
			

	
			
			
	}


	MPI_Finalize();
}
	
