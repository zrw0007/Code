#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <time.h>
#include <vector>

using namespace std;
//calculates the max value of 2 inputs
int max(int val1, int val2)
{
	if (val1 < val2)
	{
		return val2;
	}
	else
	{
		return val1;
	}
}

//the givien algorithms
int Algorithm1(int X[], int arraySize)
{
	int maxSoFar = 0;
	int sum = 0;
	int Q = arraySize;

	for (int L = 0; L < Q; L++)
	{
		for (int U = L; U <= Q; U++)
		{
			sum = 0;
			for (int I = L; I < U; I++)
			{
				sum = sum + X[I];
			}
			maxSoFar = max(maxSoFar, sum);
		}
	}
	return maxSoFar;
}
int Algorithm2(int X[], int arraySize)
{
	int maxSoFar = 0;
	int Q = arraySize;
	int sum = 0;
	for (int L = 0; L < Q; L++)
	{
		sum = 0;
		for (int U = L; U < Q; U++)
		{
			sum = sum + X[U];
			maxSoFar = max(maxSoFar, sum);
		}
	}
	return maxSoFar;
}
int Algorithm3(int X[], int first, int last)
{
	int L = first;
	int U = last;
	int maxToLeft = 0;
	int maxToRight = 0;
	int sum = 0;
	int M = 0;
	if (L > U)
	{
		return 0;
	}
	else if (L == U)
	{
		return max(0, X[L]);
	}
	else
	{
		M = floor((L + U) / 2);
		sum = 0;
		maxToLeft = 0;
		for (int I = L; I < M; I++)
		{
			sum = sum + X[I];
			maxToLeft = max(maxToLeft, sum);
		}

		sum = 0;
		maxToRight = 0;
		for (int J = M + 1; J < U; J++)
		{
			sum = sum + X[J];
			maxToRight = max(maxToRight, sum);
		}
		int maxCrossing = maxToLeft + maxToLeft;
		int maxInA = Algorithm3(X, L, M);
		int maxInB = Algorithm3(X, M + 1, U);
		int maxTotal = max(maxCrossing, max(maxInA, maxInB));
		return maxTotal;
	}
}
int Algorithm4(int X[], int Q)
{
	int maxSoFar = 0;
	int maxEndingHere = 0;
	for (int I = 0; I < Q; I++)
	{
		maxEndingHere = max(0, maxEndingHere + X[I]);
		maxSoFar = max(maxSoFar, maxEndingHere);
	}
	return maxSoFar;
}

//creates a .txt file with 19 arrays of size 10,15,20...,95,100 with values from -10 to 10
void getRandomArrays(fstream& fileOut)
{
	srand(time(0));
	for (int j = 10; j <= 100; j = j + 5)//generates each of the sequences from 10 to 100
	{
		vector<int> sequence(j);
		for (int i = 0; i < j; i++)
		{
			sequence[i] = rand() % 21 - 10;
			fileOut << sequence[i];
			if (i < j - 1)
			{
				fileOut << ", ";
			}
			else
			{
				fileOut << "A" << endl; //I couldn't figure out how to detect a new line character every time so this is my new line character
			}
		}
	}
}

//reads in the file and converts each line into a vector and ignores the commas. Outputs a vector(same thing as an ArrayList)
vector<int> readFileIn(istream& fileIn, int vectorSize)
{
	char data = 0;
	string temp = "";
	vector<int> inputVector(1);
	int index = 0;
	while (index < vectorSize)
	{
		fileIn >> data;
		if (fileIn.eof())
		{
			inputVector.resize(index + 1);
			inputVector[index] = stoi(temp);
			temp = "";
			break;
		}
		if ((data - '0' >= 0 && data - '0' < 10) || data == '-') //checks if the char being read in is a number or a negative sign
		{
			temp += data;
		}
		if (data == ',' || data == 'A')//checks if comma or new line character (see previous method) and writes the stored value into a spot in the vector
		{
			inputVector.resize(index + 1);
			inputVector[index] = stoi(temp);
			temp = "";
			index++;
		}
	}
	return inputVector;
}

//calculates the t(n) value for each array
int algorithmCal(int algorithm, int arraySize)
{
	long long n = arraySize; //this is only a long long because vs2019 throws an arithmatic warning if its an int. Its kinda unecessary but it works
	long total = 0;
	switch (algorithm)// this decides which equation to use to calculate the t(n) value for the n array
	{
	case 1:
		total = ceil((9 * pow(n,3) + 60 * pow(n,2) + 58 * n + 24)/ 6);
		break;
	case 2:
		total = ceil(((13 * pow(n, 2)) + (17 * n) + 10) / 2);
		break;
	case 3:
		total = ceil((13 * pow(n, 2)) + (25 * n) - 29);
		break;
	case 4:
		total = ceil((13 * n) + 5);
		break;
	}
		
	return total;
}

//runs all of the algorithms and times them then writes it to a .csv file for graphing
void outputTimes(fstream& fileIn, fstream& valuesOutput)
{
	int arr1[100];
	double duration = 0.0;
	int n = 200;
	vector<int> copier;
	int tn1 = 0;
	int tn2 = 0;
	int tn3 = 0;
	int tn4 = 0;
	std::chrono::high_resolution_clock::time_point start; //I found this command on stack overflow for starting a clock. The other half of the command (line 219) I also found too.
	std::chrono::high_resolution_clock::time_point stop; //I found this command on stack overflow for stopping a clock. The other half of the command (line 224) I also found too.
	if (fileIn) //checks that it read the file in correctly
	{
		for (int i = 0; i < 19; i++) //used for each of the 19 vectors/arrays for each algorithm
		{
			copier = readFileIn(fileIn, ((i + 2) * 5)); //gets the vectors of whatever line its on when reading the file
			//puts the vector values into an array up to the size of the vector. This lets me reuse an array of size 100 and not have to make 19 array objects
			for (unsigned int j = 0; j < copier.size(); j++) 
			{
				arr1[j] = copier[j];
			}
			//each array size is run through each algorithm and timed and then the t(n) value is calculated for each algorithm
			
			//Algorithm 1. I will commment the commands for this section but algorithm 2, 3, and 4 are all almost the same
			start = std::chrono::high_resolution_clock::now(); //clock starts
			for (int k = 1; k < n + 1; k++) //used to run the algorithm n amount of times
			{
				Algorithm1(arr1, copier.size());
			}
			stop = std::chrono::high_resolution_clock::now(); //clock stops
			duration = (std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()); //I found this command on stack overflow for calculating the time. I use it 4 times in this function
			tn1 = algorithmCal(1, copier.size());// used to calculate the t(n) for the inputted array 
			valuesOutput << (duration*1000) / n; //the total time taken is multiplied by 1000 to get nano seconds
			//cout << ((i + 2) * 5) << ", " << duration / n; //this was used for testing purposes

			//Algorithm 2
			start = std::chrono::high_resolution_clock::now(); 
			for (int k = 1; k < n + 1; k++) 
			{
				Algorithm2(arr1, copier.size());
				
			}
			stop = std::chrono::high_resolution_clock::now(); 
			duration = (std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()); 
			tn2 = algorithmCal(2, copier.size());
			valuesOutput << ", " << (duration * 1000) / n;
			//cout << ", " << duration / n;

			//Algorithm 3
			start = std::chrono::high_resolution_clock::now(); 
			for (int k = 1; k < n + 1; k++) 
			{
				Algorithm3(arr1, 0, copier.size());				
			}
			stop = std::chrono::high_resolution_clock::now(); 
			duration = (std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count());
			tn3 = algorithmCal(3, copier.size());
			valuesOutput << ", " << (duration * 1000) / n;
			//cout << ", " << duration / n;

			//Algorithm 4
			start = std::chrono::high_resolution_clock::now(); 
			for (int k = 1; k < n + 1; k++) 
			{
				Algorithm4(arr1, copier.size());				
			}
			stop = std::chrono::high_resolution_clock::now(); 
			duration = (std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()); 	
			tn4 = algorithmCal(4, copier.size());
			valuesOutput << ", " << (duration * 1000) / n;
			//cout << ", " << duration / n;

			//Output for the tn values
			valuesOutput << ", " << tn1 << ", " << tn2 << ", " << tn3 << ", " << tn4 << endl;
			//cout << ", " << tn1 << ", " << tn2 << ", " << tn3 << ", " << tn4 << endl;
		}
	}
}

int main()
{
	vector<int> copier;
	int arr1[10];

	//Reading in the example file "phw_input.txt"
	ifstream exampleFile;
	exampleFile.open("phw_input.txt");
	copier = readFileIn(exampleFile, 10);// reads in the file and returns an array of n size
	//puts vector values into the array
	for (unsigned int j = 0; j < copier.size(); j++)
	{
		arr1[j] = copier[j];
	}
	//displays the required outputs
	cout << "Algorithm-1 = " << Algorithm1(arr1, copier.size()) <<
		"; Algorithm-2 = " << Algorithm2(arr1, copier.size()) <<
		"; Algorithm-3 = " << Algorithm3(arr1, 0, copier.size()) <<
		"; Algorithm-4 = " << Algorithm4(arr1, copier.size()) << endl;
	exampleFile.close();

	//creates a file with the 19 rows of 10,15,20...100 random numbers. Uses these to calculate the average run times of each algorithm.
	fstream fileOut("phw_randomValues.txt", ios::out);
	getRandomArrays(fileOut);
	fileOut.close();

	//Creates the input streams for the file that was just created and an output stream for the file with times
	fstream fileIn("phw_randomValues.txt", ios::in);
	fstream valuesOutput("HankWest_phw_output.csv", ios::out);
	
	//runs all 4 algorithms and outputs the data into a file with the times and T(n) calculations
	valuesOutput << "Algorithm-1, Algorithm-2, Algorithm-3, Algorithm-4, T1(n), T2(n), T3(n), T4(n)" << endl;
	outputTimes(fileIn, valuesOutput);
	fileIn.close();	

	return 0;
}

