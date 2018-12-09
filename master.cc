/*********************************************************************
  File:          master.c
  Description:   A set of functions to handle input and output  
                 functionalities
                                                         
  Author:        James Amo      
  Course:        B424/B524 Parallel Programming
  Date:          December 2018
***********************************************************************/

#include <iostream>
#include <cstdlib>
#include <list>
#include <mpi.h>
#include "master.h"
#include "constants.h"
using namespace std;

// hold the number of philosophers
int *phil_count;
// the number of iterations
int iterations;

void master(int process_id, int no_of_process)
{
  printf("******************************************************************************\n\n");
  printf("      Welcome to DINING PHILOSOPHER PROBLEM using MPI   \n\n");
  printf("This applications requires that the number of processes should be even number.\n");
  printf("The first process is used for input and output and orchestrating the whole application.\n");
  printf("***************************************************************************\n");

  if (no_of_process % 2 != 0)
  {
    printf("\nApplication exiting. Check the usage.......\n");
    exit(0);
  }
  // create array to hold the number of philosopher eating count
  phil_count = new int[no_of_process - 1];
  // read the number of iterations from the user
  cout << endl
       << "Enter the number of iterations: ";
  cin >> iterations;

  MPI_Status status;

  // hold waiting philosophers
  list<int> queue;

  int requests, leftchopstick, rightchopstick;
  // philosopher
  int philosopher_num;
  // chopsticks
  bool chopsticks[no_of_process - 1];
  // set all chopsticks to be free in the initial stage
  for (int i = 0; i < no_of_process - 1; i++)
  {
    chopsticks[i] = true;
  }

  // broadcast the number of iterations for each philosopher
  MPI_Bcast(&iterations, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);

  // looping for the number of iterations
  for (int i = 0; i < iterations; i++)
  {
    // receive request from any source
    MPI_Recv(&requests, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    // check which philosopher message was recevied
    philosopher_num = status.MPI_SOURCE;
    leftchopstick = philosopher_num % (no_of_process - 1);
    rightchopstick = philosopher_num - 1;
    // check if the request is for chop sticks
    if (status.MPI_TAG == NEED_CHOPSTICK)
    {
      // check if both left and right chopstick are free
      if (chopsticks[leftchopstick] == true && chopsticks[rightchopstick] == true)
      {
        // set the chopsticks as taken
        chopsticks[leftchopstick] = false;
        chopsticks[rightchopstick] = false;

        // send the chop stick response to the right philosopher
        MPI_Send(&requests, 1, MPI_INT, philosopher_num, RESPONSE_CHOPSTICK, MPI_COMM_WORLD);
        // the philosopher is eating
        phil_count[philosopher_num - 1]++;
      }
      else // chopsticks are not available. add it to queue
      {
        queue.push_back(philosopher_num);
      }
    }
    // check if the request is releasing chopsticks
    if (status.MPI_TAG == RELEASE_CHOPSTICKS)
    {
      // release the chopsticks taken.
      // philosopher has release the chopsticks
      chopsticks[leftchopstick] = true;
      chopsticks[rightchopstick] = true;
      // check if there are any pending philosopher in the queue
      if (!queue.empty())
      {
        // iterate through the list
        for (list<int>::iterator it = queue.begin(); it != queue.end(); it++)
        {
          philosopher_num = *it;
          leftchopstick = philosopher_num % (no_of_process - 1);
          rightchopstick = philosopher_num - 1;
          // check if the chopsticks are free to be taken
          if (chopsticks[leftchopstick] == true && chopsticks[rightchopstick] == true)
          {
            // set the chopsticks as taken
            chopsticks[leftchopstick] = false;
            chopsticks[rightchopstick] = false;
            // send the chop stick response to the right philosopher
            MPI_Send(&requests, 1, MPI_INT, philosopher_num, RESPONSE_CHOPSTICK, MPI_COMM_WORLD);
            // the philosopher is eating. increase the counter
            phil_count[philosopher_num - 1]++;
            // from the philosopher from the queue
            it = queue.erase(it);
          }
        }
      }
    }
  }
  // output all to screen
}
