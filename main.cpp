#include <iostream> // cout
#include <fstream> // ifstream
#include <time.h> // clock_t, clock, CLOCKS_PER_SEC
#include <math.h> // pow, sqrt
#include <string> // getline
#include <stdlib.h> // srand, rand
#include <time.h> // time
#include <vector> // vector
#include <algorithm> // swap

using namespace std;

class PQ {
private:
	vector<float> heap; // holds taxi distances in heap data structure
public:
	PQ() {}
	~PQ() {}
	void addTaxi(float longitude, float latitude); // adds taxi to the heap
	float calculateDistance(float longitude, float latitude); // Euclidian distance
	void updateDistance(int key, float subtrahend); // updates variables and calls calculateDistance()
	int getSize(); // returns size of heap data struct
};

void PQ::addTaxi(float longitude, float latitude) {
	// add new taxi's distance to the end of heap
	this->heap.push_back(calculateDistance(longitude, latitude));
}

int PQ::getSize() {
	return this->heap.size(); // return heap size
}

float PQ::calculateDistance(float longitude, float latitude) {
	float distance = sqrt(pow(longitude, 2) + pow(latitude, 2));
	return distance; // calculate and return Euclidian distance
}

void PQ::updateDistance(int key, float subtrahend) { // should be between 0 and 1
	this->heap[key] -= subtrahend; // reduce distance by subtrahend
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

	for (int i = 0; i < m; i++) { // do m amount of operations
		if(i == 100) { // after every 100 operations, 101th operation is a remove operation
			// remove taxi
			// print removed taxi
		} else {
			mode = (rand() % 100) < (p * 100);
			if(mode == true) {
				// update taxi (distance = distance * 0.1)
				num_of_distance_updates++;

				queue->updateDistance(rand() % queue->getSize(), 0.01); // reduce a random taxi's distance by 0.01
			} else {
				// add taxi
				num_of_taxi_additions++;
				
				float longitude, latitude;
				file >> longitude;
				file >> latitude;
				getline(file, line, '\n'); //this is for reading the \n character into dummy variable.

				queue->addTaxi(longitude, latitude);
			}
		}
	}

	// simulation ends
	cout << "Number of taxi additions: " << num_of_taxi_additions << endl;
	cout << "Number of distance updates: " << num_of_distance_updates << endl;
    cout << "The total running time of the program was " << ( ( (float) time_elapsed ) / (CLOCKS_PER_SEC / 1000) ) << " milliseconds." << endl;

}