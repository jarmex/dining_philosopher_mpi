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
  printf("The MASTER process is used for input and output and orchestrating the whole application.\n\n");
  printf("***************************************************************************\n\n");

  // create array to hold the number of philosopher eating count
  phil_count = new int[no_of_process - 1];
  // read the number of iterations from the user
  cout << "Enter the number of iterations: ";
  cin >> iterations;

  MPI_Status status;

  // printf("Starting.......................\n\n");
  // hold waiting philosophers
  list<int> queue;

  int requests, leftchopstick, rightchopstick;
  // philosopher
  int philosopher_num, phil_done = 0, phil_current;
  // chopsticks
  bool chopsticks[no_of_process - 1];
  // set all chopsticks to be free in the initial stage
  for (int i = 0; i < no_of_process - 1; i++)
  {
    chopsticks[i] = true;
    // set the phil_count to 0/
    phil_count[i] = 0;
  }

  // broadcast the number of iterations for each philosopher
  MPI_Bcast(&iterations, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
  // looping for the number of iterations taking into account that each philosopher
  // will send two MPI send request. One for chopstick reques and the other for releasing chopsticks
  for (;;)
  {
    // receive request from any source
    MPI_Recv(&requests, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    // check which philosopher message was recevied
    philosopher_num = status.MPI_SOURCE;
    leftchopstick = philosopher_num % (no_of_process - 1);
    rightchopstick = philosopher_num - 1;
    // printf("REQUEST RCV: Philosopher %d, status %d\n", philosopher_num, status.MPI_TAG);
    // check if the request is for chop sticks
    if (status.MPI_TAG == NEED_CHOPSTICK)
    {
      if (DEBUG)
        printf("MASTER: got philosopher %d chopsticks request\n", philosopher_num);
      // check if both left and right chopstick are free
      if (chopsticks[leftchopstick] == true && chopsticks[rightchopstick] == true)
      {
        // set the chopsticks as taken
        chopsticks[leftchopstick] = false;
        chopsticks[rightchopstick] = false;
        if (DEBUG)
          printf("MASTER: sent philosopher %d the chopsticks\n", philosopher_num);
        // send the chop stick response to the right philosopher
        MPI_Send(&requests, 1, MPI_INT, philosopher_num, RESPONSE_CHOPSTICK, MPI_COMM_WORLD);

        // the philosopher is eating
        phil_current = ++phil_count[philosopher_num - 1];
        if (SHOW_RUNNING_COUNT_DEBUG)
        {
          printf("Philosopher %d has eaten %d times \n", philosopher_num, phil_current);
        }
      }
      else // chopsticks are not available. add it to queue
      {
        if (DEBUG)
          printf("MASTER: philosopher %d is waiting for chopsticks\n", philosopher_num);
        queue.push_back(philosopher_num);
      }
    }
    // check if the request is releasing chopsticks
    if (status.MPI_TAG == RELEASE_CHOPSTICKS)
    {
      if (DEBUG)
        printf("MASTER got philosopher %d chopsticks release\n", philosopher_num);
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
            if (DEBUG)
              printf("MASTER: sent philosopher %d the chopsticks\n", philosopher_num);
            MPI_Send(&requests, 1, MPI_INT, philosopher_num, RESPONSE_CHOPSTICK, MPI_COMM_WORLD);
            // the philosopher is eating. increase the counter
            phil_current = ++phil_count[philosopher_num - 1];
            // print the running count
            if (SHOW_RUNNING_COUNT_DEBUG)
            {
              printf("Philosopher %d has eaten %d times \n", philosopher_num, phil_current);
            }
            // from the philosopher from the queue
            it = queue.erase(it);
          }
        }
      }
    }
    if (status.MPI_TAG == PHILOSOPHER_COMPLETED)
    {
      // the philosopher is done eating and meditating. (this is to break out of the loop)
      if (++phil_done == no_of_process - 1)
      {
        break;
      }
    }
  }
  // output all to screen

  for (int i = 0; i < no_of_process - 1; i++)
  {
    printf("Philosopher %d eats %d times\n", i, phil_count[i]);
  }
}
