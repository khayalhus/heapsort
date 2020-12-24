#include <iostream> // cout
#include <fstream> // ifstream
#include <time.h> // clock_t, clock, CLOCKS_PER_SEC
#include <math.h> // pow, sqrt
#include <string> // getline
#include <stdlib.h> // srand, rand
#include <time.h> // time
#include <vector> // vector
#include <algorithm> // swap
#include <limits> // numeric_limits<float>::max()

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
	int left = 2 * index + 1;
	int right = 2 * index + 2;

	int smallest = index;

	if (left < this->getSize() && this->heap[left] < this->heap[index])
		smallest = left;

	if (right < this->getSize() && this->heap[right] < this->heap[smallest])
		smallest = right;

	if (smallest != index) {
		swap(this->heap[index], this->heap[smallest]);
		min_heapify(smallest);
	}
}

void PQ::addTaxi(long double longitude, long double latitude) {
	// add new taxi's distance to the end of heap
	long double key = calculateDistance(longitude, latitude); // add new taxi to the end
	this->heap.push_back(numeric_limits<long double>::max());
	this->decrease_distance(this->getSize() - 1, key);
	//min_heapify(this->getSize() - 1); // swap it into its correct place
}

int PQ::getSize() {
	return this->heap.size(); // return heap size
}

long double PQ::calculateDistance(long double longitude, long double latitude) {
	long double distance = sqrtl(powl(longitude, 2) + powl(latitude, 2));
	return distance; // calculate and return Euclidian distance
}


int main(int argc, char** argv){
	clock_t time_elapsed; // used for storing total running time

	time_elapsed = clock();

	srand(time(NULL)); // seed randomization with time

    int m;
	float p;

    if (argc > 2) {
		m = atoi(argv[1]); // get number m from console argument (./a.out m p)
        p = atof(argv[2]); // get number p from console argument (./a.out m p)
	} else if (argc > 1) {
        m = atoi(argv[1]); // get number m from console argument (./a.out m p)
        p = 0; // default assignment for p
        cout << "p was not specified as an argument" << endl;
        cout << "Automatically setting p as 0.1" << endl;
    } else {
		m = 100; // default assignment for m
        p = 0.1; // default assignment for p
		cout << "neither m, nor p was specified as an argument" << endl;
		cout << "Automatically setting m as 100 and p as 0.1" << endl;
	}

	// simulation starts

	ifstream file;
	file.open("locations.txt");
	
	if (!file){
		cerr << "File cannot be opened!";
		exit(1);
	}

	PQ * queue = new PQ;

	string line;
	
	getline(file, line); //this is the header line

	string header = line;

	bool mode; // determines whether simulation should add or update (true = update, false = add)

	int num_of_taxi_additions = 0;
	int num_of_distance_updates = 0;

	for (int i = 1; i <= m; i++) { // do m amount of operations
		if(i % 102 == 0 && i != 0) { // after every 100 operations, 101th operation is a remove operation
			try {
				long double taxi_distance = queue->extract_min(); // remove taxi
				cout << "Distance of the removed taxi: " << taxi_distance << endl; // print removed taxi
			}
    		catch (const char* result) {
        		//cout << result << endl;
    		}
		} else {
			mode = (rand() % 100) < (p * 100);
			if(mode == true) {
				// update taxi (distance = distance - 0.01)
				if(queue->getSize() == 0) {
                    //cout << "Can't update non-existent taxi" << endl;
                    continue;
                }
				num_of_distance_updates++;
				int index = rand() % queue->getSize(); // randomly choose a node
				long double reduced_value = queue->getKey(index) - 0.01;
				if(reduced_value <= 0.0) {
					queue->decrease_distance(index, queue->getKey(index) - 0.0); // distance can't be negative so reduce a random taxi's distance to 0.0
				} else {
					queue->decrease_distance(index, reduced_value); // reduce a random taxi's distance by 0.01
				}
			} else {
				// add taxi
				num_of_taxi_additions++;
				
				long double longitude, latitude;
				file >> longitude;
				file >> latitude;
				getline(file, line, '\n'); //this is for reading the \n character into dummy variable.

				queue->addTaxi(longitude, latitude);
			}
		}
	}

	delete queue;

	time_elapsed = clock() - time_elapsed;

	// simulation ends
	cout << "Number of taxi additions: " << num_of_taxi_additions << endl;
	cout << "Number of distance updates: " << num_of_distance_updates << endl;
    cout << "The total running time of the program was " << ( ( (float) time_elapsed ) / (CLOCKS_PER_SEC / 1000) ) << " milliseconds." << endl;

}