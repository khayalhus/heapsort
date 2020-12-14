#include<iostream> // cout
#include<fstream> // ifstream
#include<time.h> // clock_t, clock, CLOCKS_PER_SEC
#include<math.h> // pow, sqrt

using namespace std;

class Taxi {
public:
	float longitude;
	float latitude;
	float distance;

	Taxi() {}
	~Taxi() {}
	void calculateDistance(); // Euclidian distance
	void updateDistance(float longitude, float latitude); // updates variables and calls calculateDistance()
};

void Taxi::calculateDistance() {
	this->distance = sqrt(pow(this->longitude, 2) + pow(this->latitude, 2));
}

void Taxi::updateDistance(float longitude, float latitude) {
	this->longitude = longitude;
	this->latitude = latitude;
	calculateDistance();
}

int main(int argc, char** argv){
	clock_t time_elapsed; // used for storing total running time

	time_elapsed = clock();

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

	ifstream file;
	file.open("locations.txt");
	
	if (!file){
		cerr << "File cannot be opened!";
		exit(1);
	}

	string line;
	
	getline(file, line); //this is the header line

	string header = line;
	
	// should arrays be used when the size is changing?

	file >> line; //longitude (float)
    // = stof(line)
	// somehow store read variable
	file >> line; //latitude (float)
    // = stof(line)
	// somehow store read variable
	getline(file, line, '\n'); //this is for reading the \n character into dummy variable.
	

    cout << "The total running time of the program was " << ( ( (float) time_elapsed ) / (CLOCKS_PER_SEC / 1000) ) << " milliseconds." << endl;

}