/*********************************************************************
  File:          main.c
  Description:   A program to simulate the philosopher problem using 
                 a distribted memory model approach. This uses MPI
                                                         
  Author:        James Amo      
  Course:        B424/B524 Parallel Programming
  Date:          December 2018
***********************************************************************/

#include <mpi.h>
#include <cstdlib>
#include "philosopher.h"
#include "master.h"
#include "constants.h"

// main entry

int main(int argc, char **argv)
{

  // number of process
  int no_of_process;

  // the process id
  int process_id;

  // initialize MPI
  MPI_Init(&argc, &argv);

  // get the rank for this process
  MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

  // get the total number of process
  MPI_Comm_size(MPI_COMM_WORLD, &no_of_process);

  // check if the process id is for master
  if (process_id == MASTER_ID)
  {
    // orchestrates the whole process and handles input request
    master(process_id, no_of_process);
  }
  else
  {
    // the philsolophers
    philosopher(process_id, no_of_process);
  }

  // finalize MPI
  MPI_Finalize();

  return 0;
}