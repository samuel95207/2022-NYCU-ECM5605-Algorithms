#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <algorithm>
#include <sys/resource.h>
#include <cmath>
#include <limits.h>
#include "answer.h"
using namespace std;

// When I grade the homework, I'll use this code to validate your code.
// Please make sure that your code can compile with this code.

int main(int argc, char* argv[]){
    /* Input check */
	if (argc < 2){
		cout << "Usage: " << argv[0] << " <size of the sequence n> <random seed (integer)>" << endl;
		return -1;
	}
    long size_of_sequence = atol(argv[1]);
	int rand_seed = atoi(argv[2]);

	/*	Generate the sequence */
	srand( rand_seed );
	vector<long> input_array(size_of_sequence);
	// Generate a sorted sequence
	long number = max(-5*size_of_sequence, LONG_MIN);
	for (long i = 0; i < size_of_sequence; i++){
		number = min(LONG_MAX, number+(rand()%11));
		input_array[i] = number;
	}
	// Mess up some of the order
	for (long i = 0; i < size_of_sequence*0.05; i++){
		long pos_1 = rand()%size_of_sequence;
		long pos_2 = rand()%size_of_sequence;
		swap(input_array[pos_1], input_array[pos_2]);
	}
	// Copy the input for verification
	vector<long> copied_input_array = input_array;

    /* Run the code */
    const clock_t start_time = clock();
    vector<long> answer_permutation = my_sort(input_array);
    double run_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;

    /* Get the memory usage*/
    struct rusage r_usage;
	getrusage(RUSAGE_SELF,&r_usage);


    /* Examine your answer*/
	// Check the size of the result
	if (answer_permutation.size() != size_of_sequence){
		cout << "Error: The size of your answer does not match the size of the input sequence" << endl;
		return -1;
	}

	// Initial a list to see if there're duplicated elements or out-of-range position
	vector<bool> is_visited(size_of_sequence);
	for (long i = 0; i < size_of_sequence; i++){
		is_visited[i] = false;
	}
	for (long i = 0; i < size_of_sequence; i++){
		long position = answer_permutation[i];
		if (position < 0 || position >= size_of_sequence){
			cout << "Error: position of the number is out-of-range" << endl;
			return -1;
		}

		if (is_visited[i]){
			cout << "Error: the position is overlap with another number" << endl;
			return -1;
		}
		is_visited[i] = true;
	}

	// Check whether the answer is correctly sorted
	vector<long> answer_sequence(size_of_sequence);
	for (long i = 0; i < size_of_sequence; i++){
		long position = answer_permutation[i];
		answer_sequence[position] = copied_input_array[i];
	}
	for (long i = 0; i < size_of_sequence-1; i++){
		if (f(answer_sequence[i]) > f(answer_sequence[i+1])){
			cout << "Error: the sequence is not sorted properly" << endl;
			return -1;
		}
	}


    cout << "Your result is valid." << endl;
    cout << "=====  Answer Report  =====" << endl;
    cout << "CPU Run time: " << run_time << " seconds" << endl;
	cout << "Mem Usage:    " << r_usage.ru_maxrss << " k-bytes" << endl;

    return 0;
}
