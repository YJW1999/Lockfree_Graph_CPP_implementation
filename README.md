# CSC591 Directed Study A1

Description:

This project is a C++ Implementation of a Lock-Free Graph using CAS and FAA, all functions are thread-safe, lock-free, and linearizable. The project consists of two files, the .cpp file contains the function and description, .h file contains the implementation for each function. The proof of linearizability for each function is in the report section of this ReadME file.


Build:


license:


Report:

In this part, every function in the API will be proven its linearizability and the results of some experiment tests.

Note: All functions except 'create_graph' will check whether a graph is created or not. All functions except 'create_graph', 'add_vertex' will check whether the vertex(es) is/are created or not. The linearizablity will be proved in their seperate part. 

1.create_graph()

We assume that all read and write(fetch_add, fetch_sub) to the 'is_created' are atomic. In the main path, if the adjacency points to a nullptr, then 'is_created' will add 1. Also, a local int varaible i will record the value before adding. Then there are two paths.

In the first path, if adjacency_list is still point to nullptr and i is 0, then create the graph. The linearization point is after writing to adjacency_list, the write to 'adjacency_list' is atomic, so the state is still consistent before the writing and after the writing, thus, it is linearizable. 

In the second path, it also relies on local variable i, the linearization point is after fetch_sub, since it is also a atomic write operation, the state is still consistent, thus, it is linearizable.


2.add_vertex()

We assume that all read and write(fetch_add, fetch_sub) to the 'is_created' are atomic. Also the comparison of 'temp' and 'current_capacity' is atomic. 

In the first path, it also relies on local variable temp, inside the CAS loop, for every iteration, there are two paths, these two path are all repy on local variable and don't change any states, these two paths are linearizable. The linearization point of this main path is after the CAS loop, it increases the 'current_capacity' by 1, since it is done by CAS which is atomic, the state is consistent between before writing and after writing, therefore this path is linearizable. 

In the second path, is relies on local variable temp and won't change any state, thus, it is linearizable. 


3.add_edge()

We assume all read and write are atomic.


References:
