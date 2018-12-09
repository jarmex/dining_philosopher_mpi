# Solving the Dining Philosophers Problem using MPI

## The Dining Philosopher Problem
The dining philosopher problem states that **K** philsolphers seated around a circular table with one chopstick between each pair of philosophers. There is one chopstick between each philosopher. A philosopher may eat if he can pickup the two chopsticks adjacent to him. One chopstick may be picked up by any one of its adjacent followers but not both.


![alt text](./philosophers.png?raw=true)

Each philosopher must alternatively meditate and eat. Eating is not limited by the amount of food left: asumming and infinite supply of food. 

A philosopher can only eat while holding both the chopstick on the left and the chopstick on the right.  Each philosopher can pick up an adjacent chopstick, when available and put it down when holding it. 




## Definition

check this link https://msdn.microsoft.com/en-us/magazine/dd882512.aspx