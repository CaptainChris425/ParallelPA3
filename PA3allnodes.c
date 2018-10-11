#include <stdio.h>

#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>



int** GenerateInitialGoL(int random_seed, int n, int p){
    srand(random_seed);
    int** rows = (int**)malloc(sizeof(int*)*(n/p));
    int i, j;
    for(i = 0; i < n/p; i++){
        rows[i] = (int*)malloc(sizeof(int)*n);
        for(j = 0; j< n; j++){
            rows[i][j] = rand() % 2;
            
        }
        
    }
    return rows;
   //printf("\n-----------------------------------------------------------------------------\n");
   /*  for(i = 0; i < n/p; i++){
        rows[i] = (int*)malloc(sizeof(int)*n);
        for(j = 0; j< n; j++){
            rows[i][j] = rand() % 2;
            printf("%d ", rows[i][j]);
        }
        printf("\n");
    } */

}

void Simulate(int ** matrix, int n, int Generations, int rank, int p){
    //status[0]: top row recv status
    //status[1]: bottom row recv status
    MPI_Status status[2];  
        
    int i,j,k,l, rankToSendLastRow, rankToSendFirstRow, rankRecTopRowFrom, rankRecBottomRowFrom;
	int printrank, pi, pj;
    int neighborsAlive = 0;
    int* topRow = (int*)malloc(sizeof(int)*n);
    int* bottomRow = (int*)malloc(sizeof(int)*n);  
    
    //Creating new matrix to store new generation and not work with freshly edited values
    int** newMatrix = (int**)malloc(sizeof(int*)*(n/p));
    int** temp;
    
    for(l = 0; l < n/p; l++){
        newMatrix[l] = (int*)malloc(sizeof(int)*n);
    }

    for(i = 0; i < Generations; i++){

    	MPI_Barrier(MPI_COMM_WORLD);
        /*************************** SENDING BOTTOM ROW ******************************/
        //sending last row
        if(rank == p){ //If this is the last process then its last row will be sent to process 1
            rankToSendLastRow = 0;
        } else{ //If this is not the last process, then its last row will be sent to process 1 rank higher
            rankToSendLastRow = rank + 1;
        }
        //Send last row to respective process
        MPI_Send(matrix[(n/p) - 1], n, MPI_INT, rankToSendLastRow, 0, MPI_COMM_WORLD);
        
        

        /*************************** RECEIVING TOP ROW ******************************/
         
        if(rank == 0){ //If this is first process, then top row will be obtained from last process
            rankRecTopRowFrom = p;
        }else{ //If this process is anything but the first, then top row will be obtained from process 1 rank lower
            rankRecTopRowFrom = rank - 1;
        }
        //Recieve top row from respective process
        MPI_Recv(topRow, n, MPI_INT, rankRecTopRowFrom, MPI_ANY_TAG, MPI_COMM_WORLD, &status[0]);
        
        
        //MPI_Barrier(MPI_COMM_WORLD);
        

        /*************************** SENDING TOP ROW ******************************/

        if(rank == 0){ //If this is the first process, then the top row will be sent to the last process
            rankToSendFirstRow = p;
        } else{ //If this is not the first process, then the top row will be sent to the process 1 rank lower
            rankToSendFirstRow = rank - 1;
        }

        MPI_Send(matrix[0], n, MPI_INT, rankToSendFirstRow, 0, MPI_COMM_WORLD);

        /*************************** RECEIVING BOTTOM ROW ******************************/

        if(rank == p){ //If this process is the last process, then the bottom row will be obtained from the first process
            rankRecBottomRowFrom = 0;
        } else { //If this is not the last process, then the bottom row will be obtained from the process 1 rank higher
            rankRecBottomRowFrom = rank + 1;
        }
        
        //Recieve bottom row from respective process
        MPI_Recv(bottomRow, n, MPI_INT, rankRecBottomRowFrom, MPI_ANY_TAG, MPI_COMM_WORLD, &status[1]);
        
        for(j = 0; j < (n/p); j++){
            for(k = 0; k < n; k++){
                neighborsAlive = 0;
                if(j == 0){ //If element in first row
                    if(k == 0){ //If element in first row and first column
                        /*---------------- Cheking the 3 neighbors above ------------------*/
                        if(topRow[n-1] == 1) neighborsAlive++;
                        if(topRow[k] == 1) neighborsAlive++;
                        if(topRow[k+1] == 1) neighborsAlive++;
                        /*---------------- Checking the side neighbors -------------------*/
                        if(matrix[j][n-1] == 1) neighborsAlive++;
                        if(matrix[j][k+1] == 1) neighborsAlive++;
                        /*---------------- Checking the 3 neighbors below -------------------*/
                        if(matrix[j+1][n-1] == 1) neighborsAlive++;
                        if(matrix[j+1][k] == 1) neighborsAlive++;
                        if(matrix[j+1][k+1] == 1) neighborsAlive++;
                    } else if(k == n-1){ //If element in first row and last column
                        /*---------------- Cheking the 3 neighbors above ------------------*/
                        if(topRow[k-1] == 1) neighborsAlive++;
                        if(topRow[k] == 1) neighborsAlive++;
                        if(topRow[1] == 1) neighborsAlive++;
                        /*---------------- Checking the side neighbors -------------------*/
                        if(matrix[j][k-1] == 1) neighborsAlive++;
                        if(matrix[j][1] == 1) neighborsAlive++;
                        /*---------------- Checking the 3 neighbors below -------------------*/
                        if(matrix[j+1][k-1] == 1) neighborsAlive++;
                        if(matrix[j+1][k] == 1) neighborsAlive++;
                        if(matrix[j+1][1] == 1) neighborsAlive++;
                    } else{ //If element in the middle of first row
                        /*---------------- Cheking the 3 neighbors above ------------------*/
                        if(topRow[k-1] == 1) neighborsAlive++;
                        if(topRow[k] == 1) neighborsAlive++;
                        if(topRow[k+1] == 1) neighborsAlive++;
                        /*---------------- Checking the side neighbors -------------------*/
                        if(matrix[j][k-1] == 1) neighborsAlive++;
                        if(matrix[j][k+1] == 1) neighborsAlive++;
                        /*---------------- Checking the 3 neighbors below -------------------*/
                        if(matrix[j+1][k-1] == 1) neighborsAlive++;
                        if(matrix[j+1][k] == 1) neighborsAlive++;
                        if(matrix[j+1][k+1] == 1) neighborsAlive++;
                    }
                    
                } else if (j == (n/p) - 1) //If element in last row
                {
                    if(k == 0){ //If element in last row and first column
                        /*---------------- Cheking the 3 neighbors above ------------------*/
                        if(matrix[j-1][n-1] == 1) neighborsAlive++;
                        if(matrix[j-1][k] == 1) neighborsAlive++;
                        if(matrix[j-1][k+1] == 1) neighborsAlive++;
                        /*---------------- Checking the side neighbors -------------------*/
                        if(matrix[j][n-1] == 1) neighborsAlive++;
                        if(matrix[j][k+1] == 1) neighborsAlive++;
                        /*---------------- Checking the 3 neighbors below -------------------*/
                        if(bottomRow[n-1] == 1) neighborsAlive++;
                        if(bottomRow[k] == 1) neighborsAlive++;
                        if(bottomRow[k+1] == 1) neighborsAlive++;                        
                    } else if(k == n-1){ //If element in last row and last column
                        /*---------------- Cheking the 3 neighbors above ------------------*/
                        if(matrix[j-1][k-1] == 1) neighborsAlive++;
                        if(matrix[j-1][k] == 1) neighborsAlive++;
                        if(matrix[j-1][1] == 1) neighborsAlive++;
                        /*---------------- Checking the side neighbors -------------------*/
                        if(matrix[j][k-1] == 1) neighborsAlive++;
                        if(matrix[j][1] == 1) neighborsAlive++;
                        /*---------------- Checking the 3 neighbors below -------------------*/
                        if(bottomRow[k-1] == 1) neighborsAlive++;
                        if(bottomRow[k] == 1) neighborsAlive++;
                        if(bottomRow[1] == 1) neighborsAlive++;  
                    } else{ //If element in the middle of last row
                        /*---------------- Checking the 3 neighbors above ------------------*/
                        if(matrix[j-1][k-1] == 1) neighborsAlive++;
                        if(matrix[j-1][k] == 1) neighborsAlive++;
                        if(matrix[j-1][k+1] == 1) neighborsAlive++;
                        /*---------------- Checking the side neighbors -------------------*/
                        if(matrix[j][k-1] == 1) neighborsAlive++;
                        if(matrix[j][k+1] == 1) neighborsAlive++;
                        /*---------------- Checking the 3 neighbors below -----------------*/
                        if(bottomRow[k-1] == 1) neighborsAlive++;
                        if(bottomRow[k] == 1) neighborsAlive++;
                        if(bottomRow[k+1] == 1) neighborsAlive++;
                        
                    }                    
                } else{ //If element if in a middle row
                    if(k == 0){ //If element is in a middle row and in the first column
                        /*---------------- Checking the 3 neighbors above ------------------*/
                        if(matrix[j-1][n-1] == 1) neighborsAlive++;
                        if(matrix[j-1][k] == 1) neighborsAlive++;
                        if(matrix[j-1][k+1] == 1) neighborsAlive++;
                        /*---------------- Checking the side neighbors -------------------*/
                        if(matrix[j][n-1] == 1) neighborsAlive++;
                        if(matrix[j][k+1] == 1) neighborsAlive++;
                        /*---------------- Checking the 3 neighbors below -------------------*/
                        if(matrix[j+1][n-1] == 1) neighborsAlive++;
                        if(matrix[j+1][k] == 1) neighborsAlive++;
                        if(matrix[j+1][k+1] == 1) neighborsAlive++;
                    } else if(k == n - 1){ //If element is in a middle row and in the last column
                        /*---------------- Checking the 3 neighbors above ------------------*/
                        if(matrix[j-1][k-1] == 1) neighborsAlive++;
                        if(matrix[j-1][k] == 1) neighborsAlive++;
                        if(matrix[j-1][1] == 1) neighborsAlive++;
                        /*---------------- Checking the side neighbors -------------------*/
                        if(matrix[j][k-1] == 1) neighborsAlive++;
                        if(matrix[j][1] == 1) neighborsAlive++;
                        /*---------------- Checking the 3 neighbors below -------------------*/
                        if(matrix[j+1][k-1] == 1) neighborsAlive++;
                        if(matrix[j+1][k] == 1) neighborsAlive++;
                        if(matrix[j+1][1] == 1) neighborsAlive++;
                    } else{ //If element is in a middle row and middle column
                        /*---------------- Checking the 3 neighbors above ------------------*/
                        if(matrix[j-1][k-1] == 1) neighborsAlive++;
                        if(matrix[j-1][k] == 1) neighborsAlive++;
                        if(matrix[j-1][k+1] == 1) neighborsAlive++;
                        /*---------------- Checking the side neighbors -------------------*/
                        if(matrix[j][k-1] == 1) neighborsAlive++;
                        if(matrix[j][k+1] == 1) neighborsAlive++;
                        /*---------------- Checking the 3 neighbors below -------------------*/
                        if(matrix[j+1][k-1] == 1) neighborsAlive++;
                        if(matrix[j+1][k] == 1) neighborsAlive++;
                        if(matrix[j+1][k+1] == 1) neighborsAlive++;
                    }
                }
                if(neighborsAlive < 3 || neighborsAlive > 5){
                    newMatrix[j][k] = 0;
                } else {
                    newMatrix[j][k] = 1;
                }
            }
        }
        
        //Set matrix to newly obtained matrix, and set newMatrix to old matrix to populate next generation as a new matrix
        temp = matrix;
        matrix = newMatrix; 
        newMatrix = temp;
	//Print the generations
	if(rank==0) printf("\n Generation %d\n", i);
	//Print the full matrix in order by rank
		//by barriering of a for loop that prints only the rank that matches the iteration
        if(i%10 == 0){
		for(printrank = 0; printrank<=p; printrank++){ //for each rank 0-p
			MPI_Barrier(MPI_COMM_WORLD); //Barrier off each proccess so they all stay on the same for loop itteration
			if (rank == printrank){ //If it is the correct itteration for the rank
				for(pj = 0; pj< n; pj++){ printf("--%d--",rank);} //print a line to separate procceses
				printf("\n");
				for(pi = 0; pi < n/p; pi++){	//For each n/p row
   			     		for(pj = 0; pj< n; pj++){ //For each n value in the row
   		    	     			printf("| %d |",matrix[pi][pj]); //print its value
       			 		}
					printf("\n");
    				}
				for(pj = 0; pj< n; pj++){ printf("-----");} //Another separation line
				printf("\n");
			}
			
		}
	}

        
		/*
            printf("\n-------------------------------------------------------");
            for(i = 0; i < n/p; i++){
                for(j = 0; j < n; j++){
                    printf("%d ", matrix[i][j]);
                }
                printf("\n");
            } 
            printf("-------------------------------------------------------\n");
        }
		*/
    }
    
}

/*  printf("Process %d topRow:", rank);
        for(j = 0; j < n; j++){
            printf("%d ", topRow[j]);
        }
        printf("\n");
        printf("Process %d bottomRow:", rank);
        for(j = 0; j < n; j++){
            printf("%d ", bottomRow[j]);
        }
        printf("\n"); */
int main(int argc, char *argv[]){
    int rank, p, i;
    struct timeval t1, t2;
    int Generations = 300, n = 32;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    assert(p>=2);
    if(rank == 0){
        int* seeds_send = (int*)malloc(sizeof(int)*p);
        
        srand(time(NULL));


        for (i = 0; i < p; i++){
            seeds_send[i] = rand()%1000;
        }

        MPI_Bcast(seeds_send, p, MPI_INT, 0, MPI_COMM_WORLD);
        //MPI_Barrier(MPI_COMM_WORLD);
        //for(i = 0; i < Generations; i++){
            //MPI_Barrier(MPI_COMM_WORLD);
        //}
    }
        int cur_process_seed;
        int* seeds_rec = (int*)malloc(sizeof(int)*p-1);
        int** matrix;
         //Save the array of seeds broadcasted by process zero
        MPI_Bcast(seeds_rec, p, MPI_INT, 0, MPI_COMM_WORLD);
        //Obtain the process specific seed
        cur_process_seed = seeds_rec[rank];
        matrix = GenerateInitialGoL(cur_process_seed, n, p);
        
        Simulate(matrix, n, Generations, rank, p-1);
       
     
    MPI_Finalize();

    

}




