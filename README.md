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

There is only 1 path, the read of 'adjacency_list' is atomic. The while loop is rely on local variable 'cur', every read of cur is atomic. There is a sub path inside this path which the comparison relies on local 1 local variable 'cur->edge.first' and const variable j, the linearization point in this sub path is after the atomic write, so still in a consistent state, thus the sub path is linearizable. After the while loop, the last line is a function call on a consistent index of 'adjacency_list' as it won't changed after added.


3.1 add_Node()

We assume all read write and comparison of second last Node and tail are atomic.

There is only 1 path. Even there is a atomic write that points 'new_Node->Next' to tail dummy Node, but it is a local write, it won't change the global state. The reason is that it is a singly linked list and no others Node's Next point to new_Node, therefore, in other threads, it cannot read the new_Node. There is a sub path inside the CAS loop, all read and write in the while loop are atomic, the write operation relies on two consistent variable 'cur->edge.first' and 'j' and it is atomic, thus, the sub path is linearizable. Back to the CAS loop, the adding process is done by CAS and the position it insersion is at the tail, this ensures that other threads that read the same linked list will notice the new Node and have to read it because it is at the tail. The state is consistent before the insersion and after the insersion. Thus, this path is linearizable. 


4. inc_label()

We assume all read and write are atomic. 

There is only 1 path. Firstly, a 'tmp' is read the head of linked list of vertex i, then use a while loop to go through the linkled list, all these read operations are atomic. Inside the while loop, there is a sub path that has a write operation. This relies on a comparison between two consistent variables. 


4.
References:
