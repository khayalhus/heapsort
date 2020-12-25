#ifndef HEAP
#define HEAP

#include <vector> // vector

using namespace std;

class PQ {
private:
	vector<long double> heap; // holds taxi distances in heap data structure
	int getParentIndex(int index);
public:
	PQ() {}
	~PQ() {}
	void addTaxi(long double longitude, long double latitude); // adds taxi to the heap
	long double calculateDistance(long double longitude, long double latitude); // Euclidian distance
	int getSize(); // returns size of heap data struct
	void min_heapify(int index); // heapify the interior node
	//void build_min_heap(); // heapify all interior nodes
	long double extract_min(); // extract head node (which is closest) and return its distance
	void decrease_distance(int index, long double key);
	long double getKey(int index);
};

#endif