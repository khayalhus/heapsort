#include <iostream> // cout, endl
#include <fstream> // ifstream
#include <time.h> // clock_t, clock, CLOCKS_PER_SEC
#include <string> // getline
#include <stdlib.h> // srand, rand
#include <time.h> // time

#include "heap.h"

using namespace std;

int main(int argc, char** argv){
	clock_t time_elapsed; // used for storing total running time

	time_elapsed = clock(); // store startup time

	srand(time(NULL)); // seed randomization with time

    int m;
	float p;

    if (argc > 2) {
		m = atoi(argv[1]); // get number m from console argument (./a.out m p)
        p = atof(argv[2]); // get number p from console argument (./a.out m p)
	} else if (argc > 1) {
        m = atoi(argv[1]); // get number m from console argument (./a.out m)
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
	
	getline(file, line); // this is the header line

	string header = line;

	bool mode; // determines whether simulation should add or update (true = update, false = add)

	int num_of_taxi_additions = 0;
	int num_of_distance_updates = 0;
	//int num_of_removals = 0;
	for (int i = 1; i <= m; i++) { // do m amount of operations
		if(i % 102 == 0 && i != 0) { // after every 100 add or decrease operation, 101th operation is a remove operation
			try {
				long double taxi_distance = queue->extract_min(); // remove taxi
				cout << "Distance of the removed taxi: " << taxi_distance << endl; // print removed taxi
				//num_of_removals++;
			}
    		catch (const char* result) {
        		//cout << result << endl;
    		}
		} else {
			mode = (rand() % 100) < (p * 100); // randomly select an operation based on probabilities determined by console argument p
			if(mode == true) {
				// update taxi (distance = distance - 0.01)
				if(queue->getSize() == 0) {
                    //cout << "Can't update non-existent taxi" << endl;
                    continue; // skip iteration if there are no taxis to update in heap
                }
				num_of_distance_updates++;
				int index = rand() % queue->getSize(); // randomly choose a node
				long double reduced_value = queue->getKey(index) - 0.01; // reduce chosen node by 0.01
				if(reduced_value <= 0.0) {
					queue->decrease_distance(index, 0.0); // distance can't be negative so reduce a random taxi's distance to 0.0
				} else {
					queue->decrease_distance(index, reduced_value); // reduce a random taxi's distance by 0.01
				}
			} else {
				// add taxi
				num_of_taxi_additions++;
				
				// get a taxi from file
				long double longitude, latitude;
				file >> longitude;
				file >> latitude;
				getline(file, line, '\n'); //this is for reading the \n character into dummy variable.

				queue->addTaxi(longitude, latitude);
			}
		}
	}

	delete queue;

	time_elapsed = clock() - time_elapsed; // calculate total simulation running time

	// simulation ends
	//cout << "Number of taxi removals: " << num_of_removals << endl;
	cout << "Number of taxi additions: " << num_of_taxi_additions << endl;
	cout << "Number of distance updates: " << num_of_distance_updates << endl;
    cout << "The total running time of the program was " << ( ( (float) time_elapsed ) / (CLOCKS_PER_SEC / 1000) ) << " milliseconds." << endl;

}