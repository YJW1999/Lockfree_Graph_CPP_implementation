# CSC591 Directed Study A1

Description:

This project is a C++ Implementation of a Lock-Free Graph using CAS and FAA, all functions are thread-safe, lock-free, and linearizable. The project consists of two files, the .cpp file contains the function and description, .h file contains the implementation for each function. The proof of linearizability for each function is in the report section of this ReadME file.


Build:


license:


Report:

In this part, every function in the API will be proven its linearizability and the results of some experiment tests.

1.create_graph()

An atomic int global variable 'is_created' is used to indicate whether a graph is created or not, the graph will only be created when 'is_created' is equal to 0. There is only one operation to this variable, atomically add using fetch add. The linearization point is after the fetch_add to the is_created, so after that, this change is visible to all other threads, thus, the function is linearizable. 

2.add_vertex()

An atomic size_t counter 'current_capacity' is used to record the number of vertexes that have been added. The 'current_capacity will only be added if it is smaller than the maximum_capacity'. The linearization point is at where after the CAS loop. The CAS loop will succeed and add the value only if the number is less than the maximum capacity. After the linearization point, the new value of 'current_capacity' is visible to all other threads that read it, thus, the function is linearizable.

3.

References:
