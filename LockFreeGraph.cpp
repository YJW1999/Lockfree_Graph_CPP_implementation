#include "LockFreeGraph.h"

//*********************************
//Node structure functions
Node::Node(const std::size_t number, const std::size_t label) {
	this->edge.first = number;
	this->edge.second = label;
	this->Next.store(nullptr);
}

//*********************************
//Linked List structure functions
LockFreeLinkedList::LockFreeLinkedList() {
    Node* dummy_node_head = new Node(0,0);
    Node* dummy_node_tail = new Node(0,0);
    this->head.store(dummy_node_head);
    this->tail.store(dummy_node_tail);
    this->head.load()->Next.store(tail);
}

void LockFreeLinkedList::add_Node(const std::size_t i, const std::size_t j) {
    Node* new_node = new Node(j, 0);
    Node* cur = this->head.load();
    Node* tmp_tail = this->tail.load();
    new_node->Next.store(tail);
    while(!cur->Next.compare_exchange_weak(tmp_tail, new_node)){
        while(cur->Next.load() != this->tail.load()){
			if (cur->edge.first == j) {
				cur->edge.second.store(0);
				return;
			}
            cur = cur->Next.load();
        }
        tmp_tail = this->tail.load();
    }
}

//*********************************
//LockFreeGraph functions implementation
LockFreeGraph::LockFreeGraph() {
	adjacency_list = nullptr;
}

void LockFreeGraph::create_graph(const std::size_t max_capacity){
    if (adjacency_list.load() == nullptr) {
        int i = is_created.fetch_add(1);
			if (adjacency_list.load() == nullptr && i==0) {
                adjacency_list.store(new LockFreeLinkedList[max_capacity + 1]); //+1 make the index exactly match the max_capacity
			}
			else {
				is_created.fetch_sub(1);
				std::cout << "The graph has been created!" << std::endl;
				return;
			}
    }
}

//add_vertex implementation using cas loop
void LockFreeGraph::add_vertex() {
	if (!is_graph_created()) {
		std::cout << "graph hasn't been created yet!" << std::endl;
		return;
	}

	std::size_t temp = current_capacity.load();
	if (temp < max_capacity) {
		while (!current_capacity.compare_exchange_weak(temp, temp + 1)) {
			if (temp+1 <= max_capacity)	{
				continue;
			}
			else {
				std::cout << "1 thread try adding vertex failed due to maximum capacity! Thread return nothing";
				return;
			}
		}
	}
	else {
		std::cout << "1 thread try adding vertex failed due to maximum capacity! Thread return nothing";
		return;
	}
}

void LockFreeGraph::add_edge(const std::size_t i, const std::size_t j) {
	if (!is_graph_created()) {
		std::cout << "Graph hasn't been created yet!" << std::endl;
		return;
	}
	if (!is_vertex_valid(i, j)) {
		return;
	}
	Node* cur = adjacency_list.load()[i].head.load();
	while (cur) {
		if (cur->edge.first == j) {
			cur->edge.second.store(0);
			return;
		}
		cur = cur->Next.load();
	}
	adjacency_list[i].add_Node(i, j);
}

void LockFreeGraph::inc_label(const std::size_t i, std::size_t j, const std::size_t increment) {
	if (!is_graph_created()) {
		std::cout << "Graph hasn't been created yet!" << std::endl;
		return;
	}
	if (!is_vertex_valid(i, j)) {
		return;
	}
	Node* cur = adjacency_list.load()[i].head.load();
	while (cur) {
		if (cur->edge.first == j) {
			cur->edge.second.fetch_add(increment);
			std::cout << "add succeed!" << std::endl;
			return;
		}
		cur = cur->Next.load();
	}
}

void LockFreeGraph::dec_label(const std::size_t i, std::size_t j, const std::size_t decrement) {
	if (!is_graph_created()) {
		std::cout << "Graph hasn't been created yet!" << std::endl;
		return;
	}
	if (!is_vertex_valid(i, j)) {
		return;
	}
	size_t tmp_weight;
	size_t decreased_val;
	Node* cur = adjacency_list.load()[i].head.load();
	while (cur) {
		tmp_weight = cur->edge.second.load();
		decreased_val = tmp_weight - decrement;
		if (cur->edge.first == j && tmp_weight >= decrement ) {
			while (!cur->edge.second.compare_exchange_weak(tmp_weight, decreased_val)) {
				tmp_weight = cur->edge.second.load();
				decreased_val = tmp_weight - decrement;
				if (tmp_weight < decrement) {
					std::cout << "Dec_label Error: decrement will result the weight of edge less than 0!" << std::endl;
					return;
				}
			}
			return;
		}
		else if (cur->edge.first == j && cur->edge.second.load() < decrement) {
			std::cout << "Dec_label Error: decrement will result the weight of edge less than 0!" << std::endl;
			return;
		}
		cur = cur->Next.load();
	}
}

bool LockFreeGraph::are_connected(const std::size_t i, const std::size_t j) {
	if (!is_graph_created()) {
		std::cout << "Graph hasn't been created yet!" << std::endl;
		return false;
	}
	if (!is_vertex_valid(i, j)) {
		return false;
	}
    Node* cur = adjacency_list.load()[i].head.load();
    while (cur) {
        if (cur->edge.first == j) {
            return true;
        }
        cur = cur->Next.load();
    }
    return false;
}

bool LockFreeGraph::is_reachable(const std::size_t i, const std::size_t j) {
	if (!is_graph_created()) {
		std::cout << "Graph hasn't been created yet!" << std::endl;
		return false;
	}
	if (!is_vertex_valid(i, j)){
		return false;
	}
	std::queue<std::size_t> BFSqueue;
	BFSqueue.push(i);
	while (!BFSqueue.empty()) {
		size_t front = BFSqueue.front();
		BFSqueue.pop();
		Node* tmp = adjacency_list.load()[front].head.load()->Next.load();
		while (tmp != adjacency_list.load()[front].tail.load()) {
			if (tmp->edge.first == j) {
				return true;
			}
			else {
				BFSqueue.push(tmp->edge.first);
			}
			tmp = tmp->Next.load();
		}
	}
	return false;
}

//cmp function for shortest_path's priority queue
struct cmp{
	bool operator()(std::vector<std::size_t>& a, std::vector<std::size_t>& b) {
		return a[2] > b[2];
	}
};

std::size_t LockFreeGraph::shortest_path(std::size_t i, std::size_t j) {
	if (!is_graph_created()) {
		std::cout << "Graph hasn't been created yet!" << std::endl;
		return -1;
	}
	if (!is_vertex_valid(i, j)) {
		return -1;
	}
	if (!is_reachable(i, j)) {
		std::cout << "shortest path: The path from i to j does not exist!" << std::endl;
		return 0;
	}
	//used to store the distance from src to all other nodes;
	std::vector<std::optional<std::size_t>> distances(max_capacity + 1, std::nullopt);
	distances[0] = 0;
	//store vertex and return the vertex that has the smallest distance when pop
	std::vector<std::vector<std::size_t>> pq;
	Node* tmp = adjacency_list.load()[i].head.load()->Next.load();
	//append adjacent vertices for the src vertex
	while(tmp != adjacency_list.load()[i].tail.load()){
		size_t weight = tmp->edge.second.load();
		distances[tmp->edge.first] = weight;
		std::vector<std::size_t> src_adjacent = {i, tmp->edge.first, weight};
		pq.emplace_back(src_adjacent);
		tmp = tmp->Next.load();
	}
	std::make_heap(pq.begin(), pq.end(),cmp());
	while (!pq.empty()) {
		std::pop_heap(pq.begin(), pq.end(), cmp());
		std::vector<std::size_t> nearest_vertex = pq.back();
		pq.pop_back();
		Node* tmp = adjacency_list.load()[nearest_vertex[1]].head.load()->Next.load();
		while (tmp != adjacency_list.load()[nearest_vertex[1]].tail.load() &&
			tmp != adjacency_list.load()[nearest_vertex[1]].tail.load()) {
			size_t weight = tmp->edge.second.load();
			if (distances[tmp->edge.first] == std::nullopt) {
				distances[tmp->edge.first] = distances[nearest_vertex[1]].value() + weight;
				std::vector<std::size_t> adjacent = { nearest_vertex[1], tmp->edge.first , weight };
				pq.push_back(adjacent);
				std::push_heap(pq.begin(), pq.end(), cmp());
			}
			else if (distances[nearest_vertex[1]].value() + weight < distances[tmp->edge.first].value()) {
				distances[tmp->edge.first] = distances[nearest_vertex[1]].value() + weight;
			}
			tmp = tmp->Next.load();
		}
	}
	size_t result = distances[j].value();
	return result;
}

std::size_t LockFreeGraph::get_diameter() {
	size_t longest_distance = 0;
	size_t capacity = current_capacity.load();
	for (size_t i = 1; i <= capacity; ++i) {
		for (size_t j = 1; j <= capacity; ++j) {
			if (i != j && is_reachable(i, j)) {
				size_t tmp = shortest_path(i, j);
				if (longest_distance < tmp) {
					longest_distance = tmp;
				}
			}
		}
	}
	return longest_distance;
}

bool LockFreeGraph::is_vertex_valid(const std::size_t i, const std::size_t j) {
	size_t tmp = current_capacity.load();
	if (i > tmp || j > tmp) {
		std::cout << "add_edge error: one of the vertex has not yet been created!" << std::endl;
		return false;
	}
	else if (i == j) {
		std::cout << "add_edge error: the edge are connecting the same vertex!" << std::endl;
		return false;
	}
	return true;
}

bool LockFreeGraph::is_graph_created() {
	return is_created.load() > 0;
}