/*********************************************************************
  File:          philosopher.cc
  Description:   A set of functions to handle the philosopher problem
                                                         
  Author:        James Amo      
  Course:        B424/B524 Parallel Programming
  Date:          December 2018
***********************************************************************/

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <mpi.h>
#include "constants.h"

// Waits for a random amount of time between two limits.
void Wait()
{
  static const int LOW = 10000, HIGH = 20000;
  int cycles = LOW + rand() % (HIGH - LOW);
  double j;
  for (int i = 0; i < cycles; i++)
    j = cos(i);
}

// the philosopher is meditating
void Meditating()
{
  Wait(); // wait for a random amount of time
}

// the philosopher is eating
void Eating()
{
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

  MPI_Status status;
  // receive the number of iterations
  MPI_Bcast(&iterations, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
  if (DEBUG)
    printf("Hello Philosopher %d here. Running iteration for %d times\n", rank, iterations);
    
  for (int i = 0; i < iterations; i++)
  {
    if (DEBUG)
    {
      printf("Philosopher %d is meditating \n", rank);
    }
    // philosopher is meditating
    Meditating(); //sleep

    if (DEBUG)
      printf("Philosopher %d is waiting to eat \n", rank);
    // request for chopsticks
    MPI_Send(&leftchopstick, 1, MPI_INT, MASTER_ID, NEED_CHOPSTICK, MPI_COMM_WORLD);
    // wait for response
    MPI_Recv(&outdummy, 1, MPI_INT, MASTER_ID, RESPONSE_CHOPSTICK, MPI_COMM_WORLD, &status);
    // philosopher eating.....
    Eating();
    if (DEBUG)
    {
      printf("Philosopher %d is done eating. Releasing chopsticks \n", rank);
    }
    // release chopsticks
    MPI_Send(&outdummy, 1, MPI_INT, MASTER_ID, RELEASE_CHOPSTICKS, MPI_COMM_WORLD);
  }

  // indicate to the master, this philosopher is done eating and meditating
  MPI_Send(&outdummy, 1, MPI_INT, MASTER_ID, PHILOSOPHER_COMPLETED, MPI_COMM_WORLD);
}
