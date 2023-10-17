# CSC591 Directed Study A1

Description:

This project is a C++ Implementation of a Lock-Free Graph using CAS and FAA, all functions are thread-safe, lock-free, and linearizable. The project consists of two files, the .cpp file contains the function and description, .h file contains the implementation for each function. The proof of linearizability for each function is in the report section of this ReadME file.


Build:


license:


Report:

In this part, every function in the API will be proven its linearizability and the results of some experiment tests.

Note: All functions except 'create_graph' will check whether a graph is created or not. All functions except 'create_graph', 'add_vertex' will check whether the vertex(es) is/are created or not. The linearizablity will be proved in their seperate part. 

0. is_vertex_valid()

We assume all read operations are atomic. There are two paths.

In the first path, the linearization point is in line 272, when 'i > tmp || j > tmp' returns true. This relies on a comparison between a local variable that atomically read from 'current_capacity' and a const variable, thus, this path is linearizable. 

In the second path, the linearization point is in line 272, when 'i > tmp || j > tmp' returns false. This relies on a comparison between a local variable that atomically read from 'current_capacity' and a const variable, thus, this path is linearizable. 


0.1 Is_graph_created()

We assume the only one read operation is atomic.

There is only 1 path, the linearization point is in line 284 which relies on a comparasion between a atomic read value and an int, thus, the function is linearizable. 


1.create_graph()

We assume that all read and write(fetch_add, fetch_sub) to the 'is_created' are atomic. In the main path, the linearization point is in line 45, 'int i = is_created.fetch_add(1)', the write is linearizable because it is an atomic write. Since it relies on a condition check in line 44 'adjacency_list.load() == nullptr'. One is atomic read, and the other one is nullptr, thus, the main path is linearizable. There are two sub paths.

In the first path, the linearization point is in line 47, 'adjacency_list.store(new LockFreeLinkedList{max_capacity}+1)', this write is linearizable because it is an atomic write. Since it is relies on the line 46 when 'adjacency_list.load() == nullptr && i==0' is true, this two conditions are compared with atomic read value or local variable or nullptr or 0, thus, the path is linearizable. 

In the second path, the linearization point is in line 50 'is_created.fetch_sub(1)', this write is linearizable because it is an atomic write. This linearization point relies on the line 46 'adjacency_list.load() == nullptr && i==0' is false, this is proved to be linearizable in the first path. Thus, the path is linearizable.


2.add_vertex()

We assume that all read and write(fetch_add, fetch_sub) to the 'is_created' are atomic. Also the comparison of 'temp' and 'current_capacity' is atomic. 

In the first path, The linearization point of this path is in line 66 '!current_capacity.compare_exchange_weak(temp, temp + 1)' is true, since it is done by CAS which is atomic, the state is consistent between before writing and after writing, this is linearizable. The first iteration relies on a condition check in line 65 where 'temp < max_capacity' is true, this comparison is between a atomic read value and a static variable, thus, this is linearizable. The rest iterations relies on the two paths inside the while loop in line 67 where 'temp+1 <= max_capacity' is true or false, the comparison is between an atomic read variable and a static variable, thus these two paths inside the while loop are all linearizable, thus this whole path is linearizable.

In the second path, the lineraization point is in line 67 where 'temp+1 <= max_capacity' is false, this comparison is proved to be linearizable in the first path, thus this path is linearizable. 


3.add_edge()

We assume all read and write are atomic. 

There is only 1 path, the read of 'adjacency_list' is atomic. The while loop is rely on local variable 'cur', every read of cur is atomic. The linearization point is in line 91 'cur' is true, this is a atomic read variable, thus it is linearizable. There is a sub path inside this path which relies on 1 local variable 'cur->edge.first' and const variable j in line 92 'cur->edge.first == j'. The linearization point in this sub path is after the atomic writev in line 93 'cur->edge.second.store(0)', so still in a consistent state, thus the sub path is linearizable. After the while loop, the last line is a function call on a consistent index of 'adjacency_list' as it won't changed after added.


3.1 add_Node()

We assume all read write and comparison of second last Node and tail are atomic.

There is only 1 path. Even there is a atomic write that points 'new_Node->Next' to tail dummy Node in line 25, but it is a local write, it won't change the global state. The reason is that it is a singly linked list and no others Node's Next point to new_Node, therefore, in other threads, it cannot read the new_Node. The linearization point is in line 26 where '!cur->Next.compare_exchange_weak(tmp_tail, new_node)' is true. The first CAS loop does not relies on nothing, but the rest of the CAS loop relies on 'cur' points to the tail. 


4. inc_label()

We assume all read and write are atomic. 

There is only 1 path. Firstly, a 'cur' reads the head of linked list of vertex i, then use a while loop to go through the linkled list, all these read operations are atomic. Inside the while loop, there is a sub path that has a write operation. This relies on a comparison between two consistent variables. The linearization point is after atomic write operation, since it is atomic, the state keeps consistent, thus, the function is linearizable. 


5. dec_label()

We assume all reads and writes are atomic, and the CAS loop is atomic.

In the main path, a 'cur' reads the head of linked list of vertex i, then use a while loop to go through the linked list, all these read operations are atomic. Inside the while loop, 'tmp_weight' atomically read the weight of 'cur', 'decreased_val' is the minus of two local variable. Then there are two sub paths.

In the first sub path, A CAS loop is used to change the weight of the label with the 'decreased_val', it relies on two comparisons between local_variables or consistent variables and also relies on that the decreased value is less than the weight of the edge(one of the condition is to check this). Inside the CAS loop, for every iteration, the value of 'tmp_weight' will be updated with an atomic read and the value of 'decreased_val' will be updated upon two local variables. Then, a re-comparison between decrement value and weight is done to ensure that the new weight value changed by other threads is still larger than the decrement value. The linearization point is after the CAS loop, and the state keeps consistent since the CAS loop is atomic, thus, this path is linearizable. 

In the second sub path, it ends the threads. It relies on two condition check, one condition is a comparison between two consistent variables, the other one is a comparison between an atomic read and a consistent variable, thus, this path is also linearizable. 


6. are_connected()

We assume all read operations are atomic.

There is only 1 path. A 'cur' atomically read the head of the source vertex's edge linked list, and then go through the list, all these read are atomic. There is one sub path inside the while loop, return 'true'. This relies on a comparison between two consistent variables, thus this sub path is linearizable. So, the path is linearizable. 


7. Is_reachable()

We assume that all read operations are atomic.

There is only 1 path. The while loop relies on a conditon check of whether the local queue is empty or not. In every iteration of the while loop, a 'tmp' is used to read one vertex's edge linked list, and this read is atomic. The inside while loop relies on a condition check which are based on a comparison between 'tmp' and the 'tail' of that linked list, since these two Nodes will not be deleted, so, these two Nodes are in a consistent states. In every iteration of the inside while loop, there are two sub paths. In the first sub path, in has a return which is relies on a comparasion between a consistent value and a local variable, thus, the first sub path is linearizable. In the second sub path, it also relies on the same condition as the first one, and it only write to a local queue, thus this sub path is also linearizable. So, this path is linearizable. 




References:
