#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int generatorInt(int last) {
	return last+1;
}
int world_rank;
int world_size;
int primeHere;
  
void InitializeMPI(int argc, char** argv){
	primeHere = 0;
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);
	// Find out rank, size
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// We are assuming at least 2 processes for this task
	if (world_size < 2) {
	fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
	MPI_Abort(MPI_COMM_WORLD, 1);
	}
}

int main(int argc, char** argv) {
	InitializeMPI(argc, argv);
  int first = 1;
  int last;
  int keepSignal = 1;
  if (world_rank == 0) {

    last = first;



	while (keepSignal){
		last = generatorInt(last);
		//printf("Process 0 generates number %d\n", last);
		MPI_Send(
	      /* data         = */ &last, 
	      /* count        = */ 1, 
	      /* datatype     = */ MPI_INT, 
	      /* destination  = */ 1, 
	      /* tag          = */ 0, 
	      /* communicator = */ MPI_COMM_WORLD);

  		MPI_Bcast(&keepSignal, 1, MPI_INT, world_rank, MPI_COMM_WORLD);
	}

    

    
  } else {  
  	MPI_Recv(
	      /* data         = */ &last, 
	      /* count        = */ 1, 
	      /* datatype     = */ MPI_INT, 
	      /* source       = */ world_rank-1, 
	      /* tag          = */ 0, 
	      /* communicator = */ MPI_COMM_WORLD, 
	      /* status       = */ MPI_STATUS_IGNORE);
		
  	if (primeHere == 0){
  		primeHere = last;
	    //printf("Process %d has number %d as prime number from process %d\n", world_rank, primeHere, world_rank-1);
	    printf("%d ", primeHere);
	    keepSignal = 1;
	    if (world_rank == world_size-1){
  			//printf("Process has reached the end of the counting");
	  		keepSignal = 0;
	  	}		
	    MPI_Bcast(&keepSignal, 1, MPI_INT, world_rank, MPI_COMM_WORLD);
	    while (keepSignal){
	    	MPI_Recv(&last, 1, MPI_INT, world_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    if (last % primeHere != 0){
	  			MPI_Send(
			      /* data         = */ &last, 
			      /* count        = */ 1, 
			      /* datatype     = */ MPI_INT, 
			      /* destination  = */ world_rank+1, 
			      /* tag          = */ 0, 
			      /* communicator = */ MPI_COMM_WORLD);
	  			//printf("Process %d pass number %d to the process %d\n", world_rank, last, world_rank+1);
	  		}
	  		MPI_Bcast(&keepSignal, 1, MPI_INT, world_rank, MPI_COMM_WORLD);
	    }
	    
  	} 

  	
	
  }
  MPI_Finalize();
}