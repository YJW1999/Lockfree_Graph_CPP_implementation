#include "LockFreeGraph.h"

int main() {
	LockFreeGraph LFG;

	std::vector<std::thread> threadPool;
	
	for (int i = 0; i < 1000; ++i) {
		threadPool.push_back(std::thread(&LockFreeGraph::create_graph, &LFG, max_capacity));
	}
	for (auto it = threadPool.begin(); it != threadPool.end(); it++) {
		it->join();
	}
	
	threadPool.clear();

	for (int i = 0; i < 6; ++i) {
		threadPool.push_back(std::thread(&LockFreeGraph::add_vertex, &LFG));
	}
	for (auto it = threadPool.begin(); it != threadPool.end(); it++) {
		it->join();
	}  

	std::cout << "Number of vertex created: " << LFG.current_capacity.load() << std::endl;

	threadPool.clear();

	/*for (int i = 1; i < 1001; ++i) {
		threadPool.push_back(std::thread(&LockFreeGraph::add_edge, &LFG, 1, i));
	}
	for (auto it = threadPool.begin(); it != threadPool.end(); it++) {
		it->join();
	}

	threadPool.clear();
	
	for (int i = 0; i < 100; ++i) {
		if (i % 2 == 0) {
			threadPool.push_back(std::thread(&LockFreeGraph::inc_label, &LFG, 1, 2, 1));
		}
		else {
			threadPool.push_back(std::thread(&LockFreeGraph::dec_label, &LFG, 1, 2, 1));
		}
	}
	for (auto it = threadPool.begin(); it != threadPool.end(); it++) {
		it->join();
	}

	threadPool.clear();

	std::thread t1 = std::thread(&LockFreeGraph::dec_label, &LFG, 1, 2, 1);
	t1.join();

	int counter = 0;
	Node* curr = LFG.adjacency_list.load()[1].head.load();
	while (curr) {
		counter++;
		if (curr->edge.first == 2) {
			std::cout << curr->edge.first << "  " << curr->edge.second << std::endl;
		}
		curr = curr->Next.load();
	}
	std::cout << "Number of the edges has added: " << counter-2 << std::endl;
	std::cout << "Is reacheable? 1 -> 1000: " << LFG.is_reachable(1, 1000) << std::endl;*/
	
	LFG.add_edge(1, 2);
	LFG.inc_label(1, 2, 1);
	LFG.add_edge(2, 3);
	LFG.inc_label(2, 3, 1);
	LFG.add_edge(3, 4);
	LFG.inc_label(3, 4, 5);

	LFG.add_edge(1, 5);
	LFG.inc_label(1, 5, 3);
	LFG.add_edge(5, 6);
	LFG.inc_label(5, 6, 1);
	LFG.add_edge(6, 4);
	LFG.inc_label(6, 4, 1);

	LFG.add_edge(2, 5);
	LFG.inc_label(2, 5, 1);

	//std::cout << "is reachable 1->4:" << LFG.is_reachable(1,4) << std::endl;
	std::cout << "shortest path 1->4: " << LFG.shortest_path(1, 4) << std::endl;
	std::cout << "The diameter of graph: " << LFG.get_diameter() << std::endl;

	return 0;
}
