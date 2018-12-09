/*********************************************************************
  File:          philosopher.h
  Description:   A set of functions to handle the philosopher problem
                                                         
  Author:        James Amo      
  Course:        B424/B524 Parallel Programming
  Date:          December 2018
***********************************************************************/

#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

// Waits for a random amount of time between two limits.
void Wait();

// the philosopher is meditating
void Meditating();

// the philosopher is eating
void Eating();

void philosopher(int rank, int no_of_process);

#endif