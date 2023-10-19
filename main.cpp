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

	for (int i = 0; i < 1000; ++i) {
		threadPool.push_back(std::thread(&LockFreeGraph::add_vertex, &LFG));
	}
	for (auto it = threadPool.begin(); it != threadPool.end(); it++) {
		it->join();
	}  

	std::cout << "Number of vertex created: " << LFG.current_capacity.load() << std::endl;

	threadPool.clear();

	for (int i = 1; i < 100; ++i) {
		threadPool.push_back(std::thread(&LockFreeGraph::add_edge, &LFG, rand()%10, rand()%10));
	}
	for (auto it = threadPool.begin(); it != threadPool.end(); it++) {
		it->join();
	}

	threadPool.clear();
	
	for (int i = 0; i < 100; ++i) {
		if (i % 2 == 0) {
			threadPool.push_back(std::thread(&LockFreeGraph::inc_label, &LFG, 1, 2, 2));
		}
		else {
			threadPool.push_back(std::thread(&LockFreeGraph::dec_label, &LFG, 1, 2, 1));
		}
	}
	for (auto it = threadPool.begin(); it != threadPool.end(); it++) {
		it->join();
	}

	threadPool.clear();

	std::cout << "shortest path 1->4: " << LFG.shortest_path(1, 2) << std::endl;
	std::cout << "The diameter of graph: " << LFG.get_diameter() << std::endl;

	return 0;
}
