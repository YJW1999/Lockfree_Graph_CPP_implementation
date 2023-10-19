# CSC591 Directed Study A1

Description:

This project is a C++ Implementation of a Lock-Free Graph using CAS and FAA, all functions are thread-safe, lock-free, and linearizable. The project consists of two files, the .cpp file contains the function and description, .h file contains the implementation for each function. The proof of linearizability for each function is in the report section of this ReadME file.



Build:

build executable file: [g++ -std=17 main.cpp, LockFreeGraph.cpp -o test] 

run executable file:   [./test]



License:

Copyright (c) [2023] [Yijie Wu]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.



Report:

In this part, every function in the API will be proven its linearizability and the results of some experiment tests.

Note: All functions except 'create_graph' will check whether a graph is created or not. All functions except 'create_graph', and 'add_vertex' will check whether the vertex(es) is/are created or not. The linearizability will be proved in their separate part. 

0. is_vertex_valid()

We assume all read operations are atomic. There are two paths.

In the first path, the linearization point is in line 280, when 'i > tmp || j > tmp' returns true. This relies on a comparison between a local variable that atomically reads from 'current_capacity' and a const variable, thus, this path is linearizable. 

In the second path, the linearization point is in line 280, when 'i > tmp || j > tmp' returns false. This relies on a comparison between a local variable that atomically reads from 'current_capacity' and a const variable, thus, this path is linearizable. 


0.1 Is_graph_created()

We assume the only one read operation is atomic.

There is only 1 path, the linearization point is in line 292 which relies on a comparison between an atomic read value and an int, thus, the function is linearizable. 


1.create_graph()

We assume that all read and write(fetch_add, fetch_sub) to the 'is_created' are atomic. In the main path, the linearization point is in line 45, 'int i = is_created.fetch_add(1)', the write is linearizable because it is an atomic write. Since it relies on a condition check in line 44 'adjacency_list.load() == nullptr'. One is atomic read, and the other one is nullptr, thus, the main path is linearizable. There are two sub-paths.

In the first path, the linearization point is in line 47, 'adjacency_list.store(new LockFreeLinkedList{max_capacity}+1)', this write is linearizable because it is an atomic write. Since it relies on line 46 when 'adjacency_list.load() == nullptr && i==0' is true, these two conditions are compared with atomic read value or local variable or nullptr or 0, thus, the path is linearizable. 

In the second path, the linearization point is in line 50 'is_created.fetch_sub(1)', this write is linearizable because it is an atomic write. This linearization point relies on line 46 'adjacency_list.load() == nullptr && i==0' is false, this is proved to be linearizable in the first path. Thus, the path is linearizable.


2.add_vertex()

We assume that all read and write(fetch_add, fetch_sub) to the 'is_created' are atomic. Also the comparison of 'temp' and 'current_capacity' is atomic. 

In the first path, The linearization point of this path is in line 66 '!current_capacity.compare_exchange_weak(temp, temp + 1)' is true, since it is done by CAS which is atomic, and the state is consistent between before writing and after writing. The first iteration relies on a condition check in line 65 where 'temp < max_capacity' is true, this comparison is between an atomic read value and a static variable. The rest iterations rely on the two paths inside the while loop in line 67 where 'temp+1 <= max_capacity' is true or false, the comparison is between an atomic read variable and a static variable, thus these two paths inside the while loop are all linearizable, thus this whole path is linearizable.

In the second path, the linearization point is in line 67 where 'temp+1 <= max_capacity' is false, this comparison is proved to be linearizable in the first path, thus this path is linearizable. 


3.add_edge()

We assume all read and write are atomic. 

There is only 1 path, the read of 'adjacency_list' is atomic. The while loop relies on the local variable 'cur', every read of cur is atomic. The linearization point in line 91 'cur' is true, this is an atomic read variable, thus it is linearizable. There is a subpath inside this path which relies on 1 local variable 'cur->edge.first' and const variable j in line 92 'cur->edge.first == j'. The linearization point in this sub path is after the atomic write in line 93 'cur->edge.second.store(0)', so still in a consistent state, thus the sub-path is linearizable. After the while loop, the last line is a function call on a consistent index of 'adjacency_list' as it won't change after being added.


3.1 add_Node()

We assume all read write and comparison of the second last Node and tail are atomic.

There is only 1 path. Even though there is an atomic write that points 'new_Node->Next' to the tail dummy Node in line 25, it is a local write, it won't change the global state. The reason is that it is a singly linked list and no other Node's Next point to new_Node, therefore, in other threads, it cannot read the new_Node. The linearization point is in line 26 where '!cur->Next.compare_exchange_weak(tmp_tail, new_node)' is true. The first CAS loop does not rely on anything, but the rest of the CAS loop iterations rely on 'cur' points to the tail and cur does not find the same edge. Thus, this is another while loop inside the CAS loop and two sub-paths inside this inner while loop. The inner while loop is a comparison between two atomic read variables shown in line 27, thus, it is linearizable. In the first sub-path, the linearization point is in line 29, 'cur->edge.second.store(0)'. Since it is an atomic write operation it relies on line 28 'cur->edge.first == j', a comparison between two consistent variables, thus it is linearizable. The second sub-path's linearization point is also in line 28 which has just been shown its linearizability, thus, the function is linearizable. 


4. inc_label()

We assume all read and write are atomic. 

There is only 1 path. The linearization point is in line 113 'cur->edge.second.fetch_add(increment)'. The fetch_add is an atomic write. Also, it relies on two conditions, the first one is 'cur' is true in line 111 which all read operations are done atomically. The second condition is in line 112 'cur->edge.first == j' is true, this comparison is between two consistent variables, thus, this path is linearizable.


5. dec_label()

We assume all reads and writes  are atomic, and the CAS loop is atomic.

The linearization point is in line 136 '!cur->edge.second.compare_exchange_weak(tmp_weight, decreased_val)' is true. The CAS is an atomic operation, so the state is consistent before and after the value changes. The first iteration of this CAS loop relies on two conditions. The first condition is in line 132 where the cur is true, and the cur reads value atomically in every iteration. The second condition is in line 135 'cur->edge.first == j && tmp_weight >= decrement' is true, clearly, these two comparisons are done between atomic read value, const value and local variable. The rest iterations of the CAS loop rely on the condition in line 139 'tmp_weight < decrement', one variable is an atomic read, and the other one is a minus of the first variable and a const variable. This condition guarantees that  the CAS will only succeed if the weight is greater than the decrement value. Thus, the function is linearizable. 


6. are_connected()

We assume all read operations are atomic. There are two paths.

In the first path, the linearization point is in line 164 'cur->edge.first == j', it does not change the state and relies on 1 consistent variable and 1 const variable, thus, this path is linearizable.

In the second path, the linearization point is in line 163 'cur' is false, it does not change the state and relies on 1 atomic read value, thus, this path is linearizable. 


7. Is_reachable()

We assume that all read operations from atomic variables are atomic. There are two paths in this function.

In the first path, the linearization point is in line 189 'tmp->edge.first == j', clearly this is a comparison between two consistent variables. It relies on two conditions. The first condition is how is the 'cur' read every time, the answer is that cur reads atomically in every iteration in the while loop. The second condition in line 182, '!BFSqueue.empty()' is true, clearly it is a local variable, so the state keeps consistent. Thus, this path is linearizable.

In the second path, the linearization point is in line 183, '!BFSqueue.empty()' is false which is a condition check for a local queue. This linearization point does not rely on any other condition, thus, this path is linearizable. 


8. shortest_path()

we assume that all read operations are atomic. 

There is only 1 path. The linearization point is in line 238 where '!pq.empty()' is false which is a condition check on a local queue object. It relies on the element in the queue which involves some read operations from an atomic object. Since these read operations are all atomic, thus, this path is linearizable. 


9. get_diameter()

we assume that all read operations are atomic. 

There is only 1 path. The linearization point is in line 265 'for (size_t i = 1; i <= capacity; ++i)' where i > capacity. The comparison is between a local variable and an atomic read value. This relies on a condition check in line 266 'j <= capacity' which is a comparison between a local variable and an atomic read value. Besides, there are two function calls in line 262, and 263 separately which has been shown linearizability before. Thus, the path is linearizable. 



References:
