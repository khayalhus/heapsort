#include <iostream> // cout, endl
#include <math.h> // pow, sqrt
#include <algorithm> // swap
#include <limits> // numeric_limits<long double>::max()

#include "heap.h"

#define HOTEL_LONGITUDE 33.40819
#define HOTEL_LATITUDE 39.19001

using namespace std;

long double PQ::getKey(int index) {
	return this->heap[index];
}

int PQ::getParentIndex(int index) {
	return (index-1) / 2;
}

void PQ::decrease_distance(int index, long double key) {
	if (key > this->heap[index]) {
		cout << "New key is bigger than the value" << endl;
		return;
	}
	this->heap[index] = key;
	while(index > 1 && this->heap[this->getParentIndex(index)] > this->heap[index]) { // if parent is bigger than child
		swap(this->heap[index], this->heap[this->getParentIndex(index)]); // swap parent and child
		index = this->getParentIndex(index); // check the parent of the parent
	}
}

long double PQ::extract_min() {
	long double min = 0.0;
	if (this->getSize() == 0) {
		throw "Heap data structure is empty";
	}
	min = this->heap[0];
	swap(this->heap[0], this->heap[this->getSize() - 1]); // swap first and last nodes
	this->heap.pop_back(); // remove the last element
	min_heapify(0); // heapify the new root
	return min;
}

void PQ::min_heapify(int index) {
	int left = 2 * index + 1; // left child index
	int right = 2 * index + 2; // right child index

	int smallest = index;
    // compare left child and parent
	if (left < this->getSize() && this->heap[left] < this->heap[index])
		smallest = left;
    // compare right child and result of the previous comparison
	if (right < this->getSize() && this->heap[right] < this->heap[smallest])
		smallest = right;
    // if parent is not the smallest distance, swap
	if (smallest != index) {
		swap(this->heap[index], this->heap[smallest]);
		min_heapify(smallest); // check newly changed distance
	}
}

void PQ::addTaxi(long double longitude, long double latitude) {
	// add new taxi's distance to the end of heap
	long double key = calculateDistance(longitude, latitude); // get distance between taxi and hotel
	this->heap.push_back(numeric_limits<long double>::max()); // add "positive infinity" node
	this->decrease_distance(this->getSize() - 1, key); // reduce newly added node to taxi's distance
}

int PQ::getSize() {
	return this->heap.size(); // return heap size
}

long double PQ::calculateDistance(long double longitude, long double latitude) {
	long double distance = sqrtl(powl(longitude - HOTEL_LONGITUDE, 2) + powl(latitude - HOTEL_LATITUDE, 2));
	return distance; // calculate and return Euclidian distance between two points
}