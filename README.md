# CSC591 Directed Study A1

Description:

This project is a C++ Implementation of a Lock-Free Graph using CAS and FAA, all functions are thread-safe, lock-free, and linearizable. The project consists of two files, the .cpp file contains the function and description, .h file contains the implementation for each function. The proof of linearizability for each function is in the report section of this ReadME file.


Build:


license:


Report:

In this part, every function in the API will be proven its linearizability and the results of some experiment tests.

Note: All functions except 'create_graph' will check whether a graph is created or not. All functions except 'create_graph', 'add_vertex' will check whether the vertex(es) is/are created or not.

1.create_graph()

An atomic int global variable 'is_created' is used to indicate whether a graph is created or not, the graph will only be created when 'is_created' is equal to 0. We use a local variable to record the value before the fetch_add function and check if this local variable equal 0, we create the graph. Since fetch_add is an atomic operation, so it is linearizable and only 1 local variable in 1 thread will record a 0, thus, only 1 graph will be created successfully.

2.add_vertex()

An atomic size_t counter 'current_capacity' is used to record the number of vertexes that have been added. A CAS loop is used to increment the value 'current_capacity' and it is the only operation to change its value. Before the CAS loop, we check if the current_capacity is less than max_capacity, in every iteration, again we check if the current_capacity is less than max_capacity, and then do the CAS. This makes sure that the increment only happens when 'current_capacity' is less than the 'max_capacity'. The increment is done by CAS loop atoimically which is linearizable.


3.add_edge()
In the first step, the function will go through the linked list that contains all edges that the vertex 'i' connects, if it finds the edge to 'j', then a atomic store() is used to set the weight of the edge to 0. Since the delete_edge operation is not considered, it is correct that the weight should always set to 0 if it is existed. Since the write operation is atomic, then the other threads whatever it is read or write to the edge can notice the change imediately, thus, the first step is linearizable.

In the second step, the edge hasn't found this new edge needs to be added to the linked list.

References:
