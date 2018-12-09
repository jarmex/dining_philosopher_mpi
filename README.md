# Solving the Dining Philosophers Problem using MPI

## The Dining Philosopher Problem
The dining philosopher problem states that **K** philsolphers seated around a circular table with one chopstick between each pair of philosophers. There is one chopstick between each philosopher. A philosopher may eat if he can pickup the two chopsticks adjacent to him. One chopstick may be picked up by any one of its adjacent followers but not both.


![alt text](./philosophers.png?raw=true)

Each philosopher must alternatively meditate and eat. Eating is not limited by the amount of food left: asumming and infinite supply of food. 

A philosopher can only eat while holding both the chopstick on the left and the chopstick on the right.  Each philosopher can pick up an adjacent chopstick, when available and put it down when holding it. 

Read more about the dining philosopher problem here [Dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)

This project uses MPI to solve the dining philosopher problem. If you don't have MPI installed follow the instruction from this [Open MPI](https://www.open-mpi.org/faq/?category=building) to have it installed.

## Compiling Application
Copy the project file into a folder of your choice and run the command

`$ make`

to compile the application.

## Running the application
By default the application run with six (6) processes. The process zero (0) indicate the master which orchestrate the whole application and also handles the input and output. You can change the number of processes in the **Makefile**. 

To run the application, type

`$ make run`

Alternatively, you can also run the application by 

`$ mpirun  -np 6 ./phil`

where 6 indicates the number of processes.
