#pragma once
#include <vector>
#include <atomic>
#include <iostream>
#include <thread>
#include <queue>
#include <algorithm> 
#include <optional>

//#include <windows.h>
//#include <malloc.h>

static std::size_t max_capacity = 10000;		//Set Limitations for Number of Nodes
static std::atomic<int> is_created = 0;

//Definition for Edge Data Type
struct Node {
	std::pair<std::size_t, std::atomic<std::size_t>> edge;
	std::atomic<Node*> Next;
	Node(std::size_t number, std::size_t label);
};

//definition for LockFreeLinkedList
class LockFreeLinkedList {
private:
    
public:
	std::atomic<Node*> head;
    std::atomic<Node*> tail;
    //size of the LinkedList
    std::atomic<size_t> size;

	LockFreeLinkedList();
	
	//add Node before the tail of the LinkedList
	void add_Node(std::size_t i, std::size_t j);
};

//definition for LockFreeGraph
class LockFreeGraph
{
private:
	
public:
	//current Number of Nodes
	std::atomic<std::size_t> current_capacity = 0;  
	
	//graph data structure
	std::atomic<LockFreeLinkedList*> adjacency_list;
	
	// Constructor 
	LockFreeGraph();
	//1. create graph and set the given max_capacity
	void create_graph(std::size_t max_capacity); 
	//2. create a new vertex if it is not reach the maximum capcity
	void add_vertex();
	//3. add directed edge(i,j) to the graph, if already exists, then set the lebel to 0.
	void add_edge(std::size_t i, std::size_t j);
	//4. increase the label of the edge
	void inc_label(std::size_t i, std::size_t j, std::size_t increment);
	//5. decrease the label of the edge
	void dec_label(std::size_t i, std::size_t j, std::size_t increment);
	//6. check if two vertex are connected, return true if connected¡£
	//otherwise return false;
	bool are_connected(std::size_t i, std::size_t j);
	//7. check if two vertex are reachable, return true if reachable, else return false
	bool is_reachable(std::size_t i, std::size_t j);
	//8. return the length of shortest path given two vertex, return -1 if the path does not exist
	std::size_t shortest_path(std::size_t i, std::size_t j);
	//10.return the length of diameter of the graph
	std::size_t get_diameter();

	//check the validation of vertices
	bool is_vertex_valid(std::size_t i, std::size_t j);
	//check the validation of graph
	bool is_graph_created();
};

