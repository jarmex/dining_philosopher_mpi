/*********************************************************************
  File:          philosopher.cc
  Description:   A set of functions to handle the philosopher problem
                                                         
  Author:        James Amo      
  Course:        B424/B524 Parallel Programming
  Date:          December 2018
***********************************************************************/

#include <cstdlib>
#include <cmath>
#include <mpi.h>
#include "constants.h"


// Waits for a random amount of time between two limits.
void Wait()
{
    static const int LOW = 10000, HIGH=20000;
    int cycles = LOW + rand()%(HIGH - LOW);
    double j;
    for (int i=0; i<cycles; i++)
        j = cos(i);
}


// the philosopher is meditating
void Meditating(){
  Wait();// wait for a random amount of time
}

// the philosopher is eating
void Eating(){
  Wait(); // wait for random amount of time
}

void philosopher(int rank, int no_of_process)
{
  // left chop stick
  int leftchopstick;

  // right chop stick
  int rightchopstick;

  // number of iterations
  int iterations;

  int outdummy;

  // receive the number of iterations
  MPI_Bcast(&iterations, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
  //MPI_Recv(&iterations, 1, MPI_INT, MASTER_ID, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  for (int i = 0; i < iterations; i++)
  {
    // philosopher is meditating
    Meditating(); //sleep
    // request for chopsticks
    MPI_Send(&leftchopstick, 1, MPI_INT, MASTER_ID, NEED_CHOPSTICK, MPI_COMM_WORLD);
    // wait for response
    MPI_Recv(&outdummy, 1, MPI_INT, MASTER_ID, NEED_CHOPSTICK, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // philosopher eating.....
    Eating();
    // release chopsticks
    MPI_Send(&outdummy, 1, MPI_INT, 0, RELEASE_CHOPSTICKS, MPI_COMM_WORLD);
  }
}
