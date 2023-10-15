# CSC591 Directed Study A1

Description:

This project is a C++ Implementation of a Lock-Free Graph using CAS and FAA, all functions are thread-safe, lock-free, and linearizable. The project consists of two files, the .cpp file contains the function and description, .h file contains the implementation for each function. The proof of linearizability for each function is in the report section of this ReadME file.


Build:


license:


Report:

In this part, every function in the API will be proven its linearizability and the results of some experiment tests.

Note: All functions except 'create_graph' will check whether a graph is created or not. All functions except 'create_graph', 'add_vertex' will check whether the vertex(es) is/are created or not.

1.create_graph()



2.add_vertex()



3.add_edge()

References:
