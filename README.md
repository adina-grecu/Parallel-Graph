##Parallel Graph Traversal
This project implements a parallel depth-first search (DFS) traversal of a graph using a thread pool. The goal is to efficiently explore the graph and calculate the sum of values associated with visited nodes. The program utilizes multi-threading to process nodes concurrently, improving the overall execution time.

##Implementation
The project is implemented in C using the Pthreads library for parallel processing. It consists of several key components:

os_graph: Contains functions for creating a graph data structure from an input file.
os_threadpool: Implements a thread pool with task queue functionality for parallel processing.
os_list: Provides a basic linked list implementation for managing tasks in the thread pool.
The main program (main.c) reads an input graph from a file, creates a thread pool, and initializes a mutex lock for synchronization. It then creates a task for each unvisited node in the graph and adds it to the task queue. The tasks are processed concurrently by multiple threads in the thread pool.

The processNode function is the core of the program's logic. It takes a node index as an argument and performs the following steps:

Iterates through the node's neighbors and creates tasks for unvisited neighbors.
If the current node is unvisited, marks it as visited, and adds its value to the global sum variable.
Uses a mutex lock to ensure thread safety when accessing and updating the sum variable.
After all tasks have been processed, the program stops the thread pool and prints the final sum of visited node values.

The project makes use of parallelism and thread pooling to optimize the traversal of the graph, allowing for efficient exploration and calculation of the sum.



